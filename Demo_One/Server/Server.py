import threading
import time
import zlib
import socket
import json

DISCONNECT_TIMEOUT = 5.0


class Server:
    def __init__(self, host, tcp_port, udp_port):
        # TCP socket setup: This is for long-lived connections.
        self.tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.tcp_socket.bind((host, tcp_port))
        self.tcp_socket.listen(10)
        self.tcp_socket.setblocking(False)  # Accept loop remains non-blocking

        # UDP socket setup: This listens for UDP messages.
        self.udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.udp_socket.bind((host, udp_port))
        self.udp_socket.setblocking(False)

        # Dictionaries to track connections and client info.
        # TCP clients: key = client socket; value = {"address": addr, "last_seen": timestamp}
        self.tcp_connections = {}
        # UDP clients: key = (ip, port) tuple; value = last seen timestamp.
        self.udp_clients = {}
        self.clients = 0

        self.tcp_messages = []
        # For storing incoming UDP messages (if you want to relay them).
        self.udp_messages = []

        # Locks for thread-safe access.
        self.tcp_lock = threading.Lock()
        self.udp_lock = threading.Lock()

    def start_server(self):
        # Start background threads for handling timeouts and broadcasts.
        threading.Thread(target=self.check_tcp_timeouts, daemon=True).start()
        threading.Thread(target=self.check_udp_timeouts, daemon=True).start()
        threading.Thread(target=self.periodic_tcp_broadcast, daemon=True).start()
        threading.Thread(target=self.periodic_udp_broadcast, daemon=True).start()
        threading.Thread(target=self.udp_listener, daemon=True).start()
        print("TCP server started. Waiting for connections...")
        # Accept incoming TCP connections.
        while True:
            try:
                client_socket, addr = self.tcp_socket.accept()
                # Here we set non-blocking mode on the socket. Note: Using non-blocking
                # sending requires handling partial sends.
                client_socket.setblocking(True)
                with self.tcp_lock:
                    self.tcp_connections[client_socket] = {"address": addr, "last_seen": time.time()}
                    self.clients = self.clients + 1
                    json_data = json.dumps(self.clients).encode()
                    client_socket.sendall(json_data)
                threading.Thread(target=self.client_connected, args=(client_socket,), daemon=True).start()
                print(f"[+] TCP Connection from {addr}")
            except BlockingIOError:
                time.sleep(0.01)
                continue

    def client_connected(self, client_socket):
        # Retrieve the client's address
        with self.tcp_lock:
            addr = self.tcp_connections[client_socket]["address"]
        try:
            while True:
                data = client_socket.recv(4096)
                if not data:
                    # If no data is received, assume client has closed connection.
                    break
                try:
                    # Decompress and decode the JSON message.
                    decompressed = zlib.decompress(data)
                    message = json.loads(decompressed.decode())
                    print(f"Received TCP data from {addr}: {message}")
                except Exception as e:
                    print(f"Error parsing TCP message from {addr}: {e}")
                    continue
                # Update "last seen" for activity tracking.
                with self.tcp_lock:
                    self.tcp_connections[client_socket]["last_seen"] = time.time()
                    self.tcp_messages.append(message)
        except (ConnectionResetError, ConnectionAbortedError):
            print(f"[!] Lost TCP connection to {addr}")
        finally:
            with self.tcp_lock:
                if client_socket in self.tcp_connections:
                    del self.tcp_connections[client_socket]
            client_socket.close()
            print(f"[-] TCP Disconnected: {addr}")

    def check_tcp_timeouts(self):
        """ Disconnects TCP clients that have been idle too long. """
        while True:
            now = time.time()
            with self.tcp_lock:
                for sock in list(self.tcp_connections.keys()):
                    if now - self.tcp_connections[sock]["last_seen"] > DISCONNECT_TIMEOUT:
                        addr = self.tcp_connections[sock]["address"]
                        print(f"[!] TCP Timeout: {addr}")
                        sock.close()
                        del self.tcp_connections[sock]
            time.sleep(1)

    def broadcast_tcp(self, message):
        """
        Prepares and sends a message over TCP to all connected clients.
        Note: In non-blocking mode, send() may not send all bytes at once.
        """
        try:
            # Convert the message dictionary to JSON, then compress.
            json_data = json.dumps(message).encode()
            compressed_data = zlib.compress(json_data)
        except Exception as e:
            print("Error preparing TCP message:", e)
            return

        with self.tcp_lock:
            for sock, info in list(self.tcp_connections.items()):
                try:
                    sock.send(compressed_data)
                  #  print(f"Sent TCP message to {info['address']}")
                except Exception as e:
                    print(f"Error sending TCP message to {info['address']}: {e}")

    def periodic_tcp_broadcast(self):
        """ Periodically broadcasts a TCP update to all connected clients. """
        while True:
            update = {"type": "tcp_update", "time": time.time(), "tcp_messages": self.tcp_messages}
            self.broadcast_tcp(update)
            self.tcp_messages = []
            time.sleep(0.016)

    def udp_listener(self):
        """ Listens for incoming UDP messages. """
        while True:
            try:
                data, addr = self.udp_socket.recvfrom(4096)
            except BlockingIOError:
                time.sleep(0.01)
                continue
            except ConnectionResetError as e:
                #print(f"UDP Connection Reset")
                continue  # Continue listening for new messages
            try:
                #decompressed = zlib.decompress(data)
                #message = json.loads(data.decode())
                message = (data.decode())
                print(f"Received UDP data from {addr}: {message}")
            except Exception as e:
                print(f"Error parsing UDP message from {addr}: {e}")
                continue

            # Track the UDP client by updating last seen.
            with self.udp_lock:
                self.udp_clients[addr] = time.time()
                if message not in self.udp_messages:
                    self.udp_messages.append( message)

    def check_udp_timeouts(self):
        """ Removes UDP clients that haven't sent data recently. """
        while True:
            now = time.time()
            with self.udp_lock:
                for addr in list(self.udp_clients.keys()):
                    if now - self.udp_clients[addr] > DISCONNECT_TIMEOUT:
                        print(f"[!] UDP Timeout: {addr}")
                        self.clients -= 1
                        del self.udp_clients[addr]
            time.sleep(1)

    def broadcast_udp(self, message):
        """
        Prepares and sends a message via UDP to all known UDP clients.
        """

        try:
            json_data = json.dumps(message).encode()
            #compressed_data = zlib.compress(json_data)
        except Exception as e:
            print("Error preparing UDP message:", e)
            return

        with self.udp_lock:
            for addr in list(self.udp_clients.keys()):
                try:
                    self.udp_socket.sendto(json_data, addr)
                    #print(f"Sent UDP message to {addr}")
                except Exception as e:
                    print(f"Error sending UDP message to {addr}: {e}")



    def periodic_udp_broadcast(self):
        """ Periodically broadcasts a UDP update to all tracked UDP clients. """
        while True:
            # Prepare an update that includes current time and any accumulated UDP messages.
            if self.udp_messages:
                with self.udp_lock:
                    update = {"type": "udp_update", "time": time.time(), "udp_messages": self.udp_messages}
                    # Clear messages after broadcasting.
                    self.udp_messages = []
                self.broadcast_udp(update)
                time.sleep(0.016)


if __name__ == "__main__":
    # Use different ports for TCP and UDP (example: TCP on 5555 and UDP on 5556)
    server = Server("localhost", tcp_port=5555, udp_port=5556)
    server.start_server()

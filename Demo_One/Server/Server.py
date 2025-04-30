import threading
import time
import zlib
import socket
import json

DISCONNECT_TIMEOUT = 5.0


class Server:
    def __init__(self, host, tcp_port, udp_port):
        # TCP socket
        self.tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.tcp_socket.bind((host, tcp_port))
        self.tcp_socket.listen(10)
        self.tcp_socket.setblocking(False)

        # UDP socket
        self.udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.udp_socket.bind((host, udp_port))
        self.udp_socket.setblocking(False)

        #clients
        self.tcp_connections = {}
        self.udp_clients = {}
        self.clients = 0

        #client info
        self.tcp_messages = []
        self.udp_messages = []

        self.tcp_lock = threading.Lock()
        self.udp_lock = threading.Lock()

    def start_server(self):
        """ Starts threads for sending messages for tcp and udp clients and listens for tcp connections"""
        threading.Thread(target=self.check_tcp_timeouts, daemon=True).start()
        threading.Thread(target=self.check_udp_timeouts, daemon=True).start()
        threading.Thread(target=self.tcp_message_timer, daemon=True).start()
        threading.Thread(target=self.udp_message_timer, daemon=True).start()
        threading.Thread(target=self.udp_listener, daemon=True).start()


        while True:
            try:
                client_socket, addr = self.tcp_socket.accept()
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
        """ Listens for data from a tcp connection """
        # Retrieve the client's address
        with self.tcp_lock:
            addr = self.tcp_connections[client_socket]["address"]
        try:
            while True:
                data = client_socket.recv(4096)
                if not data:
                    break
                try:
                    decompressed = zlib.decompress(data)
                    message = json.loads(decompressed.decode())
                except Exception as e:
                    print(f"Error parsing TCP message from {addr}: {e}")
                    continue
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
        """ Removes TCP clients that haven't sent data recently. """
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

    def tcp_send_message(self, message):
        """ Sends the message to all connected TCP clients """
        try:
            json_data = json.dumps(message).encode()
            compressed_data = zlib.compress(json_data)
        except Exception as e:
            print("Error preparing TCP message:", e)
            return

        with self.tcp_lock:
            for sock, info in list(self.tcp_connections.items()):
                try:
                    sock.send(compressed_data)
                except Exception as e:
                    print(f"Error sending TCP message to {info['address']}: {e}")

    def tcp_message_timer(self):
        """  Prepares a message to be sent to all TCP clients 60 times a second. """
        while True:
            update = {"type": "tcp_update", "time": time.time(), "tcp_messages": self.tcp_messages}
            self.tcp_send_message(update)
            self.tcp_messages = []
            time.sleep(0.016)

    def udp_listener(self):
        """ Receives UDP messages and adds them to the tracked clients. """
        while True:
            try:
                data, addr = self.udp_socket.recvfrom(4096)
            except BlockingIOError:
                time.sleep(0.01)
                continue
            except ConnectionResetError as e:
                continue
            try:
                message = (data.decode())
            except Exception as e:
                print(f"Error UDP message from {addr}: {e}")
                continue

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
                        self.clients -= 1
                        del self.udp_clients[addr]
            time.sleep(1)

    def udp_send_message(self, message):
        """ Sends the message to all connected udp clients """

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



    def udp_message_timer(self):
        """ Prepares a message to be sent to all UDP clients 60 times a second. """
        while True:
            if self.udp_messages:
                with self.udp_lock:
                    update = {"type": "udp_update", "time": time.time(), "udp_messages": self.udp_messages}
                    self.udp_messages = []
                self.udp_send_message(update)
                time.sleep(0.016)


if __name__ == "__main__":
    server = Server("localhost", tcp_port=5555, udp_port=5556)
    server.start_server()

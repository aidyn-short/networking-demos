#pragma once
#include <memory>
#include <string>
#include <vector>
#include <iostream>


typedef std::vector<char> ByteString;

class Address
{
public:
	Address() : _data{ 0 } {}
	Address(const std::string& name, int port);

	struct AddressData
	{
		char data[32];
	} _data;
};




class Socket
{
public:
	

	enum Family
	{
		INET,
		INET6
	};

	enum Type
	{
		UDP,
		TCP
	};

	

	enum Error {
		SOCKLIB_ETIMEDOUT,
	};


	Socket(Family family, Type type);
	~Socket();

	Socket(Socket&& other) noexcept;
	Socket& operator=(Socket&& other) noexcept;

	Socket(const Socket&) = delete;
	Socket& operator=(const Socket&) = delete;


	int GetLastError();

	int SetNonBlockingMode(bool shouldBeNonBlocking);
	int SetTimeout(float seconds);

	void Create(Family family, Type type);
	int Bind(const Address& address);
	int Listen(int backlog = 16);
	Socket Accept();
	int Connect(const Address& address);
	int Recv(char* buffer, int size);
	int Recv(ByteString& buffer);
	int RecvFrom(char* buffer, int size, Address& src);
	size_t Send(const char* data, size_t len);
	size_t SendTo(const char* buffer, size_t len, const Address& dest);
	size_t SendAll(const char* data, size_t len);
	size_t SendAll(const ByteString& data);

	static void native_destroy(Socket& socket);

	bool _has_socket = false;
	int _last_error;

	struct SocketData
	{
		char data[32];
	} _data;
};


void SocketInit();
void SocketShutdown();



ByteString to_bytestring(const char* msg, size_t len);
std::ostream& operator<<(std::ostream& s, const ByteString& b);
std::ostream& operator<<(std::ostream& s, const Address& a);

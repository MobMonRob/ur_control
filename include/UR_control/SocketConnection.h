#ifndef SOCKET_CONNECTION_H
#define SOCKET_CONNECTION_H

#include <string>
#include <boost/asio.hpp>

class SocketConnection
{
public:
	SocketConnection(const std::string& ip, int port);
	void send(const std::string& message);
	std::string receive();

private:
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::socket socket;

};

#endif


#ifndef ROBOT_CONNECTION
#define ROBOT_CONNECTION

#include <string>
#include <boost/asio.hpp>

class RobotConnection
{
public:
	RobotConnection(const std::string& ip, int port);
	void send(const std::string& message);
	std::string receive();

private:
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::socket socket;

};

#endif


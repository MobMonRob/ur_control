#include "UR_control/SocketConnection.h"

#include <iostream>
#include <boost/asio.hpp>

using namespace boost;
using namespace asio;
using ip::tcp;
using ip::address_v4;

SocketConnection::SocketConnection(const std::string& ip, int port)
: io_service(), socket(io_service)
{
	socket.connect(tcp::endpoint(address_v4::from_string(ip), port));
}

void SocketConnection::send(const std::string& message)
{
	system::error_code error;
	asio::write(socket, asio::buffer(message), error);
	
	if( error ) {
        throw error.message();
     }
}

std::string SocketConnection::receive()
{
	system::error_code error;
	asio::streambuf receive_buffer;

	asio::read_until(socket, receive_buffer, '\n', error);
	//asio::read(socket, receive_buffer, transfer_at_least(1), error);

    if( error && (error != error::eof) ) {
        throw error.message();
    }
    else {
        return std::string(asio::buffer_cast<const char*>(receive_buffer.data()), receive_buffer.size());
    }
}


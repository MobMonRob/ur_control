#ifndef SERVER_NODE
#define SERVER_NODE

#include "robot_control/robotCommand.h" ///home/oliver/robot_command_ws/devel/include/robot_control/

#include <memory>
#include <string>

#include "ros/ros.h"

#include "../socketLib/Socket.h"
#include "../socketLib/ProtocolSimple.h"
namespace Socket = ThorsAnvil::Socket;

class ServerNode
{
public:
	ServerNode(int argc, char **argv);
	void start();

private:
	std::unique_ptr<ros::NodeHandle> node;
	std::string roboter_ip;
	int roboter_port;
	static std::string program_name;
	static std::unique_ptr<Socket::ConnectSocket> connectSocket;
	static std::unique_ptr<Socket::ProtocolSimple> protocolSimple;

	static bool robotCommand(robot_control::robotCommandRequest &request, robot_control::robotCommandResponse &response);
};

#endif

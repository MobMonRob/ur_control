#ifndef SERVER_NODE
#define SERVER_NODE

#include "robot_control/robotCommand.h"
#include "robot_control/RobotConnection.h"

#include <memory>
#include <string>

#include "ros/ros.h"

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
	static std::unique_ptr<RobotConnection> robotConnection;

	static bool robotCommand(robot_control::robotCommandRequest &request, robot_control::robotCommandResponse &response);
};

#endif


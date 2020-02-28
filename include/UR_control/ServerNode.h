#ifndef SERVER_NODE
#define SERVER_NODE

#include "UR_control/robotCommand.h"
#include "UR_control/RobotConnection.h"

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
	//static std::string program_name;
	static std::unique_ptr<RobotConnection> robotConnection;

	static bool robotCommand(UR_control::robotCommandRequest &request, UR_control::robotCommandResponse &response);
	static std::optional<std::string> decodeCommand(UR_control::robotCommandRequest& request);
	static std::string sendCommandToRobot(std::string& command);
};

#endif


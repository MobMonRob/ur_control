#ifndef SERVER_NODE_H
#define SERVER_NODE_H

#include "UR_control/robotCommand.h"
#include "UR_control/RobotConnection.h"

#include <memory>
#include <string>

#include "ros/ros.h"
#include <XmlRpcClient.h>

class ServerNode
{
public:
	ServerNode(int argc, char **argv);
	void start();

private:
	std::unique_ptr<ros::NodeHandle> node;
	std::string roboter_ip;
	int roboter_port;
	std::string xmlrpc_ip;
	int xmlrpc_port;
	static std::string program_name;
	static std::unique_ptr<RobotConnection> robotConnection;
	static std::unique_ptr<XmlRpc::XmlRpcClient> xmlRpcClient;

	static bool robotCommand(UR_control::robotCommandRequest &request, UR_control::robotCommandResponse &response);
	static std::string requestCommandLOAD(const std::string& command);
	static std::string requestCommandPLAY(const std::string& command);
};

#endif


#ifndef CLIENT_NODE_H
#define CLIENT_NODE_H

#include "UR_control/robotCommand.h"

#include <memory>

#include "ros/ros.h"

class ClientNode
{
public:
	ClientNode(int argc, char **argv);
	void robotCommand(UR_control::robotCommandRequest &commandReq);

	//Folgende Abstraktionen gehören in eine weitere Klasse
	void test();

private:
	std::unique_ptr<ros::NodeHandle> node;
	std::unique_ptr<ros::ServiceClient> client;

	void testOneCommand(UR_control::robotCommandRequest::_command_type command);
};

#endif


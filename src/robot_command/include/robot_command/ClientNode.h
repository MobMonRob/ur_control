#ifndef CLIENT_NODE
#define CLIENT_NODE

#include "robot_command/robotCommand.h" ///home/oliver/robot_command_ws/devel/include/robotCommand

#include <memory>

#include "ros/ros.h"

class ClientNode
{
	public:
	ClientNode(int argc, char **argv);
	void robotCommand(robot_command::robotCommandRequest::_command_type& command);

	//Folgende Abstraktionen gehören in eine weitere Klasse
	void test();
	
	private:
	std::unique_ptr<ros::NodeHandle> node;
	std::unique_ptr<ros::ServiceClient> client;
};

#endif


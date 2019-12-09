#ifndef SERVER_NODE
#define SERVER_NODE

#include "robot_command/robotCommand.h" ///home/oliver/robot_command_ws/devel/include/robotCommand

#include <memory>

#include "ros/ros.h"

class ServerNode
{
	public:
	ServerNode(int argc, char **argv);
	void start();
	
	private:
	std::unique_ptr<ros::NodeHandle> node;
	//static std::unique_ptr<GpioManager> gpio;
	static bool robotCommand(robot_command::robotCommandRequest& request, robot_command::robotCommandResponse& response);
};

#endif


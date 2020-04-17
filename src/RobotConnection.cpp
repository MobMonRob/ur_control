#include "UR_control/RobotConnection.h"
#include "ros/ros.h"

RobotConnection::RobotConnection(std::unique_ptr<SocketConnection>& socketConnection)
:socketConnection(std::move(socketConnection))
{
}

std::string RobotConnection::sendCommandToRobot(const std::string& command)
{
	std::string alteredCommand(command);
	alteredCommand.append("\n"); //Anforderung vom UR Dashboard Server
	
	socketConnection->send(alteredCommand);

	std::string response = socketConnection->receive();
	ROS_INFO("Robot response:\n%s", response.c_str());

	return response;
}


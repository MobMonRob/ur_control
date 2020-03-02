#ifndef ROBOT_CONNECTION_H
#define ROBOT_CONNECTION_H

#include "UR_control/SocketConnection.h"
#include <memory>

class RobotConnection
{
public:
	RobotConnection(std::unique_ptr<SocketConnection>& socketConnection);
	std::string sendCommandToRobot(const std::string& command);

private:
	std::unique_ptr<SocketConnection> socketConnection;
};

#endif


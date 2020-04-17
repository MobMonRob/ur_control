#ifndef ROBOT_CONNECTION_H
#define ROBOT_CONNECTION_H

#include "UR_control/SocketConnection.h"
#include <memory>

class RobotConnection
{
public:
	//Komposition; Als Aggregation sinnvoller?
	//Alternative Umsetzung Komposition: RobotConnection baut socketConnection (Konstuktor nimmt entsprechende Attribute); getter für socketConnection
	//Aggregation: ServerNode speichert socketConnection; RobotConnection kennt nur socketConnection (kein move in Konstruktor)
	RobotConnection(std::unique_ptr<SocketConnection>& socketConnection);
	std::string sendCommandToRobot(const std::string& command);

private:
	std::unique_ptr<SocketConnection> socketConnection;
};

#endif


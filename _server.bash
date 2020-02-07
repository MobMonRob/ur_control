#!/bin/bash

#Starten mit:
# ./_server.bash

cd /home/oliver/robot_command_ws
source devel/setup.bash
roslaunch robot_control Server.launch


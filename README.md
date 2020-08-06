Workspace:
/home/oliver/robot_control_ws/

Dokumentation zu Running:
https://www.universal-robots.com/how-tos-and-faqs/how-to/ur-how-tos/dashboard-server-cb-series-port-29999-15690/

XMLRPC Client Lib:
http://docs.ros.org/api/xmlrpcpp/html/namespaceXmlRpc.html
http://docs.ros.org/api/xmlrpcpp/html/classXmlRpc_1_1XmlRpcSocket.html#details
http://docs.ros.org/api/xmlrpcpp/html/classXmlRpc_1_1XmlRpcClient.html#details

-----------------------------------------------------------


Verwendung
-----------------------------------------------------------
Man muss den Roboter anschalten, bevor man etwas Sinnvolles bekommt bei LOAD
Dann remote aktivieren

export ROS_MASTER_URI=http://192.168.10.2:11311
roscore

XMLRPC Server starten:
python3 ./src/robot_control/src/UR_communication_without_KI.py

Server starten:
bash _server.bash

Test Client starten:
bash _client.bash
-----------------------------------------------------------


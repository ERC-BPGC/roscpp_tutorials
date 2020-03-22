#include "ros/ros.h"
#include "std_msgs/String.h"

//#include <sstream>
//#include <iostream>
//#include <stdio.h>
//using namespace std;

void chatterCallback(const std_msgs::String::ConstPtr& msg){
	ROS_INFO("Message Received: [%s]", msg->data.c_str());
}


int main(int argc, char* argv[]){

	ros::init(argc, argv, "listener");

	ros::NodeHandle n;

	ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);

	ros::spin();
	
	return 0;
}
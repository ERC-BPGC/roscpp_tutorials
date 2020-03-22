#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

int main(int argc, char* argv[]){


	ros::init(argc, argv, "talker");
	//Parametric/ remapping info given through command line by default

	ros::NodeHandle n;
	//NodeHandle is the communication access point with ROS
	//First NodeHandle constructor will initialize the node
	//Last destructor(automatic) will close down the node

	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
	//advertize() returns a publisher
	//Lists itself with the master to establish P2P with every subscriber
	//Second srgument is the queue size

	ros::Rate loop_rate(10);

	int count =0;

	while(ros::ok()){
		//Message object
		std_msgs::String msg;

		//Creating a string
		std::stringstream ss;
		ss<<"Messages Published: "<<count;

		//Making a string from the stream and filling the message with it
		msg.data = ss.str();

		//
		ROS_INFO("%s", msg.data.c_str());

		//Publish, spin and sleep
		chatter_pub.publish(msg);
		//msg should have the same tye as advertise<>()

		ros::spinOnce();
		loop_rate.sleep();
		count++;
	}

	return 0;
}
#include "ros/ros.h"
#include "basics/AddTwoInts.h"

bool add(basics::AddTwoInts::Request &req,
	basics::AddTwoInts::Response &res){

	//
	res.Sum = req.A + req.B;

	ROS_INFO("Request: x=%ld  y=%ld", (long int)req.A, (long int)req.B);
	ROS_INFO("Sending Response: [%ld]", (long int)res.Sum);

	return true;
}

int main(int argc, char* argv[]){

	ros::init(argc, argv, "add_two_ints_server");
	ros::NodeHandle n;

	ros::ServiceServer service = n.advertiseService("add_two_ints", add);

	ROS_INFO("Service ready...");

	ros::spin();
	return 0;
}
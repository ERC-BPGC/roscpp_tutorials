#include "ros/ros.h"
#include "basics/AddTwoInts.h"

#include <cstdlib>
#include <iostream>

int main(int argc, char* argv[]){

	ros::init(argc, argv, "add_two_ints_client");

	if(argc!=3){
		ROS_INFO("Incorrect usage\n Usage: add_two_ints_client A B\n");
		return 1;
	}

	ros::NodeHandle nh;
	ros::ServiceClient client = nh.serviceClient<basics::AddTwoInts>("add_two_ints");

	basics::AddTwoInts srv;
	srv.request.A = atoi(argv[1]);
	srv.request.B = atoi(argv[2]);

	if(client.call(srv)){
		ROS_INFO("Sum: %ld", (long int)srv.response.Sum);
	}else{
		ROS_ERROR("Failed!");
		return 1;
	}

	return 0;
}
#include "Stopper.h"
#include "geometry_msgs/Twist.h"

Stopper::Stopper(){

	keepMoving = true;

	command_pub = node.advertise<geometry_msgs::Twist>(
		"/cmd_vel_mux/input/teleop", 10);
	laser_sub = node.subscribe("scan", 1, &Stopper::scanCallback, this);

}


void Stopper::moveForward(){
	geometry_msgs::Twist msg;
	msg.linear.x = FORWARD_SPEED;
	command_pub.publish(msg);
}

void Stopper::scanCallback(const sensor_msgs::LaserScan::ConstPtr &scan){
	bool isObstInFront = false;

	int minIndex = ceil((MIN_SCAN_ANGLE - scan->angle_min)/scan->angle_increment);
	int maxIndex = floor((MAX_SCAN_ANGLE - scan->angle_min)/scan->angle_increment);

	for(int i = minIndex; i<maxIndex; i++){
		if(scan->ranges[i] < MIN_DIST){
			isObstInFront = true;
			break;
		}
	}

	if(isObstInFront){
		ROS_INFO("Stop!");
		keepMoving = false;
	}
}


void Stopper::startMoving(){
	ros::Rate rate(10);
	ROS_INFO("Start Motion");

	while(ros::ok()){
		moveForward();
		ros::spinOnce();

		rate.sleep();
	}
}
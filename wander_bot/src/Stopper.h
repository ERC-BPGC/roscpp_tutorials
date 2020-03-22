#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"

class Stopper{

public:
	//Tunable params

	const static double FORWARD_SPEED = 0.5;
	const static double MIN_SCAN_ANGLE = -30.0/180 * M_PI;
	const static double MAX_SCAN_ANGLE = 30.0/180 * M_PI;
	const static double MIN_DIST = 0.5;

	//Constructor
	Stopper();

	//Functions

	void startMoving();


private:

	ros::NodeHandle node;
	ros::Publisher command_pub;
	ros::Subscriber laser_sub;
	bool keepMoving;

	//Functions

	void moveForward();
	void scanCallback(const sensor_msgs::LaserScan::ConstPtr &msg);
};
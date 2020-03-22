/*
 * Follower.h
 *
 *  Created on: Nov 11, 2016
 *      Author: viki
 */

#ifndef FOLLOW_BOT_SRC_FOLLOWER_H_
#define FOLLOW_BOT_SRC_FOLLOWER_H_

#include <ros/ros.h>
#include <image_transport/image_transport.h>


class Follower {
private:
	ros::NodeHandle nh;
	image_transport::ImageTransport imageTransport;
	image_transport::Subscriber imageSubscriber;
	ros::Publisher cmdVelPublisher;

	void imageCallback(const sensor_msgs::ImageConstPtr& msg);

public:
	Follower();
	virtual ~Follower();
};

#endif /* FOLLOW_BOT_SRC_FOLLOWER_H_ */

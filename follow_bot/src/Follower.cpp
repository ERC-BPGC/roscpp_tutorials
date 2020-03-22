/*
 * Follower.cpp
 *
 *  Created on: Nov 11, 2016
 *      Author: viki
 */

#include "Follower.h"
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <geometry_msgs/Twist.h>

static const std::string OPENCV_WINDOW = "Image window";

Follower::Follower() : imageTransport(nh) {
	imageSubscriber = imageTransport.subscribe("/camera/rgb/image_raw", 1,
			&Follower::imageCallback, this);
	cmdVelPublisher = nh.advertise<geometry_msgs::Twist>("/cmd_vel_mux/input/teleop", 10);

	// Create a display window
	cv::namedWindow(OPENCV_WINDOW);
}

Follower::~Follower() {
	// Close the display window
	cv::destroyWindow(OPENCV_WINDOW);
}

void Follower::imageCallback(const sensor_msgs::ImageConstPtr& msg) {
	// convert the ROS image message to a CvImage
	cv_bridge::CvImagePtr cv_ptr;
	try {
		cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	}
	catch (cv_bridge::Exception& e) {
		ROS_ERROR("cv_bridge exception: %s", e.what());
		return;
	}

	// Convert input image to HSV
	cv::Mat image = cv_ptr->image;
	cv::Mat hsvImage;
	cv:cvtColor(image, hsvImage, CV_BGR2HSV);

	// Threshold the HSV image, keep only the yellow pixels
	cv::Scalar lower_yellow(20, 100, 100);
	cv::Scalar upper_yellow(30, 255, 255);

	cv::Mat mask;
	cv::inRange(hsvImage, lower_yellow, upper_yellow, mask);

	int width = mask.cols;
	int height = mask.rows;

	int search_top = 3 * height / 4;
	int search_bottom = search_top + 20;

	// Zero out pixels outside the desired region
	for (int y = 0; y < height - 2; y++) // For some reason, cannot modify the last two rows
	{
		if (y < search_top || y > search_bottom) {
			for (int x = 0; x < width; x++) {
				mask.at<cv::Vec3b>(y, x)[0] = 0;
				mask.at<cv::Vec3b>(y, x)[1] = 0;
				mask.at<cv::Vec3b>(y, x)[2] = 0;
			}
		}
	}

	// Use the OpenCV moments() function to calculate the centroid of the blob of the binary image
	cv::Moments M = cv::moments(mask);

	if (M.m00 > 0) {
		int cx = int(M.m10 / M.m00);
		int cy = int(M.m01 / M.m00);
		cv::circle(image, cv::Point(cx, cy), 20, CV_RGB(255, 0, 0), -1);

		// Move the robot in proportion to the error signal
		int err = cx - width / 2;
		geometry_msgs::Twist cmd;
		cmd.linear.x = 0.2;
		cmd.angular.z = -(float)err / 100;
		cmdVelPublisher.publish(cmd);
	}

	// Update the GUI window
	cv::imshow(OPENCV_WINDOW, image);
	cv::waitKey(3);
}

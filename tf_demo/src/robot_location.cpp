/*
 * robot_location.cpp
 *
 *  Created on: Nov 30, 2014
 *      Author: roiyeho
 */

#include <ros/ros.h>
#include <tf/transform_listener.h>

using namespace std;

int main(int argc, char** argv){
    ros::init(argc, argv, "robot_location");
    ros::NodeHandle node;

    tf::TransformListener listener;
    ros::Rate rate(2.0);

    //listener.waitForTransform("/odom", "/base_footprint", ros::Time(0), ros::Duration(10.0) );
    listener.waitForTransform("/map", "/base_footprint", ros::Time(0), ros::Duration(10.0) );

    while (ros::ok()){
        tf::StampedTransform transform;
        try {
            //listener.lookupTransform("/odom", "/base_footprint", ros::Time(0), transform);
            listener.lookupTransform("/map", "/base_footprint", ros::Time(0), transform);
            double x = transform.getOrigin().x();
            double y = transform.getOrigin().y();

            cout << "Current position: (" << x << "," << y << ")" << endl;
        } catch (tf::TransformException &ex) {
            ROS_ERROR("%s",ex.what());
        }
        rate.sleep();
    }

    return 0;
}




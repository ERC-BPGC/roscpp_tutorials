/*
 * run_stopper.cpp
 *
 *  Created on: Dec 12, 2016
 *      Author: Roi Yehoshua
 */

#include <ros/ros.h>
#include <nav_msgs/GetMap.h>
#include <vector>
#include <fstream>

using namespace std;

// Grid map definition
int rows;
int cols;
double mapResolution;
vector<vector<bool> > grid;

bool requestMap(ros::NodeHandle &nh);
void readMap(const nav_msgs::OccupancyGrid& msg);
void printGridToFile();

int main(int argc, char **argv)
{
    ros::init(argc, argv, "load_map");
    ros::NodeHandle nh;

    if (!requestMap(nh))
        exit(-1);

    printGridToFile();
    return 0;
}
	
bool requestMap(ros::NodeHandle &nh) {
    nav_msgs::GetMap::Request req;
    nav_msgs::GetMap::Response res;

    while (!ros::service::waitForService("static_map", ros::Duration(3.0))) {
        ROS_INFO("Waiting for service static_map to become available");
    }

    ROS_INFO("Requesting the map...");
    ros::ServiceClient mapClient = nh.serviceClient<nav_msgs::GetMap>(
            "static_map");

    if (mapClient.call(req, res)) {
        readMap(res.map);
        return true;
    } else {
        ROS_ERROR("Failed to call map service");
        return false;
    }
}

void readMap(const nav_msgs::OccupancyGrid& map) {
    ROS_INFO("Received a %d X %d map @ %.3f m/px\n", map.info.width,
            map.info.height, map.info.resolution);

    rows = map.info.height;
    cols = map.info.width;
    mapResolution = map.info.resolution;
    int currCell = 0;

    // Dynamically resize the grid
    grid.resize(rows);
    for (int i = 0; i < rows; i++) {
        grid[i].resize(cols);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (map.data[currCell] == 0) // unoccupied cell
                grid[i][j] = false;
            else
                grid[i][j] = true; // occupied (100) or unknown cell (-1)
            currCell++;
        }
    }
}

void printGridToFile() {
    ofstream gridFile;
    gridFile.open("grid.txt");
  
    for (int i = grid.size() - 1; i >= 0; i--) {        
        for (int j = 0; j < grid[0].size(); j++) {
	    gridFile << (grid[i][j] ? "1" : "0");           
        }
        gridFile << endl;
    }
    gridFile.close();
}




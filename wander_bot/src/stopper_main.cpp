#include "Stopper.h"

int main(int argc, char* argv[]){
	//
	ros::init(argc, argv, "stopper");

	Stopper stopper;

	stopper.startMoving();

	return 0;
};
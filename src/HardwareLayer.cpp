/* 
 * HardwareLayer.cpp
 * 
 * Created on: Feb 7, 2016
 * Author: Zaeem
 *
 */

#include <cstdio>
#include "WPILib.h"
#include "HardwareLayer.h"

#define CORE_THREAD_HZ 50

double SystemTime() {
	return GetFPGATime() / 1000.0;	
}

Robot2016::Robot2016(){

}

void Robot2016::RobotInit(){
	
}

void Robot2016::RobotMain(){
	//init
	float deltaTime = 1.0f / CORE_THREAD_HZ;
	double targetMillisPerFrame = 1000.0 * deltaTime;
	double lastTime = SystemTime();
	for(;;) {
		//update robot
		double elapsedMS = SystemTime() - lastTime;
		if(elapsedMS < targetMillisPerFrame) {
			Wait((targetMillisPerFrame - elapsedMS) / 1000.0f);
			double testElapsedMS = SystemTime() - lastTime;

			if(targetMillisPerFrame >= targetMillisPerFrame) {
				//log too long waited
			} else {
				do {
					elapsedMS = SystemTime() - lastTime;
				} while(elapsedMS <= targetMillisPerFrame);
			}

		} else {
			//log missed last frame
		}

		double endTime = SystemTime();
		double frameTimeMS = endTime - lastTime;
		lastTime = endTime;
		double hz = 1000.0 / frameTimeMS;
		//print
	}
}

Robot2016::~Robot2016(){
}

int main(){
 	if(!HALInitialize()){
 		//TODO: log
 		return -1;
 	}
 	HALReport(HALUsageReporting::kResourceType_Language, HALUsageReporting::kLanguage_CPlusPlus);

 	Robot2016* robot = new Robot2016();
 	RobotBase::robotSetup(robot);

 	return 0;
}
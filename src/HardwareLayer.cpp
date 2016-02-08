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

Robot2016::Robot2016(){

}

void Robot2016::RobotInit(){
	
}

void Robot2016::RobotMain(){

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
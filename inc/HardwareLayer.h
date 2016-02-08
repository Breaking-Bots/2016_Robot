#ifndef HARDWARE_LAYER_H
#define HARDWARE_LAYER_H

/* 
 * HardwareLayer.h
 * 
 * Created on: Feb 7, 2016
 * Author: Zaeem
 *
 */

#ifdef __cplusplus
extern "C" {
#endif 

class Robot2016 : public SampleRobot{
public:
	Robot2016();

	void RobotInit();

	void RobotMain();

	~Robot2016();
};

#ifdef __cplusplus
}
#endif 

#endif
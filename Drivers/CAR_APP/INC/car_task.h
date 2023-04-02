#ifndef __CAR_TASK_H
#define __CAR_TASK_H

#include "main.h"
#include "sys.h"
#include "mpu6050.h"
#include "inv_mpu_user.h"
#include "control.h"

struct mpu6050_data{
	
		short acc_x;
		short acc_y;
		short acc_z;
		
		short gyro_x;
		short gyro_y;
		short gyro_z;
	
		float pitch;    //¸©Ñö½Ç
	  float roll;     //·­¹ö½Ç
	  float yaw;      //Æ«º½½Ç
};

extern struct mpu6050_data OutMpu;


void Car_Task_200HZ(void);
void Car_Task_100HZ(void);
void Printf_Task(void);

#endif  /** __CAR_TASK_H */


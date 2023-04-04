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
	
		float pitch;    //俯仰角
	  float roll;     //翻滚角
	  float yaw;      //偏航角
};

extern struct mpu6050_data OutMpu;

extern int  Balance_Pwm,Velocity_Pwm,Turn_Pwm;        //PID计算的PWM值
extern int  Motor1, Motor2;                  //左右电机PWM值
extern int  Encoder_left, Encoder_right;     //检测速度
extern float Movement;                   //速度调节  
extern int  Contrl_Turn;                //转向调节变量

void Car_Task_200HZ(void);
void Car_Task_100HZ(void);
void Printf_Task(void);

#endif  /** __CAR_TASK_H */


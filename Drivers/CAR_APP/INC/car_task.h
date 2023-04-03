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
	
		float pitch;    //������
	  float roll;     //������
	  float yaw;      //ƫ����
};

extern struct mpu6050_data OutMpu;

extern int  Balance_Pwm,Velocity_Pwm,Turn_Pwm;        //PID�����PWMֵ
extern int  Motor1, Motor2;                  //���ҵ��PWMֵ
extern int  Encoder_left, Encoder_right;     //����ٶ�
extern float Movement;                   //�ٶȵ���  
extern int  Contrl_Turn;                //ת����ڱ���

void Car_Task_200HZ(void);
void Car_Task_100HZ(void);
void Printf_Task(void);

#endif  /** __CAR_TASK_H */


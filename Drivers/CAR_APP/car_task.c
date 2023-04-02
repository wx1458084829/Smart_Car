#include "car_task.h"

int  Balance_Pwm,Velocity_Pwm,Turn_Pwm;        //PID计算的PWM值
int  Motor1, Motor2;                  //左右电机PWM值
int  Encoder_left, Encoder_right;     //检测速度
float Movement = -0;                   //速度调节  
int  Contrl_Turn = 64;                //转向调节变量

//环境数据采集任务
void Car_Task_200HZ(void)
{
		static struct mpu6050_data Last_Data;
	
	
		if(mpu_dmp_get_data() !=0 )
		{
			OutMpu = Last_Data;
		}
		else
		{
		Last_Data = OutMpu;
		}	
}

//PID控制
void Car_Task_100HZ(void)
{

		
		
	Encoder_left  = Read_Encoder(1);
	Encoder_right = -Read_Encoder(2);
	
	//1、确定直立环PWM
	
		Balance_Pwm = Vertical_Ring_PD(OutMpu.pitch, OutMpu.gyro_x);
	
	//2、确定速度环PWM
	
	  Velocity_Pwm = Vertical_speed_PI(Encoder_left,Encoder_right,OutMpu.pitch, Movement );
	
	
	//3、确定转向环PWM
	
//		Turn_Pwm = Vertical_turn_PD(Contrl_Turn, OutMpu.gyro_z);
	
	//4、确定最终左右电机的PWM
		Motor1 = Balance_Pwm + Velocity_Pwm + Turn_Pwm;
	  Motor2 = Balance_Pwm + Velocity_Pwm - Turn_Pwm;
	
		PWM_Limiting(&Motor1,&Motor2);
	
	
	//5、设置电机
		Set_PWM(Motor1,Motor2);

}

void Printf_Task(void)
{
	//printf("%d %d\r\n",Encoder_left,Encoder_right);
		printf("%f,%f,%f -- \r\n",OutMpu.pitch,OutMpu.roll,OutMpu.yaw);
		//printf("%d---- %d\r\n",Encoder_left,Encoder_right);
}

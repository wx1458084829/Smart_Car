#include "car_task.h"


int  Balance_Pwm,Velocity_Pwm,Turn_Pwm;        //PID计算的PWM值
int  Motor1, Motor2;                  //左右电机PWM值
int  Encoder_left, Encoder_right;     //检测速度
float Movement = 0;                   //速度调节  
int  Contrl_Turn = 64;                //转向调节变量
uint8_t  Car_Status = 0;  	//小车状态
int Max_Speed = 60; //小车限制速度

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
	//获取左右编码器的值，即速度，注意取负(因为编码器位置是对称的)
	Encoder_left  = Read_Encoder(1);
	Encoder_right = -Read_Encoder(2);
	if(Car_Status){
		Motor1 = Motor2 =0;
		Set_PWM(Motor1,Motor2);
		return;
	}
	if(abs(Encoder_left)>Max_Speed||abs(Encoder_right)>Max_Speed){
			Car_Status = 1;
	}
	
	//直立环PWM
		Balance_Pwm = Vertical_Ring_PD(OutMpu.pitch, OutMpu.gyro_x);
	//速度环PWM
	  Velocity_Pwm = Vertical_speed_PI(Encoder_left,Encoder_right,OutMpu.pitch, Movement );
	//转向环PWM
		Turn_Pwm = Vertical_turn_PD(Contrl_Turn, OutMpu.gyro_z);
	//最终左右电机的PWM
		Motor1 = Balance_Pwm + Velocity_Pwm + Turn_Pwm;
	  Motor2 = Balance_Pwm + Velocity_Pwm - Turn_Pwm;
	
		//限制速度
		PWM_Limiting(&Motor1,&Motor2);
	//5、设置电机PWM
		Set_PWM(Motor1,Motor2);
}

/**
  * @brief  打印信息到串口的进程
  * @param  None
  * @retval  None
  */
void Printf_Task(void)
{
		//PITCH（俯仰角）、ROLL（翻滚角）、YAW（航向角）
		//printf("%f,%f,%f -- \r\n",OutMpu.pitch,OutMpu.roll,OutMpu.yaw);
	
		//显示采集到的速度
		//printf("%d---- %d\r\n",Encoder_left,Encoder_right);
}


void Show_Speed(int Encoder_left,int Encoder_right)
{
	//		float pitch;    //俯仰角
	//  float roll;     //翻滚角
	// float yaw;      //偏航角
	OLED_ShowString(0,0,"pitch:");
	OLED_ShowNum(44,0,(int)OutMpu.pitch,2,16);
	OLED_ShowString(66,0,"yaw:");
	OLED_ShowNum(108,0,(int)OutMpu.yaw,2,16);
	
	
	
	
	OLED_ShowString(0,3,"left:");
	OLED_ShowNum(36,3,Encoder_left,2,16);
	OLED_ShowString(60,3,"right:");
	OLED_ShowNum(106,3,Encoder_right,2,16);
}


void Oled_Task(void)
{
		Show_Speed(Encoder_left/10,Encoder_right/10);
}

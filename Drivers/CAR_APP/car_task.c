#include "car_task.h"

int  Balance_Pwm,Velocity_Pwm,Turn_Pwm;        //PID�����PWMֵ
int  Motor1, Motor2;                  //���ҵ��PWMֵ
int  Encoder_left, Encoder_right;     //����ٶ�
float Movement = -0;                   //�ٶȵ���  
int  Contrl_Turn = 64;                //ת����ڱ���

//�������ݲɼ�����
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

//PID����
void Car_Task_100HZ(void)
{

		
		
	Encoder_left  = Read_Encoder(1);
	Encoder_right = -Read_Encoder(2);
	
	//1��ȷ��ֱ����PWM
	
		Balance_Pwm = Vertical_Ring_PD(OutMpu.pitch, OutMpu.gyro_x);
	
	//2��ȷ���ٶȻ�PWM
	
	  Velocity_Pwm = Vertical_speed_PI(Encoder_left,Encoder_right,OutMpu.pitch, Movement );
	
	
	//3��ȷ��ת��PWM
	
//		Turn_Pwm = Vertical_turn_PD(Contrl_Turn, OutMpu.gyro_z);
	
	//4��ȷ���������ҵ����PWM
		Motor1 = Balance_Pwm + Velocity_Pwm + Turn_Pwm;
	  Motor2 = Balance_Pwm + Velocity_Pwm - Turn_Pwm;
	
		PWM_Limiting(&Motor1,&Motor2);
	
	
	//5�����õ��
		Set_PWM(Motor1,Motor2);

}

void Printf_Task(void)
{
	//printf("%d %d\r\n",Encoder_left,Encoder_right);
		printf("%f,%f,%f -- \r\n",OutMpu.pitch,OutMpu.roll,OutMpu.yaw);
		//printf("%d---- %d\r\n",Encoder_left,Encoder_right);
}

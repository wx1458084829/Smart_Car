#include "car_task.h"

int  Balance_Pwm,Velocity_Pwm,Turn_Pwm;        //PID�����PWMֵ
int  Motor1, Motor2;                  //���ҵ��PWMֵ
int  Encoder_left, Encoder_right;     //����ٶ�
float Movement = 0;                   //�ٶȵ���  
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
		
	//��ȡ���ұ�������ֵ�����ٶȣ�ע��ȡ��(��Ϊ������λ���ǶԳƵ�)
	Encoder_left  = Read_Encoder(1);
	Encoder_right = -Read_Encoder(2);
	
	//ֱ����PWM
		Balance_Pwm = Vertical_Ring_PD(OutMpu.pitch, OutMpu.gyro_x);
	//�ٶȻ�PWM
	  Velocity_Pwm = Vertical_speed_PI(Encoder_left,Encoder_right,OutMpu.pitch, Movement );
	//ת��PWM
		Turn_Pwm = Vertical_turn_PD(Contrl_Turn, OutMpu.gyro_z);
	//�������ҵ����PWM
		Motor1 = Balance_Pwm + Velocity_Pwm + Turn_Pwm;
	  Motor2 = Balance_Pwm + Velocity_Pwm - Turn_Pwm;
	
		//�����ٶ�
		PWM_Limiting(&Motor1,&Motor2);
	//5�����õ��PWM
		Set_PWM(Motor1,Motor2);
}

/**
  * @brief  ��ӡ��Ϣ�����ڵĽ���
  * @param  None
  * @retval  None
  */
void Printf_Task(void)
{
		//PITCH�������ǣ���ROLL�������ǣ���YAW������ǣ�
		printf("%f,%f,%f -- \r\n",OutMpu.pitch,OutMpu.roll,OutMpu.yaw);
	
		//��ʾ�ɼ������ٶ�
		//printf("%d---- %d\r\n",Encoder_left,Encoder_right);
}

#include "mpu6050.h"


//mpu6050使用的IIC定义
IIC_TypeDef mpu6050_iic;

/**************************************************************************************************************
*函数名:MPU_Init()
*功能:初始化MPU6050设置其量程
*形参:无
*返回值:0,成功，其他,错误代码
**************************************************************************************************************/
u8 MPU_Init(void)
{ 
	//初始化结构体
  memset(&mpu6050_iic, 0, sizeof(mpu6050_iic));
	
	
	//设置端口以及引脚
	mpu6050_iic.SCL_Pin = MPU6050_SCL_Pin;
	mpu6050_iic.SCL_Port = MPU6050_SCL_GPIO_Port;
	mpu6050_iic.SDA_Pin = MPU6050_SDA_Pin;
	mpu6050_iic.SDA_Port = MPU6050_SDA_GPIO_Port;
	mpu6050_iic.SDA_Enable_Clk = SCL_ENABLE_CLK;
	mpu6050_iic.SCL_Enable_Clk = SDA_ENABLE_CLK;
	
	
	IIC_Init(&mpu6050_iic); //初始化IIC
	
	HAL_Delay(1);
	u8 res;
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
  delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
	MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
	MPU_Set_Rate(50);						//设置采样率50Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//关闭所有中断
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	MPU_Write_Byte(MPU_FIFO_EN_REG,0xFF);	//关闭FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	printf("ADD: %d\n",res);
	if(res==MPU_ADDR)//器件ID正确
	{
		printf("mpu6050_addr: %d\n",res);
//		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
		MPU_Set_Rate(50);						//设置采样率为50Hz
 	}else return 1;
	return 0;
}
/*************************************************************************************************************
*函数名:MPU_Set_Gyro_Fsr()
*功能:设置MPU6050陀螺仪传感器满量程范围
*形参:(u8 fsr):0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
*返回值:0:设置成功/其他:设置失败
*************************************************************************************************************/
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//设置陀螺仪满量程范围  
}
/************************************************************************************************************
*函数名:MPU_Set_Accel_Fsr()
*功能:设置MPU6050加速度传感器满量程范围
*形参:(u8 fsr):0,±2g;1,±4g;2,±8g;3,±16g
*返回值:0:设置成功/其他:设置失败
************************************************************************************************************/
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//设置加速度传感器满量程范围  
}
/**********************************************************************************************************
*函数名:MPU_Set_LPF()
*功能:设置MPU6050的数字低通滤波器
*形参:(u16 lpf):数字低通滤波频率(Hz)
*返回值:0:设置成功/其他:设置失败
***********************************************************************************************************/
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);//设置数字低通滤波器  
}
/**********************************************************************************************************
*函数名:MPU_Set_Rate()
*功能:设置MPU6050的采样率(假定Fs=1KHz)
*形参:(u16 rate):4~1000(Hz)
*返回值:0:设置成功/其他:设置失败
***********************************************************************************************************/
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//设置数字低通滤波器
 	return MPU_Set_LPF(rate/2);	//自动设置LPF为采样率的一半
}

/***********************************************************************************************************
*函数名:MPU_Get_Temperature()
*功能:得到温度值
*形参:无
*返回值:温度值(扩大了100倍)
***********************************************************************************************************/
short MPU_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}
/***********************************************************************************************************
*函数名:MPU_Get_Gyroscope()
*功能:得到陀螺仪值(原始值)
*形参:无
*返回值:0:设置成功/其他:设置失败
************************************************************************************************************/
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];  
		*gy=((u16)buf[2]<<8)|buf[3];  
		*gz=((u16)buf[4]<<8)|buf[5];
	} 
    return res;
	
}
/***********************************************************************************************************
*函数名:MPU_Get_Accelerometer()
*功能:得到加速度值(原始值)
*形参:无
*返回值:0:设置成功/其他:设置失败
***********************************************************************************************************/
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
/************************************************************************************************************
*函数名:MPU_Read_Len()
*功能:IIC连续读取
*形参:(u8 addr):器件地址/(u8 reg):要读取的寄存器地址/(u8 len):要读取的长度/(u8 *buf):读取数据的数据储存区
*返回值:0:成功/1：失败
*************************************************************************************************************/
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(&mpu6050_iic); 
	IIC_SendByte(&mpu6050_iic,(MPU_ADDR<<1)|0);//发送器件地址+写命令	
	if(IIC_WaitAck(&mpu6050_iic))	//等待应答
	{
		IIC_Stop(&mpu6050_iic);		 
		return 1;		
	}
    IIC_SendByte(&mpu6050_iic,reg);	//写寄存器地址
    IIC_WaitAck(&mpu6050_iic);		//等待应答
	for(i=0;i<len;i++)
	{
		IIC_SendByte(&mpu6050_iic,buf[i]);	//发送数据
		if(IIC_WaitAck(&mpu6050_iic))		//等待ACK
		{
			IIC_Stop(&mpu6050_iic);	 
			return 1;		 
		}		
	}    
    IIC_Stop(&mpu6050_iic);	 
	return 0;	
} 
/**************************************************************************************************************
*函数名:MPU_Write_Len()
*功能:IIC连续写
*形参:(u8 addr):器件地址/(u8 reg):要写入的寄存器地址/(u8 len):要写入的长度/(u8 *buf):吸入数据的数据储存区
*返回值:0:读取成功/其他:读取失败
**************************************************************************************************************/
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	IIC_Start(&mpu6050_iic); 
	IIC_SendByte(&mpu6050_iic,(MPU_ADDR<<1)|0);//发送器件地址+写命令	
	if(IIC_WaitAck(&mpu6050_iic))	//等待应答
	{
		IIC_Stop(&mpu6050_iic);		 
		return 1;		
	}
    IIC_SendByte(&mpu6050_iic,reg);	//写寄存器地址
    IIC_WaitAck(&mpu6050_iic);		//等待应答
    IIC_Start(&mpu6050_iic);
	IIC_SendByte(&mpu6050_iic,(addr<<1)|1);//发送器件地址+读命令	
    IIC_WaitAck(&mpu6050_iic);		//等待应答 
	while(len)
	{
		if(len==1)*buf=IIC_ReadByte(&mpu6050_iic,0);//读数据,发送nACK 
		else *buf=IIC_ReadByte(&mpu6050_iic,1);		//读数据,发送ACK  
		len--;
		buf++; 
	}    
    IIC_Stop(&mpu6050_iic);	//产生一个停止条件 
	return 0;	
}
//IIC写一个字节 
//reg:寄存器地址
//data:数据
//返回值:0,正常
//    其他,错误代码
u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{ 
    IIC_Start(&mpu6050_iic); 
	IIC_SendByte(&mpu6050_iic,(MPU_ADDR<<1)|0);//发送器件地址+写命令	
	if(IIC_WaitAck(&mpu6050_iic))	//等待应答
	{
		IIC_Stop(&mpu6050_iic);		 
		return 1;		
	}
    IIC_SendByte(&mpu6050_iic,reg);	//写寄存器地址
    IIC_WaitAck(&mpu6050_iic);		//等待应答 
	IIC_SendByte(&mpu6050_iic,data);//发送数据
	if(IIC_WaitAck(&mpu6050_iic))	//等待ACK
	{
		IIC_Stop(&mpu6050_iic);	 
		return 1;		 
	}		 
    IIC_Stop(&mpu6050_iic);	 
	return 0;
}
/*************************************************************************************************************
*函数名:MPU_Write_Byte()
*功能:MPU6050读一个字节
*形参:(u8 reg):寄存器地址/(u8 data):数据
*返回值:
**************************************************************************************************************/
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
    IIC_Start(&mpu6050_iic); 
	IIC_SendByte(&mpu6050_iic,(MPU_ADDR<<1)|0);//发送器件地址+写命令	
	IIC_WaitAck(&mpu6050_iic);		//等待应答 
    IIC_SendByte(&mpu6050_iic,reg);	//写寄存器地址
    IIC_WaitAck(&mpu6050_iic);		//等待应答
    IIC_Start(&mpu6050_iic);
	IIC_SendByte(&mpu6050_iic,(MPU_ADDR<<1)|1);//发送器件地址+读命令	
    IIC_WaitAck(&mpu6050_iic);		//等待应答 
	res=IIC_ReadByte(&mpu6050_iic,0);//读取数据,发送nACK 
    IIC_Stop(&mpu6050_iic);			//产生一个停止条件 
	return res;		
}



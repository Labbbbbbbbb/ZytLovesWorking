#include "balance.h"

//****************平衡小车机械零点***************
float Car_zero = 0.0f;    
//直立环 
float zhili_Kp=95.5f*0.6f;  //出现大幅度低频振荡 95.5f
float zhili_Kd=-10.0f*0.6f;    //出现小幅度高频振荡  *0.6f
int zhili_out=0;       //直立环输出

//***********平衡车控制******************************************
//函数功能：控制小车保持直立
//Angle：采集到的实际角度值  angle.x
//Gyro： 采集到的实际角速度值  gyroscope.x
int zhili(float Angle,float Gyro)
{  
   float err;
	 int pwm_zhili;
	 err=Car_zero-Angle;    //期望值-实际值，这里期望小车平衡，因此期望值就是机械中值       
	 pwm_zhili=zhili_Kp*err+Gyro*zhili_Kd;//计算平衡控制的电机PWM
	 return pwm_zhili;
}


float speed_err_max=100.0f;          //速度误差限幅
float speed_integral_max=800.0f;    //速度积分限幅
float speed_ctrl_output_max=1000.0f;  //速度控制输出限幅
float speed_kp=5.0f;                 //速度环比例系数
float speed_ki=0.02f;                //速度环积分系数   
float v_target_l=0.0f;               //左轮目标速度 cm/s
float v_target_r=0.0f;               //右轮目标速度 cm/s    
float speed_feedback[2];            //速度反馈值
float speed_error[2];               //速度误差  
float speed_integral[2];           //速度积分
float speed_output[2];            //速度控制输出
//*************************************************************
//函数功能：速度环控制函数
//100hz调用一次
void speed_control_100hz(void)
{
	//平衡车直立环上的速度环，为与直立环处理速度一致，把下面两行代码注释掉
//	static uint16_t _cnt=0;
//	_cnt++;	if(_cnt<2)	return;	_cnt=0;//10ms控制一次 100hz是0.005s，两个100hz就是10ms

	speed_feedback[0]=smartcar_imu.left_motor_speed_cmps;//获取左轮实际值  
	speed_error[0]=v_target_l-speed_feedback[0];
	speed_error[0]=constrain_float(speed_error[0],-speed_err_max,speed_err_max);
	speed_integral[0]+=speed_ki*speed_error[0];
	speed_integral[0]=constrain_float(speed_integral[0],-speed_integral_max,speed_integral_max);
	speed_output[0]=speed_integral[0]+speed_kp*speed_error[0];
	speed_output[0]=constrain_float(speed_output[0],-speed_ctrl_output_max,speed_ctrl_output_max);
//	UART_printf(UART0_BASE,"%d\n",(int)smartcar_imu.left_motor_speed_cmps);
		
	speed_feedback[1]=smartcar_imu.right_motor_speed_cmps;//右轮
	speed_error[1]=v_target_r-speed_feedback[1];//期望速度减去实际速度得到速度误差
	speed_error[1]=constrain_float(speed_error[1],-speed_err_max,speed_err_max);//对速度误差做约束
	speed_integral[1]+=speed_ki*speed_error[1]; //速度积分
	speed_integral[1]=constrain_float(speed_integral[1],-speed_integral_max,speed_integral_max);//对得到的速度积分做约束
	speed_output[1]=speed_integral[1]+speed_kp*speed_error[1];//pid得到输出
	speed_output[1]=constrain_float(speed_output[1],-speed_ctrl_output_max,speed_ctrl_output_max);//对输出做约束
	//UART_printf(UART0_BASE,"%d\n",(int)smartcar_imu.right_motor_speed_cmps);
}
//转向环
float zhuan_Kp=0.0f;   //期望小车转向，正反馈
float zhuan_Kd=-10.0f;    //抑制小车转向，负反馈

//*************************************************************
//函数功能：控制小车转向
//Set_turn：目标旋转角速度
//Gyro_Z:陀螺仪Z轴的角速度
//不是一个严格的PD控制器，为小车的叠加控制
int zhuan(float Set_turn,float Gyro_Z)
{
  int PWM_Out=0; 
	if(Set_turn==0)
	{
	 PWM_Out=zhuan_Kd*Gyro_Z; //没有转向需求，Kd约束小车转向
	}
	if(Set_turn!=0)
	{
	 PWM_Out=zhuan_Kp*Set_turn; //有转向需求，Kp为期望小车转向 
	}
	return PWM_Out;
}
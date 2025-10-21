#include "control.h"
#include "tim.h"
#include "math.h"
PID_t left_pid,right_pid;
PID_t angle_pid;
PID_t gyro_pid;
PID_t turn_pid;


int16_t left_cnt;  
int16_t right_cnt;
float vel_left;
float vel_right;
float outputright,outputleft;

float left_velocity,right_velocity;

void Control_Peripheral_init(void)
{
  JY901S_Init();
  /*Motor*/
  HAL_TIM_Base_Start(&htim1); //Update_Timer
  HAL_TIM_Base_Start(&htim2); //PWM_Timer
  HAL_TIM_Base_Start(&htim3); //Left_Encoder_Timer
  HAL_TIM_Base_Start(&htim4); //Right_Encoder_Timer
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1 | TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1 | TIM_CHANNEL_2);
  __HAL_TIM_SetCounter(&htim3,65536/2);
  __HAL_TIM_SetCounter(&htim4,65536/2);//16bit ARR

  /*Servo*/
  /*
  *Servo PWM 100Hz , 32bit ARR=100000 , CNT:1us  
  *CCR:500~2500 对应 0~270度
  TIM_CHANNEL1->4分别对应左上，左下，右上，右下舵机
  */
  HAL_TIM_Base_Start(&htim5);   
  HAL_TIM_Base_Start(&htim9);   
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);

}

void Control_param_init()
{
    pid_init(&left_pid, 40, 4, 0.5, 1000, 0,200);
    pid_init(&right_pid,40, 4, 0.5, 1000, 0,200);
    pid_init(&angle_pid,8, 0, 0.2, 160, 0,20);
    pid_init(&gyro_pid,0.3, 0.2, 0.0, 40, 0,8);
    pid_init(&turn_pid,0.3, 0.1, 0.1, 10, 0,4);
    IK_Param_Init();

}


void Angle_Control_Loop()
{
    // JY901S_Update();
    /***********ANGLE&GYRO_PID_CONTROL************/
    angle_pid.ref=0;
    angle_pid.fdb=fAngle[0];
    PID_Calc_P(&angle_pid);
   if(fAngle[0]>-4&&fAngle[0]<4)  //dead band
   {
       angle_pid.output=0;
   }
   gyro_pid.ref=angle_pid.output;   //Gyro_Loop
   gyro_pid.fdb=fGyro[0];
   PID_Calc_P(&gyro_pid);
  /***********ANGLE&GYRO_PID_CONTROL************/
}

//车身前进速度forward，车身旋转速度turn，含转向环,forward没有独立的反馈，到时应该是靠手操
//forward的单位是轮子的rpm
void Velocity_Control_Loop(float forward,float turn)
{
    turn_pid.ref=turn;
    turn_pid.fdb=fGyro[2];  //Z轴陀螺仪
    PID_Calc_P(&turn_pid);
    left_velocity=forward-turn_pid.output;
    right_velocity=forward+turn_pid.output;
}

void Wheel_Control_Loop()
{
    /*Get_Feedback*/
      left_cnt=__HAL_TIM_GET_COUNTER(&htim3)-65536/2;
      right_cnt=__HAL_TIM_GET_COUNTER(&htim4)-65536/2;
      vel_left=left_cnt;  //rpm/28.0)*60.0*100
      vel_right=right_cnt; //rpm/28.0)*60.0*100
      left_cnt=0;
      right_cnt=0;
    /*Get_Feedback*/

    /***********PID_CONTROL************/
      left_pid.ref=gyro_pid.output+left_velocity;   //直立环的输出叠加车身的速度
      right_pid.ref=gyro_pid.output+right_velocity;
      left_pid.fdb=vel_left;
      right_pid.fdb=vel_right;
      PID_Calc_P(&left_pid);
      PID_Calc_P(&right_pid);
      if(left_pid.output>0)
      {
        HAL_GPIO_WritePin(Left_IN1_GPIO_Port, Left_IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Left_IN2_GPIO_Port, Left_IN2_Pin, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, left_pid.output);
      }else
      {
        HAL_GPIO_WritePin(Left_IN1_GPIO_Port, Left_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Left_IN2_GPIO_Port, Left_IN2_Pin, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1000+left_pid.output);
      }
      if(right_pid.output>0)
      {
        HAL_GPIO_WritePin(Right_IN1_GPIO_Port, Right_IN1_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(Right_IN2_GPIO_Port, Right_IN2_Pin, GPIO_PIN_SET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, right_pid.output);
      }else
      {
        HAL_GPIO_WritePin(Right_IN1_GPIO_Port, Right_IN1_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(Right_IN2_GPIO_Port, Right_IN2_Pin, GPIO_PIN_RESET);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 1000+right_pid.output);
      }

      __HAL_TIM_SetCounter(&htim3,65536/2);
      __HAL_TIM_SetCounter(&htim4,65536/2);

    /***********PID_CONTROL************/

}


//debug message
// printf("angle:%.2f,gyro:%.2f\n",fAngle[0],fGyro[0]);
// printf("lcnt:%f,rcnt:%f,angle_pid:%.2f,gyro_pid:%.2f,left_pid:%.2f,right_pid:%.2f\n",vel_left,vel_right,angle_pid.output,gyro_pid.output,left_pid.output,right_pid.output);
// printf("%f,%f,%f\n",fAngle[0],vel_left,gyro_pid.output);
// printf("%f,%f,%f,%f,%f\n",fAngle[0],fGyro[0],angle_pid.output,vel_left,gyro_pid.output);

float L1,L2,L3,L4,L5;
//float alphaLeftToAngle,betaLeftToAngle,alphaRightToAngle,betaRightToAngle;

IKparam IKParam;
#define PI 3.14159265358979323846
void IK_Param_Init(void)
{
    L1=60;
    L2=100; 
    L3=100;
    L4=60;
    L5=30;
}

void Servo_IK_Control(float height)
{
    IKParam.XLeft=0;
    IKParam.YLeft=height;
    IKParam.XRight=0;
    IKParam.YRight=height;

  float alpha1,alpha2,beta1,beta2;
  __IO uint16_t servoLeftFront,servoLeftRear,servoRightFront,servoRightRear;

  float aLeft = 2 * IKParam.XLeft * L1;
  float bLeft = 2 * IKParam.YLeft * L1;
  float cLeft = IKParam.XLeft * IKParam.XLeft + IKParam.YLeft * IKParam.YLeft + L1 * L1 - L2 * L2;
  float dLeft = 2 * L4 * (IKParam.XLeft - L5);
  float eLeft = 2 * L4 * IKParam.YLeft;
  float fLeft = ((IKParam.XLeft - L5) * (IKParam.XLeft - L5) + L4 * L4 + IKParam.YLeft * IKParam.YLeft - L3 * L3);

  alpha1 = 2 * atan((bLeft + sqrt((aLeft * aLeft) + (bLeft * bLeft) - (cLeft * cLeft))) / (aLeft + cLeft));
  alpha2 = 2 * atan((bLeft - sqrt((aLeft * aLeft) + (bLeft * bLeft) - (cLeft * cLeft))) / (aLeft + cLeft));
  beta1 = 2 * atan((eLeft + sqrt((dLeft * dLeft) + eLeft * eLeft - (fLeft * fLeft))) / (dLeft + fLeft));
  beta2 = 2 * atan((eLeft - sqrt((dLeft * dLeft) + eLeft * eLeft - (fLeft * fLeft))) / (dLeft + fLeft));

  alpha1 = (alpha1 >= 0)?alpha1:(alpha1 + 2 * PI);
  alpha2 = (alpha2 >= 0)?alpha2:(alpha2 + 2 * PI);

  if(alpha1 >= PI/2) IKParam.alphaLeft = alpha1;        //会因舵机的不同而不同
  else IKParam.alphaLeft = alpha2;
  if(beta1 >= 0 && beta1 <= PI/2) IKParam.betaLeft = beta1;
  else IKParam.betaLeft = beta2;
  
  float aRight = 2 * IKParam.XRight * L1;
  float bRight = 2 * IKParam.YRight * L1;
  float cRight = IKParam.XRight * IKParam.XRight + IKParam.YRight * IKParam.YRight + L1 * L1 - L2 * L2;
  float dRight = 2 * L4 * (IKParam.XRight - L5);
  float eRight = 2 * L4 * IKParam.YRight;
  float fRight = ((IKParam.XRight - L5) * (IKParam.XRight - L5) + L4 * L4 + IKParam.YRight * IKParam.YRight - L3 * L3);

  IKParam.alphaRight = 2 * atan((bRight + sqrt((aRight * aRight) + (bRight * bRight) - (cRight * cRight))) / (aRight + cRight));
  IKParam.betaRight = 2 * atan((eRight - sqrt((dRight * dRight) + eRight * eRight - (fRight * fRight))) / (dRight + fRight));

  alpha1 = 2 * atan((bRight + sqrt((aRight * aRight) + (bRight * bRight) - (cRight * cRight))) / (aRight + cRight));
  alpha2 = 2 * atan((bRight - sqrt((aRight * aRight) + (bRight * bRight) - (cRight * cRight))) / (aRight + cRight));
  beta1 = 2 * atan((eRight + sqrt((dRight * dRight) + eRight * eRight - (fRight * fRight))) / (dRight + fRight));
  beta2 = 2 * atan((eRight - sqrt((dRight * dRight) + eRight * eRight - (fRight * fRight))) / (dRight + fRight));

  alpha1 = (alpha1 >= 0)?alpha1:(alpha1 + 2 * PI);
  alpha2 = (alpha2 >= 0)?alpha2:(alpha2 + 2 * PI);

  if(alpha1 >= PI/2) IKParam.alphaRight = alpha1;   //会因舵机的不同而不同
  else IKParam.alphaRight = alpha2;
  if(beta1 >= 0 && beta1 <= PI/2) IKParam.betaRight = beta1;
  else IKParam.betaRight = beta2;

  __IO int alphaLeftToAngle = (int)((IKParam.alphaLeft / 6.28) * 360);//弧度转角度
  __IO int betaLeftToAngle = (int)((IKParam.betaLeft / 6.28) * 360);

  __IO int alphaRightToAngle = (int)((IKParam.alphaRight / 6.28) * 360);
  __IO int betaRightToAngle = (int)((IKParam.betaRight / 6.28) * 360);

  // servoLeftFront = 90 + betaLeftToAngle;
  // servoLeftRear = 90 + alphaLeftToAngle;
  // servoRightFront = 270 - betaRightToAngle;
  // servoRightRear = 270 - alphaRightToAngle;

  servoLeftFront =   betaLeftToAngle;
  servoLeftRear =   alphaLeftToAngle;
  servoRightFront = 180 - betaRightToAngle;
  servoRightRear = 180 - alphaRightToAngle;
   int ch1=(int)(servoLeftFront/300.0*2000+500);        //待验证
   int ch2=(int)(servoLeftRear/300.0*2000+500);
   int ch3=(int)(servoRightFront/300.0*2000+500);
   int ch4=(int)(servoRightRear/300.0*2000+500);
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, ch1);
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, ch2);
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, ch3);
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_4, ch4);
}
//安装方法：
//先运行setcompare(500），然后装四个单向舵盘朝前方
//调好L1~L5的尺寸参数后，,给出一个合理的高度值height（单位mm），合理指它满足机械结构的要求，然后运行Servo_IK_Control(height);
//舵机会转到对应的位置，此时左边舵盘所指方向即舵机在该高度会转过的alpha和beta角度，右边则是舵盘的反向延长线为响应角度
//此时机械顺着这个角度安装腿部即可

//https://gitee.com/StackForce/bipedal_wheeled_robot/blob/master/%E8%AF%BE%E7%A8%8B%E4%BB%A3%E7%A0%81/%E7%AC%AC%E5%85%AD%E8%AF%BE_%E8%BF%90%E5%8A%A8%E5%AD%A6%E9%80%86%E8%A7%A3/lesson6_HeightCtrl/src/main.cpp
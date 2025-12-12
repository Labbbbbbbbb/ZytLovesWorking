#include "control.h"
#include "tim.h"
#include "math.h"
#include "bldc.h"
PID_t left_pid,right_pid;
PID_t angle_pid;
PID_t gyro_pid;
PID_t turn_pid;
PID_t roll_pid;


int16_t left_cnt;  
int16_t right_cnt;
__IO int16_t vel_left;
__IO int16_t vel_right;
float outputright,outputleft;

float left_velocity,right_velocity;

void Control_Peripheral_init(void)
{
  JY901S_Init();
  /*Motor*/
  HAL_TIM_Base_Start_IT(&htim1); //Update_Timer
  HAL_TIM_Base_Start(&htim2); //PWM_Timer
  HAL_TIM_Base_Start(&htim3); //Left_Encoder_Timer
  HAL_TIM_Base_Start(&htim4); //Right_Encoder_Timer
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(&htim3,TIM_CHANNEL_1 | TIM_CHANNEL_2);
  HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1 | TIM_CHANNEL_2);
  __HAL_TIM_SetCounter(&htim3,65536/2);
  __HAL_TIM_SetCounter(&htim4,65536/2);//16bit ARR
  HAL_UART_Receive_IT(BLDC_UART, (uint8_t *)bldc_rxdata, 1);  //BLDC Decoder UART Receive IT
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
    // pid_init(&left_pid, 4, 1, 0.5, 100, 0,20);
    // pid_init(&right_pid,4, 1, 0.5, 100, 0,20);
    // pid_init(&angle_pid,1.5, 0, 0.1 , 40, 0,15);
    // pid_init(&gyro_pid,0.5, 0.2 , 0.0, 55, 0,20);
    // pid_init(&turn_pid,0.3, 0.1, 0.1, 10, 0,4);

    //with higher current level
    pid_init(&left_pid, 5, 1, 0.5, 100, 0,20);    //
    pid_init(&right_pid,5, 1, 0.5, 100, 0,20);    
    pid_init(&angle_pid,40, 2.5, 3.0, 1000, 0,15);     //66 3.6 10  1000增量式 offset=25
    pid_init(&gyro_pid,0.01, 0.03 , 0.03, 65, 0,20);  //0.01 0.03 0.03 位置式 offset=25
    pid_init(&turn_pid,0.3, 0.1, 0.1, 10, 0,4);
    pid_init(&roll_pid,2, 1, 0.1, 100, 0,50);
    IK_Param_Init();

    /*
    问题：我的d环怎么死掉了  必须得加上i才能用 但是这样会有积分累积现象且容易疯转
    而且问题是它纯靠p的话总开始有一点点倾斜并且程度会积累直到翻车，但是p再增大的话又会震荡
    而且d又不好用得离谱  可以再去看一下b站那两个调参视频
    现在打算把offset=55的30作为基准来调pid  再看需不需要随着高度改变的pid
    */

}


void Angle_Control_Loop()
{
    // JY901S_Update();
    /***********ANGLE&GYRO_PID_CONTROL************/
    angle_pid.ref=0;
    angle_pid.fdb=fAngle[0];
    PID_Calc(&angle_pid);
   //if(fAngle[0]>-1&&fAngle[0]<1)  //dead band
   //{
    //   angle_pid.output=0;
   //}
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
    left_velocity=forward;//+turn_pid.output;
    right_velocity=forward;//-turn_pid.output;
}

  uint8_t  bufferl[10] ;
  uint8_t  bufferr[10] ;

void Wheel_Control_Loop()
{
    
      // vel_left=(int16_t)(gyro_pid.output+left_velocity);   //直立环的输出叠加车身的速度
      // vel_right=(int16_t)(gyro_pid.output+right_velocity);
      vel_left=(int16_t)(gyro_pid.output);   //直立环的输出叠加车身的速度
      vel_right=(int16_t)(gyro_pid.output);
      uint32_t lenth_l=sprintf(bufferl,"B%d\n",-vel_left);
      HAL_UART_Transmit(BLDC_UART, (uint8_t *)bufferl, lenth_l, 100);

      uint32_t lenth_r=sprintf(bufferr,"A%d\n",vel_right);
      HAL_UART_Transmit(BLDC_UART, (uint8_t *)bufferr, lenth_r, 100);
      //HAL_Delay(8);
      //printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",(float)vel_left,(float)vel_right,angle_pid.output,fGyro[0],gyro_pid.output, bldc_msg[0], bldc_msg[2]);
      printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",(float)vel_left,(float)fAngle[0],angle_pid.output,fGyro[0],-bldc_msg[1],bldc_msg[3], bldc_msg[0], bldc_msg[2]);

    /***********PID_CONTROL************/

}


//debug message
// printf("angle:%.2f,gyro:%.2f\n",fAngle[0],fGyro[0]);
// printf("lcnt:%f,rcnt:%f,angle_pid:%.2f,gyro_pid:%.2f,left_pid:%.2f,right_pid:%.2f\n",vel_left,vel_right,angle_pid.output,gyro_pid.output,left_pid.output,right_pid.output);
// printf("%f,%f,%f\n",fAngle[0],vel_left,gyro_pid.output);
// printf("%f,%f,%f,%f,%f\n",fAngle[0],fGyro[0],angle_pid.output,vel_left,gyro_pid.output);

__IO uint16_t L1,L2,L3,L4,L5;
//float alphaLeftToAngle,betaLeftToAngle,alphaRightToAngle,betaRightToAngle;

IKparam IKParam;
#define PI 3.14159265358979323846
void IK_Param_Init(void)
{
    L1=65;
    L2=75; 
    L3=75;
    L4=65;
    L5=45;
}

void Servo_IK_Control(uint8_t index,float height)
{
    IKParam.XLeft=L5/2;//30
    IKParam.YLeft=height;
    IKParam.XRight=L5/2;
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
  uint16_t offset=35;   //没招了 不知道为什么y给到30以下反而会站的更高了，于是只能最低给到30，加个offset让它蹲低一点 offset=55可以折叠到最矮
  uint16_t offset0=90;   //加这个是因为为防止初始角度小于脉宽500所在的位置把零点前移了九十度
  servoLeftFront =   betaLeftToAngle-offset+offset0;
  servoLeftRear =   alphaLeftToAngle+offset+offset0;
  servoRightFront = 180 - betaRightToAngle+offset+offset0;
  servoRightRear = 180 - alphaRightToAngle-offset+offset0;
   int ch1=(int)(servoLeftFront/300.0*2000+500);        //待验证
   int ch2=(int)(servoLeftRear/300.0*2000+500);
   int ch3=(int)(servoRightFront/300.0*2000+500);
   int ch4=(int)(servoRightRear/300.0*2000+500);
   if(ch1>=500&&ch1<=2500&&ch2>=500&&ch2<=2500&&ch3>=500&&ch3<=2500&&ch4>=500&&ch4<=2500){
    if(index==LEFT)
    {
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, ch1);
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, ch2);
    }
    else if(index==RIGHT)
    {
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, ch3);
    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_4, ch4);
   }
  }
}
//安装方法：
//先运行setcompare(500），然后装四个单向舵盘朝前方
//调好L1~L5的尺寸参数后，,给出一个合理的高度值height（单位mm），合理指它满足机械结构的要求，然后运行Servo_IK_Control(height);
//舵机会转到对应的位置，此时左边舵盘所指方向即舵机在该高度会转过的alpha和beta角度，右边则是舵盘的反向延长线为响应角度
//此时机械顺着这个角度安装腿部即可

//https://gitee.com/StackForce/bipedal_wheeled_robot/blob/master/%E8%AF%BE%E7%A8%8B%E4%BB%A3%E7%A0%81/%E7%AC%AC%E5%85%AD%E8%AF%BE_%E8%BF%90%E5%8A%A8%E5%AD%A6%E9%80%86%E8%A7%A3/lesson6_HeightCtrl/src/main.cpp

void Roll_Control_Loop()
{
    // float roll=fAngle[1]*2.0f;
    // if(roll<100&&roll>=0)
    // {
    //  Servo_IK_Control(LEFT,30.0f); //范围30-130
    //   Servo_IK_Control(RIGHT,30.0f+roll); //范围30-130
    // }else if(roll<0&&roll>=-100)
    // {
    //  Servo_IK_Control(RIGHT,30.0f); //范围30-130
    //   Servo_IK_Control(LEFT,30.0f+roll); //范围30-130
    // }
    // else if (roll>=100)
    // {
    //  Servo_IK_Control(RIGHT,130.0f); //范围30-130
    //  Servo_IK_Control(LEFT,30.0f); //范围30-130
    // }
    // else if (roll<-100)
    // {
    //  Servo_IK_Control(LEFT,130.0f); //范围30-130
    //  Servo_IK_Control(RIGHT,30.0f); //范围30-130
    // }
    roll_pid.ref=0;
    roll_pid.fdb=fAngle[1];
    PID_Calc_P(&roll_pid);
    if(roll_pid.output>0)
    {
     Servo_IK_Control(LEFT,30.0f+roll_pid.output); //范围30-130
     Servo_IK_Control(RIGHT,30.0f); //范围30-130
    }else
    {
      Servo_IK_Control(RIGHT,30.0f-roll_pid.output); //范围30-130
      Servo_IK_Control(LEFT,30.0f); //范围30-130
      }
}
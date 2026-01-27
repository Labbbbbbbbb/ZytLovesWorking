#include "control.h"
#include "tim.h"
#include "math.h"
#include "bldc.h"
PID_t left_pid,right_pid;
PID_t angle_pid;
PID_t gyro_pid;
PID_t turn_pid;
PID_t roll_pid;
PID_t vel_pid;

static float base_angle_kp;
static float base_angle_ki;
static float base_angle_kd;

static float base_gyro_kp;
static float base_gyro_ki;
static float base_gyro_kd;

static float base_vel_kp;
static float base_vel_ki;
static float base_vel_kd;

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
    base_angle_kp=2.0588;//2.0588  3.5
    base_angle_ki=0.1157;//130/1000/60
    base_angle_kd=8.1764;//3

    base_gyro_kp=0.12;
    base_gyro_ki=0.09;
    base_gyro_kd=0.001;//1

    base_vel_kp=1;
    base_vel_ki=0.0;
    base_vel_kd=0.03;//3

    //pid_init(&vel_pid,base_vel_kp, base_vel_ki, base_vel_kd, 6, 0, 2);  //
    pid_init(&angle_pid,base_angle_kp, base_angle_ki, base_angle_kd, 120, 0, 90);     //
    //pid_init(&gyro_pid,base_gyro_kp, base_gyro_ki , base_gyro_kd, 120, 0, 45);  //


    //with higher current level
    // pid_init(&left_pid, 0.01, 0.0, 0.15, 1, 0,0.5);    //嗯似乎重新装了一遍机械后左右反过来了
    // pid_init(&right_pid,0.01, 0.0, 0.15, 1, 0,0.5);    
    // pid_init(&angle_pid,38+24.0/100*height_ref, 1.5, 2, 1500, 0,15);     //35+20.0/100*height_ref, 1.5, 2, 1500, 0,15
    // pid_init(&angle_pid,55, 2, 3, 1500, 0,15);     //35, 2.5, 2.5, 1000, 0,15增量式 offset=30
    // pid_init(&gyro_pid,0.06, 0.04 , 0.03, 65, 0,20);  //0.05, 0.04 , 0.03, 65, 0,20 位置式 offset=30
    // pid_init(&turn_pid,0.35, 0.1, 0.1, 10, 0,4);
    // pid_init(&roll_pid,2, 1, 0.1, 100, 0,50);
    IK_Param_Init();

}


void Angle_Control_Loop(float ref)
{
    
    //pid_init(&angle_pid,35+20.0/100*height_ref, 2, 3, 1500, 0,15);     //35, 2.5, 2.5, 1000, 0,15增量式 offset=30
  
    //angle_pid.KP=38+24.0/100*height_ref;
    //angle_pid.KI=1.5+1.0/100*height_ref;
    //   if(fabs(fAngle[0])>20+(100.0-height_ref)/20)  //防止翻车后pid风车
    // {
    //     angle_pid.KP=110;
    //     angle_pid.KI=0.0;

       
    // }else
    // {
    //     angle_pid.KP=120;
    //     angle_pid.KI=0.0;

    // }
    /***********ANGLE&GYRO_PID_CONTROL************/ 
    angle_pid.ref=2.6; //+forward
    angle_pid.fdb=fAngle[0]; //角度环反馈带死区n    tan((-(1+ref)+fAngle[0])/180*3.14)*180/3.14*(fmax(fabs((1+ref-fAngle[0])),1.0f))    
    PID_Calc_P(&angle_pid);

   /*Angle -- fuzzy pid control*/
    // double delta_Kp, delta_Ki, delta_Kd;
    // fuzzy_inference(angle_pid.cur_error, angle_pid.cur_error - angle_pid.error[1], &delta_Kp, &delta_Ki, &delta_Kd);
    // angle_pid.KP = base_angle_kp + delta_Kp;
    // angle_pid.KI = base_angle_ki + delta_Ki;
    // angle_pid.KD = base_angle_kd + delta_Kd;
    // fuzzy_pid_update(&angle_pid, angle_pid.cur_error, 1.0f/1000*(tim_mark[0]-tim_mark[1])); 
   /*Angle -- fuzzy pid control*/

    // gyro_pid.ref=angle_pid.output;   //Gyro_Loop
    // gyro_pid.fdb=fGyro[0];
    // PID_Calc_P(&gyro_pid);
   /*Gyro -- fuzzy pid control*/
    // fuzzy_inference(gyro_pid.cur_error, gyro_pid.cur_error - gyro_pid.error[1], &delta_Kp, &delta_Ki, &delta_Kd);
    // gyro_pid.KP = base_gyro_kp + delta_Kp;
    // gyro_pid.KI = base_gyro_ki + delta_Ki;
    // gyro_pid.KD = base_gyro_kd + delta_Kd;
    // fuzzy_pid_update(&gyro_pid, gyro_pid.cur_error, 1.0f/1000*(tim_mark[0]-tim_mark[1])); 
   /*Gyro -- fuzzy pid control*/

  /***********ANGLE&GYRO_PID_CONTROL************/
}

//车身前进速度forward，车身旋转速度turn，含转向环,forward没有独立的反馈，到时应该是靠手操
//forward的范围
void Velocity_Control_Loop(float forward,float turn)
{
    vel_pid.ref=forward;
    vel_pid.fdb=1.0f*(-bldc_msg[3]+bldc_msg[1])/2.0;
    PID_Calc_P(&vel_pid);
    left_velocity=angle_pid.output - turn + vel_pid.output; //注意！这里两个速度的量的含义是目标电流的一百倍
    right_velocity=angle_pid.output + turn + vel_pid.output;

}

  uint8_t  bufferl[16] ;
  uint8_t  bufferr[16] ;

void Wheel_Control_Loop()
{
    
      // vel_left=(int16_t)(gyro_pid.output+left_velocity);   //直立环的输出叠加车身的速度
      // vel_right=(int16_t)(gyro_pid.output+right_velocity);

      // left_pid.ref=-vel_left;
      // left_pid.fdb=bldc_msg[1];   //BLDC Decoder Speed Feedback
      // PID_Calc_P(&left_pid);
      // right_pid.ref=vel_right;
      // right_pid.fdb=bldc_msg[3]; //BLDC Decoder Speed Feedback
      // PID_Calc_P(&right_pid);
      

      uint32_t lenth_l=sprintf(bufferl,"B%d\n",(int16_t)((-left_velocity))); //注意这里的含义是电流的一百倍  电机驱动那里除了一百
      HAL_UART_Transmit(BLDC_UART, (uint8_t *)bufferl, lenth_l, 100);

      uint32_t lenth_r=sprintf(bufferr,"A%d\n", (int16_t)((right_velocity)));//right_pid.output-0.35*tan(fAngle[0]/180*3.14)-0.008*fAngle[0]
      HAL_UART_Transmit(BLDC_UART, (uint8_t *)bufferr, lenth_r, 100);
      //HAL_Delay(8);
      printf("%.2f,%.2f,%.2f,%.2f,%f,%f\n",(float)vel_pid.output,(float)angle_pid.output, fGyro[0], gyro_pid.output, angle_pid.KP,fAngle[0]);
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
    // IKParam.XLeft=L5/2+0.3*fAngle[0];//L5/2
    // IKParam.YLeft=height;
    // IKParam.XRight=L5/2+0.3*fAngle[0];
    // IKParam.YRight=height;
    // if(IKParam.XLeft<-10) IKParam.XLeft=-10;
    // if(IKParam.XRight<-10) IKParam.XRight=-10;  
    // if (IKParam.XLeft>L5) IKParam.XLeft=L5;
    // if (IKParam.XRight>L5) IKParam.XRight=L5;
    IKParam.XLeft=L5/2-4;//L5/2
    IKParam.YLeft=height;
    IKParam.XRight=L5/2;
    IKParam.YRight=height+8;
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
  uint16_t offset=10;   //没招了 不知道为什么y给到30以下反而会站的更高了，于是只能最低给到30，加个offset让它蹲低一点 offset=55可以折叠到最矮
  //uint16_t offset0=90;   //加这个是因为为防止初始角度小于脉宽500所在的位置把零点前移了九十度
  servoLeftFront =   betaLeftToAngle-offset;//180°舵机，不加90°
  servoLeftRear =   alphaLeftToAngle+offset;
  servoRightFront = 270 - betaRightToAngle+offset;//300°舵机
  servoRightRear = 180 - alphaRightToAngle-offset;
//   servoLeftFront =   0;
//   servoLeftRear =   0;
//   servoRightFront = 0;
//   servoRightRear = 0;
   int ch1=(int)(servoLeftFront/180.0*2000+500);        //待验证
   int ch2=(int)(servoLeftRear/180.0*2000+500);
   int ch3=(int)(servoRightFront/300.0*2000+500);
   int ch4=(int)(servoRightRear/180.0*2000+500);
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
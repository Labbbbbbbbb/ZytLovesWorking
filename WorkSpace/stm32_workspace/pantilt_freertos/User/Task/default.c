#include "head.h"

#define DISTANCE 1000 //云台到白板的距离 单位mm
#define PI 3.141592
<<<<<<< HEAD
#define ANGLE_MAXIMUM 800  //限定舵机能往一边转的角度（4095为一圈
=======
#define ANGLE_MAXIMUM 1000  //限定舵机能往一边转的角度（4095为一圈
>>>>>>> origin/zyt1

/*题号*/
uint8_t Q_NO;

/***target from openmv***/
#ifdef GREEN
uint8_t ReceivebufferG[6];
#endif
#ifdef RED
uint8_t ReceivebufferR[14];
uint8_t dot1x,dot1y,dot2x,dot2y,dot3x,dot3y,dot4x,dot4y;
int16_t tar_pos1,tar_pos2;
#endif
uint8_t red_x,red_y;
/***Feedback Data***/
int16_t fdb_pos1;
int16_t fdb_pos2;

int16_t ori_pos1;
int16_t ori_pos2;

uint16_t flag;
/***Feedback Data***/

/*spe_*/
int16_t spe1;
int16_t spe2;

uint8_t mode1=0; //电机模式
uint8_t mode2=0; //电机模式
uint8_t ERROR_FLAG;

void StartDefaultTask(void *argument)
{
    #ifdef GREEN
    HAL_UART_Receive_IT(&huart2,ReceivebufferG,sizeof(ReceivebufferG));
    #endif 
    #ifdef RED
    HAL_UART_Receive_IT(&huart2,ReceivebufferR,sizeof(ReceivebufferR));
    #endif
    
    WheelMode(1,0); //模式0：位置模式 模式1：恒速； 模式2：pwm调速；模式3：步进
    WheelMode(2,0); //模式0：位置模式 模式1：恒速； 模式2：pwm调速；模式3：步进
    WriteSpe(1,0,0);
    WriteSpe(2,0,0);
    Q_NO=5;
    Servo_Start();
    Statemachine_Start();
    


    for (;;) {
    fdb_pos1=ReadPos(1);
    fdb_pos2=ReadPos(2);
        //反馈数据
    if(flag==0){    //只运行一次，记录初始角度数据（一般为2048上下
        ori_pos1=fdb_pos1;
        ori_pos2=fdb_pos2;
        //spe1=-300;
        flag=1;       
    }
    

  /*判断角度是否在正常范围内*/  
<<<<<<< HEAD
   if(ori_pos1>=ANGLE_MAXIMUM&&ori_pos1<=4095-ANGLE_MAXIMUM)
    {
      if(fdb_pos1>=ori_pos1+ANGLE_MAXIMUM||fdb_pos1<=ori_pos1-ANGLE_MAXIMUM)
      {
        spe1=0;
        mode1=0;
        WheelMode(1,mode1);
        spe2=0;
        mode2=0;
        WheelMode(2,mode2);
        ERROR_FLAG=1;
      }
    }

    if(ori_pos2>=ANGLE_MAXIMUM&&ori_pos2<=4095-ANGLE_MAXIMUM)
    {
      //if(fdb_pos2<=3000&&fdb_pos2>=2000)
      if(fdb_pos2>=ori_pos2+ANGLE_MAXIMUM-150||fdb_pos2<=ori_pos2-ANGLE_MAXIMUM+50)
      {
        spe2=0;
        mode2=0;
        WheelMode(2,mode2);
        spe1=0;
        mode1=0;
        WheelMode(1,mode1);
        ERROR_FLAG=1;

      }
    }
=======
  //  if(ori_pos1>=ANGLE_MAXIMUM&&ori_pos1<=4095-ANGLE_MAXIMUM)
  //   {
  //     if(fdb_pos1>=ori_pos1+ANGLE_MAXIMUM+500||fdb_pos1<=ori_pos1-ANGLE_MAXIMUM-500)
  //     {
  //       spe1=0;
  //       mode1=0;
  //       WheelMode(1,mode1);
  //       spe2=0;
  //       mode2=0;
  //       WheelMode(2,mode2);
  //       ERROR_FLAG=1;
  //     }
  //   }

  //   if(ori_pos2>=ANGLE_MAXIMUM&&ori_pos2<=4095-ANGLE_MAXIMUM)
  //   {
  //     //if(fdb_pos2<=3000&&fdb_pos2>=2000)
  //     if(fdb_pos2>=ori_pos2+ANGLE_MAXIMUM-150||fdb_pos2<=ori_pos2-ANGLE_MAXIMUM+50)
  //     {
  //       spe2=0;
  //       mode2=0;
  //       WheelMode(2,mode2);
  //       spe1=0;
  //       mode1=0;
  //       WheelMode(1,mode1);
  //       ERROR_FLAG=1;

  //     }
  //   }
>>>>>>> origin/zyt1
    /*判断角度是否在正常范围内*/ 
   
     printf("%d,%d,%d,%d,%d,%d,%d,%d\n",spe1,spe2,ori_pos1,ori_pos2,fdb_pos1,fdb_pos2,mode1,mode2);
   
    }
        
}

#include "head.h"

void Statemachine_Task(void *argument)
{
    osDelay(100);
    for (;;) {
    #ifdef RED
        if(Q_NO==1)
        {
            /*past*/
        }
        else if (Q_NO==2)
        {
            /* 仍然使用位置模式 */
<<<<<<< HEAD
            uint8_t square_size=200;        //待确认各个点是哪个位置
            tar_pos1=2048-square_size;
            tar_pos2=2223+square_size;
            osDelay(3000);
            tar_pos1=2048+square_size;
            tar_pos2=2223+square_size;
            osDelay(3000);
            tar_pos1=2048+square_size;
            tar_pos2=2223-square_size;
            osDelay(3000);
            tar_pos1=2048-square_size;
            tar_pos2=2223-square_size;
            osDelay(3000);
            tar_pos1=2048-square_size;
            tar_pos2=2223+square_size;
=======
            uint16_t square_size=139;        //待确认各个点是哪个位置
            tar_pos1=2048-square_size-10;
            tar_pos2=2223+150;
            osDelay(3000);
            tar_pos1=2048+square_size;
            tar_pos2=2223+150;
            osDelay(3000);
            tar_pos1=2048+square_size;
            tar_pos2=2223-28;
            osDelay(3000);
            tar_pos1=2048-square_size-10;
            tar_pos2=2223-28;
            osDelay(3000);
            tar_pos1=2048-square_size-10;
            tar_pos2=2223+150;
>>>>>>> origin/zyt1
            osDelay(3000);
        }
        else if (Q_NO>=3)       //实际上3和4一起算了
        {
            static uint8_t flag=0;
            if(flag==0){
                mode1=1;
                mode2=1; 
                WheelMode(1,mode1);
                WheelMode(2,mode2);   
                flag=1;
            }
            static uint8_t state=0;
            if(state==0){
                spe1=dot1x-red_x;       //正负待定，乘系数待定
                spe2=dot1y-red_y;
                if(abs(dot1x-red_x)<10&&abs(dot1y-red_y)<10) 
                {
                   state=1;
                    osDelay(1000);
                }
            }else if (state==1)
            {
                spe1=dot2x-red_x;       //正负待定，乘系数待定
                spe2=dot2y-red_y;
                if(abs(dot2x-red_x)<10&&abs(dot2y-red_y)<10)
                {
                    state=2;
                    osDelay(1000);
                } 
            }else if (state==2)
            {
                spe1=dot3x-red_x;       //正负待定，乘系数待定
                spe2=dot3y-red_y;
                if(abs(dot3x-red_x)<10&&abs(dot3y-red_y)<10) 
                {
                    state=3;
                    osDelay(1000);
                }
            }else if (state==3)
            {
                spe1=dot4x-red_x;       //正负待定，乘系数待定
                spe2=dot4y-red_y;
                if(abs(dot4x-red_x)<10&&abs(dot4y-red_y)<10) 
                {
                    state=0;
                    osDelay(1000);
                }
            }
            
        }
        
    #endif
    #ifdef GREEN
    if(Q_NO>=4)
    {
        static uint8_t flag=0;
            if(flag==0){
                mode1=1;
                mode2=1; 
                WheelMode(1,mode1);
                WheelMode(2,mode2);   
                flag=1;
            }
        // spe1=red_x-80;  //不一定是80和60，待测, 正负待定，乘系数待定
        // spe2=red_y-60;
        //spe1和spe2由openmv直接发过来,否则要在绿光部分解码red坐标
    }
    
    #endif // DEBUG 
    osDelay(1);
    }
        
    
}

void Statemachine_Start(void)
{
    osThreadId_t StatemachineHandle;
    const osThreadAttr_t Statemachine_attributes = {
        .name       = "Statemachine",
        .stack_size = 128 *3,
        .priority   = (osPriority_t)osPriorityNormal,
    };
    StatemachineHandle = osThreadNew(Statemachine_Task, NULL, &Statemachine_attributes);
}

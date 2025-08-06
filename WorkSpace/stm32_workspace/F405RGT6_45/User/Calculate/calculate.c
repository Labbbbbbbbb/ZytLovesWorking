#include "calculate.h"


/**
 * 
 * 所以就是说不要用角度计给的航向角，而要用龙格库塔算法用角速度算出来是吗* 
 * 角速度---->四元数微分*2----->累加得到四元数---->计算出角度
 * yaw角度---->导航坐标系（这时应该不需要考虑pitch和roll吧？
 * 
 * 以及编码器那里只需要改一下函数定义把angle1和angle2拿出来就好了对吧？
 * 
 */

/*************龙格库塔积分角度***************/
float Runge_Kutta_yaw;
float Runge_Kutta_pitch;
float Runge_Kutta_roll;
float Qk0[4]={1,0,0,0};
float Qk1[4]={1,0,0,0};
float W_matrix[16];

float* Matrix44_41Multiplication(float* matrix_a,float* matrix_b,float* result)
{
    for(uint8_t i = 0; i < 4; i++ )
    {
        for(uint8_t j = 0; j < 4; j++ )
        {
            result[i] += matrix_a[j+4*i] * matrix_b[j];
        }
    }
    return result;
}


float* Matrix22_21Multiplication(float* matrix_a,float* matrix_b,float* result)
{
    for(uint8_t i = 0; i < 2; i++ )
    {
        for(uint8_t j = 0; j < 2; j++ )
        {
            result[i] += matrix_a[j+2*i] * matrix_b[j];
        }
    }
    return result;
}

float* Matrix4_1Add(float* matrix_a,float* matrix_b,float* result)
{
    for(uint8_t i = 0; i < 4; i++ )  result[i] = matrix_a[i] + matrix_b[i];
    return result;
}
void W_matrix_update()
{
    W_matrix[0]=0;
    W_matrix[5]=0;
    W_matrix[10]=0;
    W_matrix[15]=0;

    W_matrix[1]=-1*fGyro[0];
    W_matrix[14]=-1*fGyro[0];
    W_matrix[4]=fGyro[0];
    W_matrix[11]=fGyro[0];

    W_matrix[2]=-1*fGyro[1];
    W_matrix[7]=-1*fGyro[1];
    W_matrix[8]=fGyro[1];
    W_matrix[13]=fGyro[1];

    W_matrix[3]=-1*fGyro[2];
    W_matrix[9]=-1*fGyro[2];
    W_matrix[6]=fGyro[2];
    W_matrix[12]=fGyro[2];
}



void TimingDataProcess(int32_t Ts)      //Q0:上次四元数数据  Q1：本次四元数数据
{
    static uint8_t flag=0;
    static float Q_temp[4]={0};
    W_matrix_update();
    
    if(flag==0)
    {
        float DeltaQ0[4]={0};
        Matrix44_41Multiplication(W_matrix , Qk0 , DeltaQ0);
        for(uint8_t i=0;i<4;i++)
        {
            DeltaQ0[i] *= (float)Ts/2/1000;
        }
        Matrix4_1Add(Qk0,DeltaQ0,Q_temp);
        flag=1;
    }else{
        float DeltaQ1[4]={0};
        Matrix44_41Multiplication(W_matrix , Q_temp , DeltaQ1);
        for(uint8_t i=0;i<4;i++)
        {
            DeltaQ1[i] *= (float)Ts/2/1000;
        }
        Matrix4_1Add(Q_temp,DeltaQ1,Qk1);
        for(uint8_t i=0;i<4;i++)
        {
            Q_temp[i]=0;
            /*****四元数规范化处理*******/
            Qk1[i]=Qk1[i]/(sqrt((double)(Qk1[0]*Qk1[0]+Qk1[1]*Qk1[1]+Qk1[2]*Qk1[2]+Qk1[3]*Qk1[3])));
            Qk0[i]=Qk1[i]; //更新Q数据
        }
        Runge_Kutta_yaw=atan2((double)(2*Qk1[1]*Qk1[2]-2*Qk1[0]*Qk1[3]),(double)(Qk1[0]*Qk1[0]-Qk1[1]*Qk1[1]+Qk1[2]*Qk1[2]-Qk1[3]*Qk1[3]))/3.14159*180;
        Runge_Kutta_pitch=atan((double)(2*Qk1[0]*Qk1[1]+2*Qk1[2]*Qk1[3]))/3.14159*180;
        Runge_Kutta_roll=atan((double)((2*Qk1[0]*Qk1[2]-2*Qk1[1]*Qk1[3])/(Qk1[0]*Qk1[0]-Qk1[1]*Qk1[1]-Qk1[2]*Qk1[2]+Qk1[3]*Qk1[3])))/3.14159*180; //w的单位是什么无所谓i因为最后算的都是比例
        flag=0;
    }
    
}
/**
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
    {
        TimingDataProcess(10); //时间单位：毫秒  //中断处理周期：5ms，数据更新周期：10ms  TIM3为APB1下的时钟
    }
}
**/

/*********************/
/**********导航坐标系的建立**********/
float delta_distance[2]={0};
float position[2]={0};  //0=Xb 和 1=Yb
float Encoder0[2]={0};//[0]为左，[1]为右
float Encoder1[2]={0};
void Error_Modify(float error_angle)  //单位是弧度,钝角时取负值
{
    float temp_delta1=delta_distance[0];
    float temp_delta2=delta_distance[1];
    delta_distance[0]=(cos(error_angle)*temp_delta1-sin(error_angle)*temp_delta2);
    delta_distance[1]=(cos(error_angle)*temp_delta2+sin(error_angle)*temp_delta1);
}
//**待定：编码器转化成距离的方法 */
void Timing_Distance_Calculate(void)
{
    /*先记录起始时的偏移量*/
    static uint8_t offset_flag=0;
    static float offset_angle[2]={0};
    static float offset_yaw=0;
    if(offset_flag==0)
    {
        offset_angle[0]=angle1;
        offset_angle[1]=angle2;
        offset_yaw=fAngle[2];
        if(offset_yaw!=0.00)   offset_flag=1;   //否则运行第一遍时fAngle可能还没有完成更新
    }

    static float yaw_temp=0;
    float yaw_aver=(yaw_temp+fAngle[2]-offset_yaw)/2/180*PI;
    tag=yaw_aver;
    //**计算出distance，即为单位时间内的载体坐标系位移**//
    //printf("yaw_aver:%f,%f\n",yaw_aver,fAngle[2]);
    Encoder1[0]=angle1-offset_angle[0];
    Encoder1[1]=angle2-offset_angle[1];
    delta_distance[0]=1*(Encoder1[0]-Encoder0[0]);      //此处delta[0]为左轮变化量，[1]为右轮变化量
    delta_distance[1]=-1*(Encoder1[1]-Encoder0[1]); //乘以-1是因为向前时编码器数字反而减小
    if(delta_distance[0]>180) delta_distance[0]=delta_distance[0]-360;
    if(delta_distance[0]<-180) delta_distance[0]=delta_distance[0]+360;
    if(delta_distance[1]>180) delta_distance[1]=delta_distance[1]-360;
    if(delta_distance[1]<-180) delta_distance[1]=delta_distance[1]+360;
    Error_Modify(0);
    //printf("distance:%f,%f,%f\n",delta_distance[0],delta_distance[1],tag); 

    float temp_delta1=delta_distance[0];
    float temp_delta2=delta_distance[1];
    delta_distance[0]=(-cos(PI/4)*temp_delta1+sin(PI/4)*temp_delta2);   
    delta_distance[1]=(cos(PI/4)*temp_delta2+sin(PI/4)*temp_delta1);//先转换到45°坐标系  
    temp_delta1=delta_distance[0];
    temp_delta2=delta_distance[1];
    delta_distance[0]=(cos(yaw_aver)*temp_delta1-sin(yaw_aver)*temp_delta2);   
    delta_distance[1]=(cos(yaw_aver)*temp_delta2+sin(yaw_aver)*temp_delta1);   //转换到导航坐标系
    delta_distance[0]*=(float)R1/180*PI;
    delta_distance[1]*=(float)R2/180*PI;
    //printf("delta_distance:%f,%f,%f\n",delta_distance[0],delta_distance[1],yaw_aver);

    position[0]+=delta_distance[0];
    position[1]+=delta_distance[1];
    yaw_temp=fAngle[2]-offset_yaw;
    Encoder0[0]=Encoder1[0];
    Encoder0[1]=Encoder1[1];
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance==TIM3)
    {
        TLE5012B_SPI_RDATA();
        //ProcessData();
        //TimingDataProcess(10); //时间单位：毫秒  //中断处理周期：5ms，数据更新周期：10ms  TIM3为APB1下的时钟
        Timing_Distance_Calculate();//处理周期目前先设1ms
    }
}
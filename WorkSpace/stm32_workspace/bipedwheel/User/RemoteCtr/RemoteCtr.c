/*
 * @Author: doge60 3020118317@qq.com
 * @Date: 2025-12-13 20:44:38
 * @LastEditors: doge60 3020118317@qq.com
 * @LastEditTime: 2025-12-19 21:17:47
 * @FilePath: \bipedwheel\User\RemoteCtr\RemoteCtr.c
 * @Description: 
 * 
 * Copyright (c) 2025 by doge60 3020118317@qq.com, All Rights Reserved. 
 */
#include "RemoteCtr.h"

uint8_t rx_temp;
uint8_t rx_buffer[BUFFER_SIZE]={0};
volatile uint8_t buffer_received = 0; //接收标志位
uint8_t up_flag=0;
uint8_t down_flag=0;

float forward_ref=0;
float turn_ref=0;
float height_ref=0; //0-100

void ProcessRemoteBuffer(uint8_t *(buffer))
{
    int Remote_X,Remote_Y=0;
    if(buffer[0]==0x0D && buffer[1]==0x0A)
    {
        Remote_X=(buffer[2]-0x30)*100+(buffer[3]-0x30)*10+(buffer[4]-0x30)-100;//负左正右
        Remote_Y=(buffer[5]-0x30)*100+(buffer[6]-0x30)*10+(buffer[7]-0x30)-100;//负后正前
        up_flag=buffer[8];
        down_flag=buffer[9];
        if(Remote_Y>0) 
        {
            forward_ref=Remote_Y*1.0/30;  // /30
            if(height_ref>50)
            {
                forward_ref=Remote_Y*1.0/60;  // /30
            }
        }
        else{
            forward_ref=Remote_Y*1.0/45;  // /30
            if(height_ref>50)
            {
                forward_ref=Remote_Y*1.0/70;  // /30
            }
        }
        turn_ref=-Remote_X*1.0/10;  // /6
        if(up_flag==0x31)
        {
        if(height_ref<50)
        {
            height_ref+=6;
        }else if(height_ref<100)
        {
            height_ref+=2;
        }
    }
        if(down_flag==0x31)
        {
        if(height_ref>50)
        {
            height_ref-=2;
        }else if (height_ref>0)
        {
            height_ref-=6;
        }
        
        }

    }


}
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
uint8_t buffer_index = 17;

float forward_ref=0;
float turn_ref=0;

void ProcessRemoteBuffer(uint8_t *(buffer))
{
    int Remote_X,Remote_Y=0;
    if(buffer[BUFFER_SIZE-2]==0x0D && buffer[BUFFER_SIZE-1]==0x0A)
    {
        Remote_X=(buffer[0]-0x30)*100+(buffer[1]-0x30)*10+(buffer[2]-0x30)-100;//负左正右
        Remote_Y=(buffer[3]-0x30)*100+(buffer[4]-0x30)*10+(buffer[5]-0x30)-100;//负后正前
        forward_ref=Remote_Y*1.0/2;
        turn_ref=-Remote_X*1.0/10;
    }
}
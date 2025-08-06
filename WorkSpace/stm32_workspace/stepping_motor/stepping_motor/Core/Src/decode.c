//
// Created by liam on 2023/7/19.
//
#include "decode.h"

byte2int data_decoded[4];
int world_point[4];

void decode(uint8_t *p)
{
    if(p[0]==0xfb && p[1]==0x11 && p[10] == 0xff)
    {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 2; j++) {
                data_decoded[i].byte[j] = p[2 + i * 2 + j];
            }
            world_point[i] = data_decoded[i].data;
        }
    }
    else
    {
        printf("校验不通过！！！\n");
    }
}
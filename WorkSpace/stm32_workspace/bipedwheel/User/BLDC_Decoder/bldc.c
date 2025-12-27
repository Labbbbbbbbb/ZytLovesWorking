#include "bldc.h"

uint8_t bldc_rxdata[1];
float bldc_msg[4];
uint8_t rx_index = 0;
uint8_t rx_complete = 0;
uint8_t bldc_buffer[50];
// 分割字符串并转换为浮点数
void parse_uart_data(void) 
{
    if (!rx_complete) 
    {
        bldc_buffer[rx_index++] = bldc_rxdata[0];
        if (bldc_rxdata[0] == '\n' || rx_index >= sizeof(bldc_buffer) - 1) 
        {
            bldc_buffer[rx_index] = '\n';  
            rx_complete = 1;               // 标记接收完成
            rx_index = 0;                  // 重置索引以便下次接收
        }
        else 
        {
            return;  // 继续接收数据
        }
        
    }
    // 步骤1：用strtok()按逗号分割字符串
    char *token = strtok(bldc_buffer, ",");  // 第一次调用传入原始字符串
    if (token == NULL) {
        rx_complete = 0;
        return;  // 解析失败
    }

    // 步骤2：依次提取4个浮点数（按发送顺序：M0电流、M0速度、M1电流、M1速度）
    bldc_msg[0] = atof(token);  // 第1个数值：DFOC_M0_Current()
    
    token = strtok(NULL, ",");  // 后续调用传入NULL
    if (token == NULL) { rx_complete = 0; return; }
    bldc_msg[1] = atof(token);  // 第2个数值：DFOC_M0_Velocity()
    
    token = strtok(NULL, ",");
    if (token == NULL) { rx_complete = 0; return; }
    bldc_msg[2] = atof(token);  // 第3个数值：DFOC_M1_Current()
    
    token = strtok(NULL, "\n");  // 最后一个数值以换行符结束
    if (token == NULL) { rx_complete = 0; return; }
    bldc_msg[3] = atof(token);  // 第4个数值：DFOC_M1_Velocity()

    // 解析完成，重置标志
    rx_complete = 0;

    // 调试：打印解析结果（可选）
    //printf("M0cur=%.2f, M0vel=%.2f, M1cur=%.2f, M1vel=%.2f\n",
    //       bldc_msg[0], bldc_msg[1], bldc_msg[2], bldc_msg[3]);
}
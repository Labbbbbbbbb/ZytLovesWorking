 /*
 * @Author: szf
 * @Date: 2023-02-22 11:59:22
 * @LastEditTime: 2024-04-16 14:20:28
 * @LastEditors: labbbbbbbbb 
 * @brief 
 * @FilePath: \Upper_trial01\UserCode\Lib\Can\wtr_can.c
 * @WeChat:szf13373959031
 */
/*过滤器配置，can的fifo0接收*/

#include "its_can.h"

uint8_t CanReceiveData[8];
uint16_t CanReceiveMsg[4];
static uint32_t TxMailbox;


HAL_StatusTypeDef CANFilterInit(CAN_HandleTypeDef* hcan){
  CAN_FilterTypeDef  sFilterConfig;

  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;
  
	
  if(HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_CAN_Start(hcan) != HAL_OK)
  {
    Error_Handler();
  }
	
  if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    Error_Handler();
  }

	return HAL_OK;
}

void CanDataDecode(CAN_RxHeaderTypeDef RxHeader,uint8_t* ReceiveData,uint16_t* ReceiveMsg){     //此解码函数仅用于DLC=8的can消息
  /* Can message Decode */
  if( RxHeader.IDE == CAN_ID_STD ){
    if(RxHeader.StdId==0x200)   //标识可以为任何数，此处与发送处一致，为200
    {
      ReceiveMsg[0]=(uint16_t)((ReceiveData[0] << 8) + ReceiveData[1]);
      ReceiveMsg[1]=(uint16_t)((ReceiveData[2] << 8) + ReceiveData[3]);
      ReceiveMsg[2]=(uint16_t)((ReceiveData[4] << 8) + ReceiveData[5]);
      ReceiveMsg[3]=(uint16_t)((ReceiveData[6] << 8) + ReceiveData[7]);

    }
  }
  if( RxHeader.IDE == CAN_ID_EXT ){
    // vesc反馈关掉这里就不会有消息
    ;;
  }
  
}

void CanDataTransmit(CAN_HandleTypeDef *hcanx, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq)
{
    CAN_TxHeaderTypeDef TxMessage;

    TxMessage.DLC   = 0x08;
    TxMessage.StdId = 0x200;
    TxMessage.IDE   = CAN_ID_STD;
    TxMessage.RTR   = CAN_RTR_DATA;

    uint8_t TxData[8];
    TxData[0] = (uint8_t)(cm1_iq >> 8);
    TxData[1] = (uint8_t)cm1_iq;
    TxData[2] = (uint8_t)(cm2_iq >> 8);
    TxData[3] = (uint8_t)cm2_iq;
    TxData[4] = (uint8_t)(cm3_iq >> 8);
    TxData[5] = (uint8_t)cm3_iq;
    TxData[6] = (uint8_t)(cm4_iq >> 8);
    TxData[7] = (uint8_t)cm4_iq;
    while (HAL_CAN_GetTxMailboxesFreeLevel(hcanx) == 0)
        ;
    if (HAL_CAN_AddTxMessage(hcanx, &TxMessage, TxData, &TxMailbox) != HAL_OK) {
        Error_Handler(); // 如果CAN信息发送失败则进入死循环
    }
    

}

// CAN中断回调函数，初始化在Servo.c  Core_xy_Motor_init()里面
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    CAN_RxHeaderTypeDef   RxHeader;
    if( hcan->Instance == hcan1.Instance ){
        if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, CanReceiveData) != HAL_OK)
        {
            Error_Handler();
        }
        CanDataDecode(RxHeader,CanReceiveData,CanReceiveMsg);
    }
}

#ifndef WTR_CAN_H
#define WTR_CAN_H


#include "can.h"

HAL_StatusTypeDef CANFilterInit(CAN_HandleTypeDef *hcan);
void CanDataTransmit(CAN_HandleTypeDef *hcanx, int16_t cm1_iq, int16_t cm2_iq, int16_t cm3_iq, int16_t cm4_iq);
extern uint16_t CanReceiveMsg[4];
#endif

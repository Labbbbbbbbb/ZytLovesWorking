#include "jy901s.h"



static void SensorUartSend(uint8_t *p_data, uint32_t uiSize)
{
    HAL_UART_Transmit(&HUART, p_data, uiSize, uiSize*4);
}
static void CopeSensorData(uint32_t uiReg, uint32_t uiRegNum)
{
	int i;
    for(i = 0; i < uiRegNum; i++)
    {
        switch(uiReg)
        {
            case AZ:
				s_cDataUpdate |= ACC_UPDATE;
            break;
            case GZ:
				s_cDataUpdate |= GYRO_UPDATE;
            break;
            case HZ:
				s_cDataUpdate |= MAG_UPDATE;
            break;
            case Yaw:
				s_cDataUpdate |= ANGLE_UPDATE;
            break;
            default:
				s_cDataUpdate |= READ_UPDATE;
			break;
        }
		uiReg++;
    }
}

static void MX_USART_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  HUART.Instance = USART1;
  HUART.Init.BaudRate = uiBuad;
  HUART.Init.WordLength = UART_WORDLENGTH_8B;
  HUART.Init.StopBits = UART_STOPBITS_1;
  HUART.Init.Parity = UART_PARITY_NONE;
  HUART.Init.Mode = UART_MODE_TX_RX;
  HUART.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  HUART.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&HUART) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

    UART_Start_Receive_IT(&HUART, &ucRxData, 1);
  /* USER CODE END USART1_Init 2 */

}

static void AutoScanSensor(void)
{
	const uint32_t c_uiBaud[9] = {4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};
	int i, iRetry;
	
	for(i = 0; i < 9; i++)
	{
        uiBuad = c_uiBaud[i];
        MX_USART_UART_Init();
		iRetry = 2;
		do
		{
			s_cDataUpdate = 0;
			WitReadReg(AX, 3);
			HAL_Delay(100);
			if(s_cDataUpdate != 0)
			{
				printf("%d baud find sensor\r\n\r\n", c_uiBaud[i]);
				return ;
			}
			iRetry--;
		}while(iRetry);		
	}
	printf("can not find sensor\r\n");
	printf("please check your connection\r\n");
}

void JY901S_Init(void)
{
	WitInit(WIT_PROTOCOL_NORMAL, 0x50);
	WitSerialWriteRegister(SensorUartSend);
	WitRegisterCallBack(CopeSensorData);
	AutoScanSensor();
}

void JY901S_Update(void)
{
    if(s_cDataUpdate)
		{
			printf("3");
			for(i = 0; i < 3; i++)
			{
				fAcc[i] = sReg[AX+i] / 32768.0f * 16.0f;
				fGyro[i] = sReg[GX+i] / 32768.0f * 2000.0f;
				fAngle[i] = sReg[Roll+i] / 32768.0f * 180.0f;
			}
			if(s_cDataUpdate & ACC_UPDATE)
			{
				printf("acc:%.3f %.3f %.3f\r\n", fAcc[0], fAcc[1], fAcc[2]);
				s_cDataUpdate &= ~ACC_UPDATE;
			}
			if(s_cDataUpdate & GYRO_UPDATE)
			{
				printf("gyro:%.3f %.3f %.3f\r\n", fGyro[0], fGyro[1], fGyro[2]);
				s_cDataUpdate &= ~GYRO_UPDATE;
			}
			if(s_cDataUpdate & ANGLE_UPDATE)
			{
                fYaw = (float)((unsigned short)sReg[Yaw]) / 32768 * 180.0;
				printf("angle:%.3f %.3f %.3f(%.3f)\r\n", fAngle[0], fAngle[1], fAngle[2], fYaw);
				s_cDataUpdate &= ~ANGLE_UPDATE;
			}
			if(s_cDataUpdate & MAG_UPDATE)
			{
				printf("mag:%d %d %d\r\n", sReg[HX], sReg[HY], sReg[HZ]);
				s_cDataUpdate &= ~MAG_UPDATE;
			}
            s_cDataUpdate = 0;
		}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance==HUART)
  {
      WitSerialDataIn(ucRxData);
      UART_Start_Receive_IT(huart, &ucRxData, 1);
  }
}
收录了两个版本的库，Emm版本是官方例程改过来的，datou版本是网上扒拉下来并且更改了一些帧的格式得到的（但是注意只改了vel和pos两种控制模式的帧，其他并没有去改，可能不能用）

brochure里面有说明书（[Emm_V5.0步进闭环驱动说明书Rev1.3.pdf](file:///F:/ITS_Log/EC_resources/brochure/ZDT闭环步进资料/Emm_V5.0闭环步进资料/说明书/Emm_V5.0步进闭环驱动说明书Rev1.3.pdf)），格式什么的很详细，库可能有错的地方，可以依据那个再改改

同时这两个版本都没有开启串口读的功能，也就是没有从电机接收信息的能力，纯自顾自地跑

Emm版本使用函数：

```
void Emm_V5_Vel_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, bool snF); // 速度模式控制
void Emm_V5_Pos_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, uint32_t clk, bool raF, bool snF); // 位置模式控制
```

主要是这两个，都是用串口发送一定格式的消息帧。直接放到main中就能跑，不需要任何初始化（因为电机默认使能）

注意在main中运行这个的时候一定要加延迟，否则会导致所有数据帧连在一起识别不出来



也可以使用脉冲控制模式

```
HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);
for(int i = 0; i < 200; i++) {
      HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_SET);
     HAL_Delay(1);
     HAL_GPIO_WritePin(STEP_GPIO_Port, STEP_Pin, GPIO_PIN_RESET);
     HAL_Delay(1);
 }
```





用逻辑分析仪得到的数据帧：

位控

![7f7e6f8bc71100a79fbe6ccd91d25b6f](assets\7f7e6f8bc71100a79fbe6ccd91d25b6f.png)

速控

![bbb7bb66dda61f629ebb0cc48f264616](assets\bbb7bb66dda61f629ebb0cc48f264616.png)

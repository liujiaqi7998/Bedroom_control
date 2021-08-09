#ifndef DOOR_H
#define DOOR_H
#include <Blinker.h>
#include <Ticker.h>

bool door_state = false;

BlinkerButton DoorButtonKey("DoorButtonKey"); //初始化门按钮
Ticker CloseDoor_timer;

/**
  * @brief  舵机控制函数
  * @param  angle: 角度映射，0到165左右
  * @retval 无
  */
void servopulse(int angle) //定义一个脉冲函数
{
  int pulsewidth = (angle * 11) + 500; //将角度转化为500-2480的脉宽值
  for (int i = 0; i < 100; i++)         //发送50个脉冲
  {
    ESP.wdtFeed();
    digitalWrite(DoorServo_pin, HIGH); //将舵机接口电平至高
    delayMicroseconds(pulsewidth);     //延时脉宽值的微秒数
    digitalWrite(DoorServo_pin, LOW);  //将舵机接口电平至低
    delayMicroseconds(20000 - pulsewidth);
  }
}

/**
  * @brief  关门函数
  * @retval 无
  */
void CloseDoor()
{
  if (door_state)
  {
    room_state_led(0, 0, 0, 0);
    servopulse(1); // 舵机角度写入角度0
    if (Blinker.connected())
    {
      DoorButtonKey.color("#808080");
      DoorButtonKey.print();
    }
    door_state = 0;
  }
}

/**
  * @brief  开门函数
  * @retval 无
  */
void OpenDoor()
{
  Serial.print("OpenDoor\n");
  if (!door_state)
  {
    room_state_led(0, 150, 0, 50);
    State_Led.show(); // Send the updated pixel colors to the hardware.
    CloseDoor_timer.attach(12, CloseDoor);
    servopulse(165); // 舵机角度写入角度180
    if (Blinker.connected())
    {
      DoorButtonKey.color("#FF0000");
      DoorButtonKey.print();
    }
    door_state = 1;
  }
}

/**
  * @brief  开门按钮回调函数
  * @retval 无
  */
void DoorButtonKey_callback(const String &state)
{
  if (state == BLINKER_CMD_BUTTON_TAP)
    OpenDoor();
}

/**
  * @brief  门锁初始化
  * @retval 无
  */
void int_door()
{
  pinMode(DoorServo_pin, OUTPUT); //设定舵机接口为输出接口
  servopulse(1);
  DoorButtonKey.attach(DoorButtonKey_callback); //Blinker注册门按钮回调函数
}

#endif
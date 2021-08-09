/* 根据Blinker要求，配置设备类型 */
#define BLINKER_WIFI                  //设置WIFI模式
#define BLINKER_ALIGENIE_MULTI_OUTLET //设置天猫精灵设备类型为插座
#define BLINKER_MIOT_MULTI_OUTLET     //设置设备类型为多孔插座
#define BLINKER_ESP_SMARTCONFIG       //智能配网

/* 定义引脚 */

#define Light_pin 4         //灯引脚默认低电平
#define Light_butt 13       //按钮默认低电平
#define Finger_butt 0       //指纹识别按钮默认高电平
#define DoorServo_pin 16    //开门舵机引脚
#define State_Led_PIN 5     //状态LED
#define Finger_power_PIN 10 //指纹识别模块电源
#define Red_LED_pin 14      //状态灯（板载红色）
#define Blue_LED_pin 12     //状态灯（板载蓝色）
#define Finger_Tx 2         //指纹识别Tx
#define Finger_Rx 15        //指纹识别Rx

/* 导入头文件*/
#include <Blinker.h>
#include "state.h"
#include "door.h"
#include "light.h"
#include "finger.h"
#include "assistant.h"


/**
  * @brief  设备复位
  * @retval 无
  */
void deviceReset()
{
  close_state_led();
  Blinker.reset();
}
/**
  * @brief  设备即将复位
  * @retval 无
  */
void devicewillReset()
{
  room_state_led(220, 20, 60, 255);
  door_state_led(220, 20, 60, 255);
}

void dataRead(const String &data)
{
}

void setup()
{
  int_Finger();

  int_state_led();
  room_state_led(220, 20, 60, 255);

  Serial.begin(115200);              //初始化波特率15000
  BLINKER_DEBUG.stream(Serial);      //设置日志输出为Serial
  Blinker.begin("33e718a73b77");               //Blinker初始化
  Blinker.attachData(dataRead);      //Blinker注册回调函数
  Finger_button.setPressTicks(7000); //长按7秒

  Finger_button.attachDuringLongPress(devicewillReset);
  Finger_button.attachLongPressStop(deviceReset); //长按7秒

  int_door();
  int_light();
  int_assistant();
  close_state_led();
}

void loop()
{
  Blinker.run(); //Blinker重要函数
  digitalWrite(Red_LED_pin, Blinker.connected());
  LigtButton_loop();
  Finger_button.tick();
}

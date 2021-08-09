#ifndef LIGHT_H
#define LIGHT_H
#include <Blinker.h>

bool light_state = false;
bool last_LingtButton = true;

BlinkerButton LingtButton("LigtButton"); //初始化灯按钮

/**
  * @brief  开关灯函数
  * @param  state: 电源状态
  * @retval 无
  */
void light_do(bool state)
{
    light_state = state;
    digitalWrite(Light_pin, light_state);
    if (Blinker.connected())
    {
        if (light_state)
        {
            LingtButton.color("#FF0000");
            LingtButton.print("on");
            BlinkerAliGenie.powerState("on", 1);
            BlinkerAliGenie.print();
            BlinkerMIOT.powerState("on", 1);
            BlinkerMIOT.print();
        }
        else
        {
            LingtButton.color("#808080");
            LingtButton.print("off");
            BlinkerAliGenie.powerState("off", 1);
            BlinkerAliGenie.print();
            BlinkerMIOT.powerState("off", 1);
            BlinkerMIOT.print();
        }
    }
}

/**
  * @brief  灯按钮回调函数
  * @param  state: 电源状态
  * @retval 无
  */
void LigtButton_callback(const String &state)
{

    light_do((state == BLINKER_CMD_ON) ? true : false);
}

/**
  * @brief  初始化灯
  * @retval 无
  */
void int_light()
{
    pinMode(Light_butt, INPUT_PULLUP);       //设置按钮pin口输出
    pinMode(Light_pin, OUTPUT);              //设置灯pin口输出
    pinMode(Light_pin, OUTPUT);              //设置灯pin口输出
    LingtButton.attach(LigtButton_callback); //Blinker注册灯按钮回调函数
}

/**
  * @brief  灯按钮循环线程
  * @retval 无
  */
void LigtButton_loop()
{
    if (digitalRead(Light_butt) != last_LingtButton) //结果不等于上次测量
    {
        Blinker.delay(50);                         //不卡线程延时50
        bool LigtButton_stat = digitalRead(Light_butt); //再次读取灯按钮
        if (LigtButton_stat != last_LingtButton)   //结果仍然不等于上次测量
        {
            light_state = !light_state;
            light_do(light_state);
            last_LingtButton = LigtButton_stat; //保存本次状态为上一次状态
        }
    }
}

#endif
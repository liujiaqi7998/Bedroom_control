#ifndef STATE_H
#define STATE_H
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel State_Led(2, State_Led_PIN, NEO_RGB + NEO_KHZ800);
BlinkerButton close_state("close_state"); //初始化关闭指示灯按钮
BlinkerRGB state_clo("state_clo");

/**
  * @brief  室内指示灯
  * @param   r  Red brightness, 0 to 255.
  * @param   g  Green brightness, 0 to 255.
  * @param   b  Blue brightness, 0 to 255.
  * @param   w  White brightness, 0 to 255.
  * @retval 无
  */
void room_state_led(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
  State_Led.setPixelColor(0, State_Led.Color(r, g, b, w));
  State_Led.show();
}

/**
  * @brief  室外指示灯
  * @param   r  Red brightness, 0 to 255.
  * @param   g  Green brightness, 0 to 255.
  * @param   b  Blue brightness, 0 to 255.
  * @param   w  White brightness, 0 to 255.
  * @retval 无
  */
void door_state_led(uint8_t r, uint8_t g, uint8_t b, uint8_t w)
{
  State_Led.setPixelColor(1, State_Led.Color(r, g, b, w));
  State_Led.show();
}
/**
  * @brief  熄灭状态指示灯
  * @retval 无
  */
void close_state_led()
{
  State_Led.clear();
  State_Led.show();
}

/**
  * @brief  关闭指示灯按钮回调函数
  * @retval 无
  */
void close_state_callback(const String &state)
{
  close_state_led();
}

void state_clo_callback(uint8_t r_value, uint8_t g_value, uint8_t b_value, uint8_t bright_value)
{

  State_Led.setBrightness(bright_value);
  State_Led.setPixelColor(0, r_value, g_value, b_value);
  State_Led.show();
}

/**
  * @brief  初始化状态指示灯
  * @retval 无
  */
void int_state_led()
{
  pinMode(Red_LED_pin, OUTPUT);    //设置红色状态灯pin口输出
  digitalWrite(Red_LED_pin, HIGH); //设置红色状态灯拉高

  pinMode(Blue_LED_pin, OUTPUT);    //设置蓝色状态灯pin口输出
  digitalWrite(Blue_LED_pin, HIGH); //设置蓝色状态灯拉高

  close_state.attach(close_state_callback); //Blinker注册关闭指示灯按钮回调函数
  state_clo.attach(state_clo_callback);

  State_Led.begin();
  close_state_led();
}
#endif
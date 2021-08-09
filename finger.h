#ifndef FINGER_H
#define FINGER_H
#include <Blinker.h>
#include <Adafruit_Fingerprint.h>
#include <Ticker.h>
#include "OneButton.h"

#define Finger_Sleep_time 180

bool Finger_lock = false;
bool Finger_power = true;
uint8 Finger_id = 1;

Ticker Finger_lock_timer;
Ticker Finger_power_timer;

SoftwareSerial FingerSerial(Finger_Tx, Finger_Rx);                 //指纹识别模块软串口
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&FingerSerial); //指纹识别模块
OneButton Finger_button(Finger_butt, true);

BlinkerText Finger_state("Finger_state");  //指纹状态文本标签
BlinkerSlider Finger_Slider("Finger_id");  //指纹ID选择滑块
BlinkerButton DeleteButton("Finger_dele"); //删除指纹按钮按钮
BlinkerButton AddButton("Finger_add");     //添加指纹按钮按钮
BlinkerButton CheckButton("Finger_che");   //验证指纹按钮

/**
  * @brief  解除指纹识别占用锁定
  * @retval 无
  */
void Finger_ulock()
{
    if (Finger_lock)
    {
        Finger_lock = false;
        door_state_led(0, 0, 0, 0);
    }
    Finger_lock_timer.detach();
}

/**
  * @brief  关闭指纹识别电源
  * @retval 无
  */
void close_Finger()
{
    if (Finger_power)
    {
        Finger_power = false;
        digitalWrite(Finger_power_PIN, LOW);
    }
    Finger_power_timer.detach();
}

/**
  * @brief  等待开启指纹模块
  * @retval 无
  */
void is_Finger()
{
    Finger_power_timer.detach();
    Finger_power_timer.attach(Finger_Sleep_time, close_Finger); //设置指纹识别自动休眠
    if (!Finger_power)
    {
        digitalWrite(Finger_power_PIN, HIGH); //置指纹识别供电输拉高
        Finger_power = true;
        door_state_led(0, 129, 255, 100);
        LigtButton_loop();
        Blinker.delay(300);
        door_state_led(0, 0, 0, 0);
        LigtButton_loop();
        Blinker.delay(300);
        door_state_led(0, 129, 255, 100);
        LigtButton_loop();
        Blinker.delay(300);
        door_state_led(0, 0, 0, 0);
        LigtButton_loop();
        Blinker.delay(300);
        door_state_led(0, 129, 255, 100);
        LigtButton_loop();
        Blinker.delay(300);
        door_state_led(0, 0, 0, 0);
        LigtButton_loop();
        Blinker.delay(300);
        door_state_led(0, 129, 255, 100);
        LigtButton_loop();
        Blinker.delay(300);
        door_state_led(0, 0, 0, 0);
        LigtButton_loop();
        Blinker.delay(300);
        door_state_led(0, 129, 255, 100);
        LigtButton_loop();
        Blinker.delay(300);
        door_state_led(0, 0, 0, 0);
        LigtButton_loop();
        Blinker.delay(300);
    }
}

/**
  * @brief  验证指纹函数
  * @retval 无
  */
void Check_Finger()
{
    if (!Finger_lock)
    {
        digitalWrite(Blue_LED_pin, LOW); //亮

        is_Finger();
        door_state_led(0, 255, 216, 100);
        Finger_lock = true;
        Finger_lock_timer.attach(7, Finger_ulock);
        Serial.print("Check_Finger\n");
        if (finger.verifyPassword())
        {
            uint8_t p = finger.getImage();

            digitalWrite(Blue_LED_pin, HIGH); //灭

            if (p != FINGERPRINT_OK)
            {
                door_state_led(255, 255, 0, 255);
                Serial.print("getImage error\n");
                if (Blinker.connected())
                    Finger_state.print("指纹状态", "未检测到手指");
                return;
            }

            p = finger.image2Tz();
            if (p != FINGERPRINT_OK)
            {
                door_state_led(255, 165, 0, 255);
                Serial.print("image2Tz error\n");
                if (Blinker.connected())
                    Finger_state.print("指纹状态", "图像错误");
                return;
            }

            p = finger.fingerFastSearch();
            if (p == FINGERPRINT_OK)
            {
                door_state_led(0, 150, 0, 255);
                if (Blinker.connected())
                    Finger_state.print("指纹状态", "用户:" + (String)finger.fingerID);
                OpenDoor();
                return;
            }
            else
            {
                door_state_led(255, 0, 0, 255);
                Serial.print("fingerFastSearch error\n");
                if (Blinker.connected())
                    Finger_state.print("指纹状态", "未知用户");
                return;
            }
        }
        else
        {
            door_state_led(255, 69, 0, 255);
            Serial.print("Did not find fingerprint sensor\n");
            if (Blinker.connected())
                Finger_state.print("指纹状态", "模块错误");
            return;
        }
    }
}

/**
  * @brief  添加指纹函数
  * @retval 无
  */
void AddFinger()
{
    if (!Finger_lock)
    {
        digitalWrite(Blue_LED_pin, LOW); //亮
        is_Finger();
        door_state_led(0, 255, 216, 100);
        Finger_lock = true;
        Finger_lock_timer.attach(10, Finger_ulock);
        Serial.println("AddFinger");
        if (Blinker.connected())
            Finger_state.print("指纹状态", "指纹录入开始");
        if (finger.verifyPassword())
        {
            int p = finger.getImage();
            digitalWrite(Blue_LED_pin, HIGH); //灭
            if (p == FINGERPRINT_OK)
            {
                Serial.println("First Image taken");
            }
            else
            {
                door_state_led(255, 255, 0, 255);
                Serial.print("First Image error");
                if (Blinker.connected())
                    Finger_state.print("指纹状态", "未检测到手指");
                return;
            }
            p = finger.image2Tz(1);
            if (p == FINGERPRINT_OK)
            {
                Serial.println("First Image taken");
            }
            else
            {
                Serial.println("First Image error");
                door_state_led(255, 165, 0, 255);
                if (Blinker.connected())
                    Finger_state.print("指纹状态", "图像错误");
                return;
            }
            Blinker.delay(500);
            p = finger.getImage();
            if (p == FINGERPRINT_OK)
            {
                Serial.println("Second Image taken");
            }
            else
            {
                door_state_led(255, 255, 0, 255);
                Serial.println("Second Image error");
                if (Blinker.connected())
                    Finger_state.print("指纹状态", "请勿移开手指");
                return;
            }
            p = finger.image2Tz(2);
            if (p == FINGERPRINT_OK)
            {
                Serial.println("Second Image taken");
            }
            else
            {
                door_state_led(255, 165, 0, 255);
                Serial.println("Second Image error");
                if (Blinker.connected())
                    Finger_state.print("指纹状态", "图像错误");
                return;
            }
            p = finger.createModel();
            if (p == FINGERPRINT_OK)
            {
                Serial.println("Prints matched!");
            }
            else
            {
                door_state_led(255, 0, 0, 255);
                Serial.println("Second Image error");
                if (Blinker.connected())
                    Finger_state.print("指纹状态", "两次指纹不匹配");
                return;
            }
            p = finger.storeModel(Finger_id);
            if (p == FINGERPRINT_OK)
            {
                door_state_led(0, 150, 0, 255);
                Serial.println("Prints Stored!");
                if (Blinker.connected())
                    Finger_state.print("指纹状态", "录入完成");
                return;
            }
            else
            {
                door_state_led(255, 0, 0, 255);
                Serial.println("Second Image error");
                if (Blinker.connected())
                    Finger_state.print("指纹状态", "储存失败");
                return;
            }
        }
        else
        {
            door_state_led(255, 69, 0, 255);
            Serial.println("Did not find fingerprint sensor");
            if (Blinker.connected())
                Finger_state.print("指纹状态", "模块错误");
            return;
        }
    }
}

/**
  * @brief  删除指纹按钮回调函数
  * @retval 无
  */
void DeleteButton_callback(const String &state)
{
    if (state == BLINKER_CMD_BUTTON_TAP)
    {
        Serial.print("Deleted Finger\n");
        if (!Finger_lock)
        {
            if (finger.verifyPassword())
            {
                uint8_t p = -1;
                p = finger.deleteModel(Finger_id);
                if (p == FINGERPRINT_OK)
                {
                    Serial.print("Deleted!\n");
                    if (Blinker.connected())
                        Finger_state.print("指纹状态", "删除成功");
                }
                else
                {
                    Serial.print("Deleted Unknown error: 0x\n");
                    if (Blinker.connected())
                        Finger_state.print("指纹状态", "删除失败");
                }
            }
            else
            {
                Serial.print("Did not find fingerprint sensor\n");
                if (Blinker.connected())
                    Finger_state.print("指纹状态", "模块错误");
            }
        }
    }
}

/**
  * @brief  设置ID回调函数
  * @retval 无
  */
void Finger_Slider_callback(int32_t value)
{
    Finger_id = value;
    Finger_state.print("指纹状态", "选择ID:" + (String)Finger_id);
}

/**
  * @brief  添加指纹按钮回调函数
  * @retval 无
  */
void AddeButton_callback(const String &state)
{
    if (state == BLINKER_CMD_BUTTON_TAP)
    {
        Serial.print("AddeButton_callback\n");
        AddFinger();
    }
}

/**
  * @brief  验证指纹按钮回调函数
  * @retval 无
  */
void CheckButton_callback(const String &state)
{
    if (state == BLINKER_CMD_BUTTON_TAP)
    {
        Serial.print("CheckButton_callback\n");
        Check_Finger();
    }
}

/**
  * @brief  初始化指纹识别
  * @retval 无
  */
void int_Finger()
{
    pinMode(Finger_power_PIN, OUTPUT);                          //设置指纹识别供电输出模式
    digitalWrite(Finger_power_PIN, HIGH);                       //置指纹识别供电输拉高
    Finger_power_timer.attach(Finger_Sleep_time, close_Finger); //设置指纹识别自动休眠
    Finger_button.attachClick(Check_Finger);                    //设置按钮回调
    finger.begin(57600);                                        //初始化指纹识别模块
    Finger_Slider.attach(Finger_Slider_callback);               //Blinker注册指纹ID选择滑块回调函数
    DeleteButton.attach(DeleteButton_callback);                 //Blinker注册删除指纹按钮按钮回调函数
    AddButton.attach(AddeButton_callback);                      //Blinker注册添加指纹按钮按钮回调函数
    CheckButton.attach(CheckButton_callback);                   //Blinker注册验证指纹按钮按钮回调函数
}

#endif
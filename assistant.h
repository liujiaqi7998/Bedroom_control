#ifndef ASSISTANT_H
#define ASSISTANT_H
#include <Blinker.h>

bool oState[5] = {false}; //智能音箱插排状态缓存

/**
  * @brief  天猫精灵多孔插座回调函数
  * @param  state: 电源状态
  * @param  num: 插孔ID
  * @retval 无
  */
void aligeniePowerState(const String &state, uint8_t num)
{
    BLINKER_LOG("need set outlet: ", num, ", power state: ", state);
    BlinkerAliGenie.powerState(state, num);
    BlinkerAliGenie.print();
    oState[num] = (state == BLINKER_CMD_ON) ? true : false;
    light_do(oState[1]);
}

/**
  * @brief  天猫精灵多孔插座查询函数
  * @param  state: 电源状态
  * @param  num: 插孔ID
  * @retval 无
  */
void aligenieQuery(int32_t queryCode, uint8_t num)
{
    BLINKER_LOG("AliGenie Query outlet: ", num, ", codes: ", queryCode);
    BlinkerAliGenie.powerState(oState[num] ? "on" : "off", num);
    BlinkerAliGenie.print();
}

/**
  * @brief  小爱同学多孔插座回调函数
  * @param  state: 电源状态
  * @param  num: 插孔ID
  * @retval 无
  */

void miotPowerState(const String &state, uint8_t num)
{
    BLINKER_LOG("need set outlet: ", num, ", power state: ", state);
    BlinkerMIOT.powerState(state, num);
    BlinkerMIOT.print();
    oState[num] = (state == BLINKER_CMD_ON) ? true : false;
    light_do(oState[1]);
}

/**
  * @brief  小爱同学多孔插座查询函数
  * @param  state: 电源状态
  * @param  num: 插孔ID
  * @retval 无
  */
void miotQuery(int32_t queryCode, uint8_t num)
{
    BLINKER_LOG("AliGenie Query outlet: ", num, ", codes: ", queryCode);
    BlinkerMIOT.powerState(oState[num] ? "on" : "off", num);
    BlinkerMIOT.print();
}

/**
  * @brief  初始化智能音箱
  * @retval 无
  */
void int_assistant()
{
    BlinkerAliGenie.attachPowerState(aligeniePowerState); //Blinker注册天猫精灵命令函数
    BlinkerAliGenie.attachQuery(aligenieQuery);           //Blinker注册天猫精灵查询函数
    BlinkerMIOT.attachPowerState(miotPowerState);         //Blinker注册小爱同学查询函数
    BlinkerMIOT.attachQuery(miotQuery);                   //Blinker注册小爱同学查询函数
}

#endif
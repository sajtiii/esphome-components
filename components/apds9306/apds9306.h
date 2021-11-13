#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/application.h"
#include "esphome/core/preferences.h"

#include <cmath>

namespace esphome {
namespace apds9306 {

  #define APDS9306_I2C_ADDRESS 0x52

  //Command register
  #define APDS9306_CMD_MAIN_CTRL  0x00
  #define APDS9306_CMD_ALS_MEAS_RATE  0x04
  #define APDS9306_CMD_ALS_GAIN 0x05
  #define APDS9306_CMD_PART_ID 0x06
  #define APDS9306_CMD_MAIN_STATUS 0x07
  #define APDS9306_CMD_CLEAR_DATA_0 0x0A
  #define APDS9306_CMD_CLEAR_DATA_1 0x0B
  #define APDS9306_CMD_CLEAR_DATA_2 0x0C
  #define APDS9306_CMD_ALS_DATA_0 0x0D
  #define APDS9306_CMD_ALS_DATA_1 0x0E
  #define APDS9306_CMD_ALS_DATA_2 0x0F
  #define APDS9306_CMD_INT_CFG 0x19
  #define APDS9306_CMD_INT_PERSISTENCE 0x1A
  #define APDS9306_CMD_ALS_THRES_UP_0 0x21
  #define APDS9306_CMD_ALS_THRES_UP_1 0x22
  #define APDS9306_CMD_ALS_THRES_UP_2 0x23
  #define APDS9306_CMD_ALS_THRES_LOW_0 0x24
  #define APDS9306_CMD_ALS_THRES_LOW_1 0x25
  #define APDS9306_CMD_ALS_THRES_LOW_2 0x26
  #define APDS9306_CMD_ALS_THRES_VAR 0x27


  //MAIN_CTRL register
  #define APDS9306_MAIN_CTRL_SW_RESET 0x10
  #define APDS9306_MAIN_CTRL_ALS_ENABLE 0x02
  #define APDS9306_MAIN_CTRL_ALS_STANDBY 0x00


  typedef enum
  {
    APDS9306_ALS_MEAS_RES_20BIT_400MS = 0x00,
    APDS9306_ALS_MEAS_RES_19BIT_200MS = 0x10,
    APDS9306_ALS_MEAS_RES_18BIT_100MS = 0x20,
    APDS9306_ALS_MEAS_RES_17BIT_50MS = 0x30,
    APDS9306_ALS_MEAS_RES_16BIT_25MS = 0x40,
    APDS9306_ALS_MEAS_RES_13BIT_3MS = 0x50
  }
  APDS9306_ALS_MEAS_RES_t;


  typedef enum
  {
    APDS9306_ALS_MEAS_RATE_25MS = 0x00,
    APDS9306_ALS_MEAS_RATE_50MS = 0x01,
    APDS9306_ALS_MEAS_RATE_100MS = 0x02,
    APDS9306_ALS_MEAS_RATE_200MS = 0x03,
    APDS9306_ALS_MEAS_RATE_500MS = 0x04,
    APDS9306_ALS_MEAS_RATE_1000MS = 0x05,
    APDS9306_ALS_MEAS_RATE_2000MS = 0x06
  }
  APDS9306_ALS_MEAS_RATE_t;

  typedef enum
  {
    APDS9306_ALS_GAIN_1 = 0x00,
    APDS9306_ALS_GAIN_3 = 0x01,
    APDS9306_ALS_GAIN_6 = 0x02,
    APDS9306_ALS_GAIN_9 = 0x03,
    APDS9306_ALS_GAIN_18 = 0x04
  }
  APDS9306_ALS_GAIN_t;

  class APDS9306Component;
 
  class APDS9306Component : public PollingComponent, public sensor::Sensor, public i2c::I2CDevice {
    public:
      void setup() override;
      void update() override;
      float get_setup_priority() const override { return setup_priority::DATA; }
  };
 }  // namespace apds9306
 }  // namespace esphome

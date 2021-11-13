#include "apds9306.h"
#include "esphome/core/log.h"

namespace esphome {
namespace apds9306 {

static const char *const TAG = "apds9306";

void APDS9306Component::power_on() {
    if (this->write_byte(APDS9306_CMD_MAIN_CTRL, APDS9306_MAIN_CTRL_ENABLE)) {
        this->mark_failed();
    }
}

void APDS9306Component::power_off() {
    if (this->write_byte(APDS9306_CMD_MAIN_CTRL, APDS9306_MAIN_CTRL_DISABLE)) {
        this->mark_failed();
    }
}

int APDS9306Component::meas_rate_value() {
    switch (this->meas_rate) {
        case APDS9306_ALS_MEAS_RES_20BIT_400MS:
            return 400;
        case APDS9306_ALS_MEAS_RES_19BIT_200MS:
            return 200;
        case APDS9306_ALS_MEAS_RES_18BIT_100MS:
            return 100;
        case APDS9306_ALS_MEAS_RES_17BIT_50MS:
            return 50;
        case APDS9306_ALS_MEAS_RES_16BIT_25MS:
            return 25;
        case APDS9306_ALS_MEAS_RES_13BIT_3MS:
            return 3;
        default:
            return -1;
    }
}

int APDS9306Component::gain_value() {
    switch (this->gain) {
        case APDS9306_ALS_GAIN_1:
            return 1;
        case APDS9306_ALS_GAIN_3:
            return 3;
        case APDS9306_ALS_GAIN_6:
            return 6;
        case APDS9306_ALS_GAIN_9:
            return 9;
        case APDS9306_ALS_GAIN_18:
            return 18;
        default:
            return -1.0;
    }
}


void APDS9306Component::setup() {
    ESP_LOGCONFIG(TAG, "Setting up APDS9306...");
    uint8_t part_id = 0;
    if (!this->read_byte(APDS9306_CMD_PART_ID, &part_id)) {
        this->mark_failed();
        return;
    }

    if (part_id != APDS9306_PART_ID_APDS9306 && part_id != APDS9306_PART_ID_APDS9306065) {
        ESP_LOGE(TAG, "Wrong chip type...");
        this->mark_failed();
        return;
    }

    ESP_LOGD(TAG, "    Part ID: APDS-9306%s", part_id == APDS9306_PART_ID_APDS9306065 ? "-065" : "");
    this->power_off();
}

void APDS9306Component::set_measurement_rate(APDS9306_ALS_MEAS_RATE meas_rate) {
    this->meas_rate = meas_rate;
    if (!this->write_byte(APDS9306_CMD_ALS_MEAS_RATE, meas_rate)) {
        this->mark_failed();
    }
}

void APDS9306Component::set_gain(APDS9306_ALS_GAIN gain) {
    this->gain = gain;
    if (!this->write_byte(APDS9306_CMD_ALS_GAIN, gain)) {
        this->mark_failed();
    }
}

void APDS9306Component::update() {
    this->power_on();
    while (!(this->read_byte(APDS9306_CMD_MAIN_STATUS) && APDS9306_MAIN_STATUS_ALS_DATA));
    uint32_t raw_data = this->read_byte(APDS9306_CMD_ALS_DATA_2) << 16;
    raw_data |= this->read_byte(APDS9306_CMD_ALS_DATA_2) << 8;
    raw_data |= this->read_byte(APDS9306_CMD_ALS_DATA_2);
    this->power_off();

    publish_state(((float)raw_data / this->gain_value()) * (100.0 / this->meas_rate_value()));
}


}  // namespace apds9306
}  // namespace esphome

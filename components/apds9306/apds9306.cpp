#include "apds9306.h"
#include "esphome/core/log.h"

namespace esphome {
namespace apds9306 {

static const char *const TAG = "apds9306";

void APDS9306Component::enable() {
    if (this->write_byte(APDS9306_CMD_MAIN_CTRL, APDS9306_MAIN_CTRL_ENABLE)) {
        this->mark_failed();
    }
}

void APDS9306Component::disable() {
    if (this->write_byte(APDS9306_CMD_MAIN_CTRL, APDS9306_MAIN_CTRL_DISABLE)) {
        this->mark_failed();
    }
}

float APDS9306Component::meas_res_value() {
    switch (this->meas_res) {
        case APDS9306_ALS_MEAS_RES_400MS:
            return 400;
        case APDS9306_ALS_MEAS_RES_200MS:
            return 200;
        case APDS9306_ALS_MEAS_RES_100MS:
            return 100;
        case APDS9306_ALS_MEAS_RES_50MS:
            return 50;
        case APDS9306_ALS_MEAS_RES_25MS:
            return 25;
        case APDS9306_ALS_MEAS_RES_3MS:
            return 3.125;
        default:
            return -1;
    }
}

int APDS9306Component::meas_rate_value() {
    switch (this->meas_rate) {
        case APDS9306_ALS_MEAS_RATE_25MS:
            return 25;
        case APDS9306_ALS_MEAS_RATE_50MS:
            return 50;
        case APDS9306_ALS_MEAS_RATE_100MS:
            return 100;
        case APDS9306_ALS_MEAS_RATE_200MS:
            return 200;
        case APDS9306_ALS_MEAS_RATE_500MS:
            return 500;
        case APDS9306_ALS_MEAS_RATE_1000MS:
            return 1000;
        case APDS9306_ALS_MEAS_RATE_2000MS:
            return 2000;
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
    this->disable();
}

void APDS9306Component::set_measurement_bits() {
    if (!this->write_byte(APDS9306_CMD_ALS_MEAS_RATE, this->meas_res << 4 | this->meas_rate)) {
        this->mark_failed();
    }
}

void APDS9306Component::set_measurement_resolution(APDS9306_ALS_MEAS_RES meas_res) {
    this->meas_res = meas_res;
    this->set_measurement_bits();
}

void APDS9306Component::set_measurement_rate(APDS9306_ALS_MEAS_RATE meas_rate) {
    this->meas_rate = meas_rate;
    this->set_measurement_bits();
}

void APDS9306Component::set_gain(APDS9306_ALS_GAIN gain) {
    this->gain = gain;
    if (!this->write_byte(APDS9306_CMD_ALS_GAIN, gain)) {
        this->mark_failed();
    }
}

void APDS9306Component::update() {
    this->enable();
    while (!this->data_ready());
    uint8_t raw_data[3];
    this->read_byte(APDS9306_CMD_ALS_DATA_2, &raw_data[0]);
    this->read_byte(APDS9306_CMD_ALS_DATA_2, &raw_data[1]);
    this->read_byte(APDS9306_CMD_ALS_DATA_2, &raw_data[2]);
    this->disable();

    publish_state(((float)(raw_data[0] << 16 | raw_data[1] << 8 | raw_data[2]) / this->gain_value()) * (100.0 / this->meas_res_value()));
}

bool APDS9306Component::data_ready() {
    uint8_t status;
    this->read_byte(APDS9306_CMD_MAIN_STATUS, &status);
    return status & APDS9306_MAIN_STATUS_ALS_DATA;
}


}  // namespace apds9306
}  // namespace esphome

#include "apds9306.h"
#include "esphome/core/log.h"

namespace esphome {
namespace apds9306 {

static const char *const TAG = "apds9306";

float APDS9306Component::get_setup_priority() const { return setup_priority::DATA; }

void APDS9306Component::setup() {
    ESP_LOGCONFIG(TAG, "Setting up APDS9306...");
    uint8_t part_id;
    if (!this->read_byte(APDS9306_CMD_PART_ID, &part_id)) {
        this->error_code_ = COMMUNICATION_FAILED;
        this->mark_failed();
        return;
    }
    this->part_id_ = part_id;

    if (this->part_id_ != APDS9306_PART_ID_APDS9306 && this->part_id_ != APDS9306_PART_ID_APDS9306065) {
        this->error_code_ = WRONG_CHIP_TYPE;
        this->mark_failed();
        return;
    }

    if (!this->write_byte(APDS9306_CMD_ALS_MEAS_RATE, this->conf_measurement_resolution_ << 4 | this->conf_measurement_rate_)) {
        ESP_LOGE(TAG, "Failed to set measurement config bits!");
        this->mark_failed();
    }

    if (!this->write_byte(APDS9306_CMD_ALS_GAIN, this->conf_gain_)) {
        ESP_LOGE(TAG, "Failed to set gain bits!");
        this->mark_failed();
    }

    this->disable();
}

void APDS9306Component::dump_config() {
    ESP_LOGCONFIG(TAG, "APDS9306:");
    LOG_I2C_DEVICE(this);
    switch (this->error_code_) {
        case COMMUNICATION_FAILED:
            ESP_LOGE(TAG, "Communication with APDS9306 failed!");
            break;
        case WRONG_CHIP_TYPE:
            ESP_LOGE(TAG, "Wrong chip type. Is it an APDS-9306?");
            break;
        case NONE:
        default:
            break;
    }
    if (this->error_code_ == 0) {
        ESP_LOGCONFIG(TAG, "  Part ID: 0x%X", this->part_id_);
        ESP_LOGCONFIG(TAG, "  Product: %s", this->part_id_ == APDS9306_PART_ID_APDS9306 ? "APDS-9306" : "APDS-9306-065");
    }
    ESP_LOGCONFIG(TAG, "  Measurement resolution: %fms", this->meas_res_value());
    ESP_LOGCONFIG(TAG, "  Measurement rate: %ims", this->meas_rate_value());
    ESP_LOGCONFIG(TAG, "  Gain: %ix", this->gain_value());
    LOG_UPDATE_INTERVAL(this);
}

void APDS9306Component::update() {
    ESP_LOGV(TAG, "Measurement process started.");
    if (!this->enable()) {
        ESP_LOGW(TAG, "Failed to start sensor. Skipping update.");
        this->status_set_warning();
        return;
    }
    while (!this->data_ready());
    uint8_t raw_data[3];
    this->read_byte(APDS9306_CMD_ALS_DATA_2, &raw_data[0]);
    this->read_byte(APDS9306_CMD_ALS_DATA_1, &raw_data[1]);
    this->read_byte(APDS9306_CMD_ALS_DATA_0, &raw_data[2]);
    this->disable();
    ESP_LOGV(TAG, "Measurement process finished.");

    publish_state(((float)(raw_data[0] << 16 | raw_data[1] << 8 | raw_data[2]) / this->gain_value()) * (100.0 / this->meas_res_value()));
}

bool APDS9306Component::enable() {
    return this->write_byte(APDS9306_CMD_MAIN_CTRL, APDS9306_MAIN_CTRL_ENABLE);
}

bool APDS9306Component::disable() {
    return this->write_byte(APDS9306_CMD_MAIN_CTRL, APDS9306_MAIN_CTRL_DISABLE);
}

bool APDS9306Component::data_ready() {
  uint8_t status;
  this->read_byte(APDS9306_CMD_MAIN_STATUS, &status);
  return status & APDS9306_MAIN_STATUS_ALS_DATA;
}

void APDS9306Component::set_measurement_resolution(APDS9306_ALS_MEAS_RES meas_res) {
  this->conf_measurement_resolution_ = meas_res;
}

void APDS9306Component::set_measurement_rate(APDS9306_ALS_MEAS_RATE meas_rate) {
  this->conf_measurement_rate_ = meas_rate;
}

void APDS9306Component::set_gain(APDS9306_ALS_GAIN gain) {
  this->conf_gain_ = gain;
}

float APDS9306Component::meas_res_value() {
    switch (this->conf_measurement_resolution_) {
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
            return 0;
    }
}

int APDS9306Component::meas_rate_value() {
    switch (this->conf_measurement_rate_) {
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
            return 0;
    }
}

int APDS9306Component::gain_value() {
    switch (this->conf_gain_) {
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
            return 0;
    }
}

}  // namespace apds9306
}  // namespace esphome

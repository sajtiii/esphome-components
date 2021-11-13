#include "apds9306.h"
#include "esphome/core/log.h"

namespace esphome {
namespace apds9306 {

static const char *const TAG = "apds9306";

void APDS9306Component::setup() {
    ESP_LOGCONFIG(TAG, "Setting up APDS9306...");
    uint8_t part_id = 0;
    if (!this->read_byte(APDS9306_CMD_PART_ID, &part_id)) {
        this->mark_failed();
        return;
    }

    if (part_id != 0xB1 && part_id != 0xB3) {
        ESP_LOGE(TAG, "Wrong chip type...");
        this->mark_failed();
        return;
    }

    ESP_LOGD(TAG, "    Part ID: APDS-9306%s", part_id == 0XB3 ? "-065" : "");
}

void APDS9306Component::update() {
  publish_state(45);
 // if (this->status_has_warning()) {
 //   ESP_LOGD(TAG, "Retrying to reconnect the sensor.");
 //   this->write_command_(SHT3XD_COMMAND_SOFT_RESET);
 // }
 // if (!this->write_command_(SHT3XD_COMMAND_POLLING_H)) {
 //   this->status_set_warning();
 //   return;
 // }

 // this->set_timeout(50, [this]() {
 //   uint16_t raw_data[2];
 //   if (!this->read_data_(raw_data, 2)) {
 //     this->status_set_warning();
 //     return;
 //   }

 //   float temperature = 175.0f * float(raw_data[0]) / 65535.0f - 45.0f;
 //   float humidity = 100.0f * float(raw_data[1]) / 65535.0f;

 //   ESP_LOGD(TAG, "Got temperature=%.2f°C humidity=%.2f%%", temperature, humidity);
 //   if (this->temperature_sensor_ != nullptr)
 //     this->temperature_sensor_->publish_state(temperature);
 //   if (this->humidity_sensor_ != nullptr)
 //     this->humidity_sensor_->publish_state(humidity);
 //   this->status_clear_warning();
 // });
}

// bool APDS9306Component::write_command_(uint8_t command) {
//     return this->write_byte(command);
// }

//bool APDS9306Component::read_data_(uint16_t *data, uint8_t len) {
 /*const uint8_t num_bytes = len * 3;
 std::vector<uint8_t> buf(num_bytes);

 if (this->read(buf.data(), num_bytes) != i2c::ERROR_OK) {
   return false;
 }

 for (uint8_t i = 0; i < len; i++) {
   const uint8_t j = 3 * i;
   uint8_t crc = sht_crc(buf[j], buf[j + 1]);
   if (crc != buf[j + 2]) {
     ESP_LOGE(TAG, "CRC8 Checksum invalid! 0x%02X != 0x%02X", buf[j + 2], crc);
     return false;
   }
   data[i] = (buf[j] << 8) | buf[j + 1];
 }

 return true;*/
//}

}  // namespace sht3xd
}  // namespace esphome

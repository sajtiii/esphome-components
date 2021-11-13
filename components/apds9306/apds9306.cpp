 #include "apds9306.h"
 #include "esphome/core/log.h"
 
 namespace esphome {
 namespace apds9306 {
 
 static const char *const TAG = "apds9306";
 
 void APDS9306Component::setup() {
   ESP_LOGCONFIG(TAG, "Setting up APDS9306...");
   /*if (!this->write_command_(APDS9306_CMD_PART_ID)) {
     this->mark_failed();
     return;
   }
 
   uint16_t raw_serial_number[2];
   if (!this->read_data_(raw_serial_number, 2)) {
     this->mark_failed();
     return;
   }
   uint32_t serial_number = (uint32_t(raw_serial_number[0]) << 16) | uint32_t(raw_serial_number[1]);
   ESP_LOGV(TAG, "    Serial Number: 0x%08X", serial_number);
 }
 void APDS9306Component::dump_config() {
   ESP_LOGCONFIG(TAG, "SHT3xD:");
   LOG_I2C_DEVICE(this);
   if (this->is_failed()) {
     ESP_LOGE(TAG, "Communication with SHT3xD failed!");
   }
   LOG_UPDATE_INTERVAL(this);
 
   LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
   LOG_SENSOR("  ", "Humidity", this->humidity_sensor_);*/
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
 
 //bool APDS9306Component::write_command_(uint8_t command) {
   // Warning ugly, trick the I2Ccomponent base by setting register to the first 8 bit.
   //return this->write_byte(command >> 8, command & 0xFF);
 //}
 
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

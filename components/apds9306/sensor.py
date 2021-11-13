import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID,
    ICON_LIGHTBULB,
    DEVICE_CLASS_ILLUMINANCE,
    STATE_CLASS_MEASUREMENT,
    CONF_RESOLUTION,
    CONF_RATE,
    CONF_GAIN,
)

DEPENDENCIES = ["i2c"]

CODEOWNERS = ["@sajtiii"]

apds9306 = cg.esphome_ns.namespace("apds9306")
APDS9306Component = apds9306.class_(
    "APDS9306Component", sensor.Sensor, cg.PollingComponent, i2c.I2CDevice
)

APDS9306AlsMeasRes = apds9306.enum("APDS9306_ALS_MEAS_RES")
ALS_MEAS_RES_OPTIONS = {
    "400MS": APDS9306AlsMeasRes.APDS9306_ALS_MEAS_RES_400MS,
    "200MS": APDS9306AlsMeasRes.APDS9306_ALS_MEAS_RES_200MS,
    "100MS": APDS9306AlsMeasRes.APDS9306_ALS_MEAS_RES_100MS,
    "50MS": APDS9306AlsMeasRes.APDS9306_ALS_MEAS_RES_50MS,
    "25MS": APDS9306AlsMeasRes.APDS9306_ALS_MEAS_RES_25MS,
    "3MS": APDS9306AlsMeasRes.APDS9306_ALS_MEAS_RES_3MS,
}

APDS9306AlsMeasRate = apds9306.enum("APDS9306_ALS_MEAS_RATE")
ALS_MEAS_RATE_OPTIONS = {
    "25MS": APDS9306AlsMeasRate.APDS9306_ALS_MEAS_RATE_25MS,
    "50MS": APDS9306AlsMeasRate.APDS9306_ALS_MEAS_RATE_50MS,
    "100MS": APDS9306AlsMeasRate.APDS9306_ALS_MEAS_RATE_100MS,
    "200MS": APDS9306AlsMeasRate.APDS9306_ALS_MEAS_RATE_200MS,
    "500MS": APDS9306AlsMeasRate.APDS9306_ALS_MEAS_RATE_500MS,
    "1000MS": APDS9306AlsMeasRate.APDS9306_ALS_MEAS_RATE_1000MS,
    "2000MS": APDS9306AlsMeasRate.APDS9306_ALS_MEAS_RATE_2000MS,
}

APDS9306AlsGain = apds9306.enum("APDS9306_ALS_GAIN")
ALS_GAIN_OPTIONS = {
    "1X": APDS9306AlsGain.APDS9306_ALS_GAIN_1,
    "3X": APDS9306AlsGain.APDS9306_ALS_GAIN_3,
    "6X": APDS9306AlsGain.APDS9306_ALS_GAIN_6,
    "9X": APDS9306AlsGain.APDS9306_ALS_GAIN_9,
    "18X": APDS9306AlsGain.APDS9306_ALS_GAIN_18,
}


CONFIG_SCHEMA = (
    sensor.sensor_schema(
        icon=ICON_LIGHTBULB,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_ILLUMINANCE,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.GenerateID(): cv.declare_id(APDS9306Component),
            cv.Optional(CONF_RESOLUTION, default="100MS"): cv.enum(
                ALS_MEAS_RES_OPTIONS, upper=True
            ),
            cv.Optional(CONF_RATE, default="100MS"): cv.enum(
                ALS_MEAS_RATE_OPTIONS, upper=True
            ),
            cv.Optional(CONF_GAIN, default="3X"): cv.enum(
                ALS_GAIN_OPTIONS, upper=True
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x52))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    await sensor.register_sensor(var, config)
    cg.add(var.set_measurement_resolution(config[CONF_RESOLUTION]))
    cg.add(var.set_measurement_rate(config[CONF_RATE]))
    cg.add(var.set_gain(config[CONF_GAIN]))

//uncomment line below to enable debug mode
//#define DEBUG_MODE
//uncomment line below to enable debug output of ID pins
//#define DEBUG_SENS_MUX

#define SOFTVERSION "v1.8.1-debug"
uint8_t noSensorReply[6] = {0x01, 0x00,};
uint8_t strobe = 0x00;

//    ANALOG magnet includes    //

uint8_t magnetAnalogIn = A0;
uint16_t magnetSensVal = 0;
uint32_t del_magnet = 1000;

//    ANALOG pressure includes    //

uint8_t pressureAnalogIn = A0;
uint16_t pressureSensVal = 0;
uint32_t del_pressure = 1000;

//    ANALOG TDS includes    //

uint8_t tdsAnalogIn = A0;
uint16_t tdsSensVal = 0;
uint32_t del_tds = 1000;

//    ANALOG thermistor   //

uint8_t thermAnalogIn = A0;
uint16_t thermSensVal = 0;
uint32_t del_therm = 1000;

//    ANALOG PH includes    //

uint8_t phAnalogIn = A0;
uint16_t phSensVal = 0;
uint32_t del_ph = 1000;

//    ANALOG voltage includes   //

uint8_t voltageAnalogIn = A0;
uint16_t voltageSensVal = 0;
uint32_t del_voltage = 1000;

//    ANALOG current includes   //

uint8_t currentAnalogIn = A0;
uint16_t currentSensVal = 0;
uint32_t del_current = 1000;

//      BLE includes          //

#include <ArduinoBLE.h>
BLEService labService("2086C901-9167-4F23-8A7A-F514BD665227");
BLECharacteristic TCS34725_SEND_CHR_UID("B807621A-19E3-40E0-B4F5-AEDCFE28C7CA", BLERead | BLEWrite, 5, true);
BLECharacteristic TCS34725_NOTIFY_CHR_UID("48606B19-820D-48D0-91B8-DF6A4F8DCBD4", BLERead | BLENotify, 33, true);
BLECharacteristic DS18B20_SEND_CHR_UID("09EB425D-8627-4FCB-AEA8-638BCF3F73F7", BLERead | BLEWrite, 5, true);
BLECharacteristic DS18B20_NOTIFY_CHR_UID("8103F9B3-C91E-47CD-8634-4B7D8F4D018D", BLERead | BLENotify, 5, true);
BLECharacteristic MAGNET_SEND_CHR_UID("3B75281E-00A0-4424-84C5-4C549CC1AE82", BLERead | BLEWrite, 6, true);
BLECharacteristic MAGNET_NOTIFY_CHR_UID("EF8A1B0B-1005-4DAD-B49D-75F84488E52C", BLERead | BLENotify, 6, true);
BLECharacteristic MAX31855K_SEND_CHR_UID("2DAF3C9C-CABA-461C-9FBC-1839D6F4E5B9", BLERead | BLEWrite, 5, true);
BLECharacteristic MAX31855K_NOTIFY_CHR_UID("F449E6B7-FE1E-45FF-AEBB-DCFC914DEB42", BLERead | BLENotify, 5, true);
BLECharacteristic BLUX_SEND_CHR_UID("B97A0D31-5278-408E-9CBB-D236E8A1A5C3", BLERead | BLEWrite, 5, true);
BLECharacteristic BLUX_NOTIFY_CHR_UID("BF7C133C-7438-4AEF-A577-3D1BF65B2D1D", BLERead | BLENotify, 5, true);
BLECharacteristic TDS_SEND_CHR_UID("FE0BECBC-6B15-4BEB-89EA-7FE670BC75C9", BLERead | BLEWrite, 6, true);
BLECharacteristic TDS_NOTIFY_CHR_UID("89117508-0D7E-47CC-9688-09177F4E979B", BLERead | BLENotify, 6, true);
BLECharacteristic PH_SEND_CHR_UID("4572926D-B013-4868-9DD1-A930CD44D7FF", BLERead | BLEWrite, 6, true);
BLECharacteristic PH_NOTIFY_CHR_UID("9B264AE2-98F1-4905-A495-C15113A0D35B", BLERead | BLENotify, 6, true);
BLECharacteristic MPX57000P_SEND_CHR_UID("6C8B0BBC-8096-4A20-9051-1819B5001EFD", BLERead | BLEWrite, 6, true);
BLECharacteristic MPX57000P_NOTIFY_CHR_UID("D22A68DB-3CC5-44FA-BDDE-45528B2A367D", BLERead | BLENotify, 6, true);
BLECharacteristic VOLT_SEND_CHR_UID("EB0C9C2F-CD83-4460-8AC5-33DDAB36A393", BLERead | BLEWrite, 6, true);
BLECharacteristic VOLT_NOTIFY_CHR_UID("3C04F8A5-C302-468F-975B-E0EFD4FF2DD4", BLERead | BLENotify, 6, true);
BLECharacteristic ACS712_SEND_CHR_UID("9B69DE8C-B847-4F81-A3E6-9D6998740D15", BLERead | BLEWrite, 6, true);
BLECharacteristic ACS712_NOTIFY_CHR_UID("A300379E-2934-43F7-BA6D-AECF4CF6605B", BLERead | BLENotify, 6, true);
BLECharacteristic IMU_SEND_CHR_UID("8F89213C-97EE-4331-8C1D-A60501CB44F1", BLERead | BLEWrite, 5, true);
BLECharacteristic IMU_NOTIFY_CHR_UID("00C31D7F-FDD6-422F-AC87-E4C8EB2F4A52", BLERead | BLENotify, 37, true);
BLECharacteristic HTS_SEND_CHR_UID("03BF53D8-2F5E-45DB-9F52-9FD737BF9605", BLERead | BLEWrite, 5, true);
BLECharacteristic HTS_NOTIFY_CHR_UID("7C41E7D8-EF7C-4B11-A401-49ACA64F5962", BLERead | BLENotify, 9, true);
BLECharacteristic LPS22HB_SEND_CHR_UID("0FBA45BE-2DFE-45FF-9F52-9FD737380605", BLERead | BLEWrite, 5, true);
BLECharacteristic LPS22HB_NOTIFY_CHR_UID("FBF457D8-AF8B-4A22-A60F-49AFA64FA962", BLERead | BLENotify, 5, true);
BLECharacteristic THERM_SEND_CHR_UID("972f7de0-e9cb-4000-a466-c03da4b69dd0", BLERead | BLEWrite, 6, true);
BLECharacteristic THERM_NOTIFY_CHR_UID("a8ffefd0-ab5a-4345-8cc6-97cf4f6e6ecb", BLERead | BLENotify, 6, true);
BLECharacteristic STROBE_CHR_UID("b9446632-734a-4b53-91ac-f72a14ddd3b3", BLERead | BLENotify, 1, true);
uint8_t ds18b20_n = 0, tcs34725_n = 0, magnet_n = 0, max31855_n = 0, imu_n = 0, hts221_n = 0, therm_n = 0,
        bluxv30_n = 0, tds_n = 0, ph_n = 0, pressure_n = 0, voltage_n = 0, current_n = 0, lps22hb_n = 0;
bool magnet_conn = false, voltage_conn = false, tds_conn = false, therm_conn = false,
     ph_conn = false, pressure_conn = false, current_conn = false;
bool magnet_conn_override = false, voltage_conn_override = false, tds_conn_override = false, therm_conn_override = false,
     ph_conn_override = false, pressure_conn_override = false, current_conn_override = false;

//      DS18B20 includes      //

#define MAXIMWIRE_EXTERNAL_PULLUP
#include <MaximWire.h>
MaximWire::Bus line(7);
MaximWire::DS18B20 dallas;
uint32_t del_ds18b20 = 1000;

//      TCS34725 includes     //

#include <Wire.h>
#include "Adafruit_TCS34725.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_420MS, TCS34725_GAIN_1X);
uint32_t del_tcs34725 = 1000;

//    LSM9DS1 includes    //

#include <Arduino_LSM9DS1.h>
uint32_t del_imu = 1000;

//    HTS221 includes   //

#include <Arduino_HTS221.h>
uint32_t del_hts221 = 1000;

//    LPS22HB includes    //

#include <Arduino_LPS22HB.h>
uint32_t del_lps22hb = 1000;

//      MAX31855 includes     //

#include <SPI.h>
#include "Adafruit_MAX31855.h"
const uint8_t MAXCS = 10;
Adafruit_MAX31855 thermocouple(MAXCS);
uint32_t del_max31855 = 1000;

//      B-LUX-V30B includes     //

#include <Adafruit_I2CDevice.h>
#include <Adafruit_BusIO_Register.h>
uint8_t blux_i2c_addr = (uint8_t) 0x4A;
Adafruit_I2CDevice blux_i2c_obj = Adafruit_I2CDevice(blux_i2c_addr);
uint32_t del_bluxv30 = 1000;

//       RTOS includes         //

#include <rtos.h>
#include <mbed.h>
using namespace mbed;
using namespace rtos;
Thread sensMuxThread;
Thread strobingThread;
Thread* max31855Thread;
Thread* tcs34725Thread;
Thread* ds18b20Thread;
Thread* A0Thread;
Thread* A1Thread;
Thread* bluxv30Thread;
Thread* imuThread;
Thread* hts221Thread;
Thread* lps22hbThread;

//routine for thermocouple ---------------------------------------------------

void max31855() {
  thermocouple.begin();
  for (;;) {
    uint32_t time1 = micros() / 1000;
    float c = thermocouple.readCelsius();
    uint8_t s[5] = {0,};
    memcpy(&s[1], &c, sizeof(c));
#ifdef DEBUG_MODE
    Serial.print("[MAX]\tAmbient Temp = ");
    Serial.println(thermocouple.readInternal());
    if (isnan(c)) {
      Serial.println("[MAX]\tSomething wrong with thermocouple!");
    } else {
      Serial.print("[MAX]\tThermocouple temp = ");
      Serial.println(c);
    }
#endif
    MAX31855K_NOTIFY_CHR_UID.writeValue(s, sizeof(s));
    uint32_t time2 = micros() / 1000;
    int32_t delta = del_max31855 - (time2 - time1);
    if (delta > 0) {
      ThisThread::sleep_for(delta);
    } else {
#ifdef DEBUG_MODE
      Serial.print("[MAX]\t!!!Time overflow: ");
      Serial.println(delta);
#endif
      ThisThread::yield();
    }
  }
}

//routine for ds18b20 temperature sensor -------------------------------------

void ds18b20() {
  line.Discover().FindNextDevice(dallas);
  for (;;) {
    uint32_t time1 = micros() / 1000;
    float temp = dallas.GetTemperature<float>(line);
    dallas.Update(line);
    uint8_t s[5] = {0,};
    memcpy(&s[1], (unsigned char*) (&temp),  4);
#ifdef DEBUG_MODE
    Serial.print("[DS18]\tSensor value: ");
    Serial.println(temp);
#endif
    DS18B20_NOTIFY_CHR_UID.writeValue(s, sizeof(s));
    uint32_t time2 = micros() / 1000;
    int32_t delta = del_ds18b20 - (time2 - time1);
    if (delta > 0) {
      ThisThread::sleep_for(delta);
    } else {
#ifdef DEBUG_MODE
      Serial.print("[DS18]\t!!!Time overflow: ");
      Serial.println(delta);
#endif
      ThisThread::yield();
    }
  }
}

//routine for colorimeter tcs34725 -------------------------------------------

void tcs34725() {
  tcs.begin();
  for (;;) {
    uint32_t time1 = micros() / 1000;
    uint16_t r, g, b, c, colorTemp, lux = 0x00;
    float red, green, blue;

    uint8_t s[33] = {255, 0,};
    tcs.getRawData(&r, &g, &b, &c);
    tcs.getRGB(&red, &green, &blue);
    colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);

    memcpy(&s[1], (uint8_t*) &r, 4);
    memcpy(&s[5], (uint8_t*) &g, 4);
    memcpy(&s[9], (uint8_t*) &b, 4);
    memcpy(&s[13], (uint8_t*) &c, 4);

    memcpy(&s[17], (uint8_t*) &red, 4);
    memcpy(&s[21], (uint8_t*) &green, 4);
    memcpy(&s[25], (uint8_t*) &blue, 4);

    memcpy(&s[29], (uint8_t*) &colorTemp, 2);

    //colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);

    //s[1] = (uint8_t)(r & 0x00FF);
    //s[2] = (uint8_t)((r & 0xFF00) >> 8);
    //s[5] = (uint8_t)(g & 0x00FF);
    //s[6] = (uint8_t)((g & 0xFF00) >> 8);
    //s[9] = (uint8_t)(b & 0x00FF);
    //s[10] = (uint8_t)((b & 0xFF00) >> 8);
    //s[13] = (uint8_t)(colorTemp & 0x00FF);
    //s[14] = (uint8_t)((colorTemp & 0xFF00) >> 8);
#ifdef DEBUG_MODE
    Serial.print("[COLOR]\tRAW ["); Serial.print(r); Serial.print(","); Serial.print(g); Serial.print(","); Serial.print(b); Serial.print(","); Serial.print(c);
    Serial.print("]. RGB ["); Serial.print(red); Serial.print(","); Serial.print(green); Serial.print(","); Serial.print(blue); Serial.print("] T "); Serial.println(colorTemp);


    //Serial.print("[COLOR]\tColor Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
    //Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
    //Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
    //Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
    //Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
    //Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
    //Serial.println(" ");
    //Serial.println(r, HEX);
    //Serial.println(s[1], HEX);
    //Serial.println(s[2], HEX);
#endif
    TCS34725_NOTIFY_CHR_UID.writeValue(s, sizeof(s));
    uint32_t time2 = micros() / 1000;
    int32_t delta = del_tcs34725 - (time2 - time1);
    if (delta > 0) {
      ThisThread::sleep_for(delta);
    } else {
#ifdef DEBUG_MODE
      Serial.print("[COLOR]\t!!!Time overflow: ");
      Serial.println(delta);
#endif
      ThisThread::yield();
    }
  }
}

//routine for analog TDS sensor ----------------------------------------------

void tds() {
  pinMode(tdsAnalogIn, INPUT);
  for (;;) {
    uint32_t time1 = micros() / 1000;
    uint8_t s[6];
    if (tds_conn) {
      s[0] = 0xFF;
    } else {
      s[0] = 0x01;
    }
    s[1] = 0x00;
    tdsSensVal = analogRead(tdsAnalogIn);
    memcpy(&s[2], &tdsSensVal, sizeof(tdsSensVal));
#ifdef DEBUG_MODE
    Serial.print("[TDS]\tSensor raw value: ");
    Serial.println(tdsSensVal);
#endif
    TDS_NOTIFY_CHR_UID.writeValue(s, 6);
    uint32_t time2 = micros() / 1000;
    int32_t delta = del_tds - (time2 - time1);
    if (delta > 0) {
      ThisThread::sleep_for(delta);
    } else {
#ifdef DEBUG_MODE
      Serial.print("[TDS]\t!!!Time overflow: ");
      Serial.println(delta);
#endif
      ThisThread::yield();
    }
  }
}

//routine for LPS22HB sensor -------------------------------------------------

void lps22hb() {
  BARO.begin();
  for (;;) {
    uint32_t time1 = micros() / 1000;
    float pressure = BARO.readPressure();
    uint8_t s[5] = {0,};
    memcpy(&s[1], (uint8_t*) &pressure, 4);
#ifdef DEBUG_MODE
    Serial.print("[BARO]\tSensor value: pressure: ");
    Serial.print(pressure);
    Serial.println(" kPa");
#endif
    LPS22HB_NOTIFY_CHR_UID.writeValue(s, sizeof(s));
    uint32_t time2 = micros() / 1000;
    int32_t delta = del_lps22hb - (time2 - time1);
    if (delta > 0) {
      ThisThread::sleep_for(delta);
    } else {
#ifdef DEBUG_MODE
      Serial.print("[BARO]\t!!!Time overflow: ");
      Serial.println(delta);
#endif
      ThisThread::yield();
    }
  }
}

//routine for HTS221 sensor --------------------------------------------------

void hts221() {
  HTS.begin();
  for (;;) {
    uint32_t time1 = micros() / 1000;
    float temp = HTS.readTemperature(),
          humid = HTS.readHumidity();
    uint8_t s[9] = {0,};
    memcpy(&s[1], (uint8_t*) &temp, 4);
    memcpy(&s[5], (uint8_t*) &humid, 4);
#ifdef DEBUG_MODE
    Serial.print("[HTS]\tSensor value: temp: ");
    Serial.print(temp);
    Serial.print(", humid: ");
    Serial.println(humid);
#endif
    HTS_NOTIFY_CHR_UID.writeValue(s, sizeof(s));
    uint32_t time2 = micros() / 1000;
    int32_t delta = del_hts221 - (time2 - time1);
    if (delta > 0) {
      ThisThread::sleep_for(delta);
    } else {
#ifdef DEBUG_MODE
      Serial.print("[HTS]\t!!!Time overflow: ");
      Serial.println(delta);
#endif
      ThisThread::yield();
    }
  }
}

//routine for LSM9DS1 sensor -------------------------------------------------

void lsm9ds1() {
  IMU.begin();
  for (;;) {
    uint32_t time1 = millis();
    float xa, ya, za, xg, yg, zg, xm, ym, zm;
    IMU.readAcceleration(xa, ya, za);
    IMU.readGyroscope(xg, yg, zg);
    IMU.readMagneticField(xm, ym, zm);
    uint8_t s[37] = {0xFF, 0,};
    memcpy(&s[1], (uint8_t*) &xa, 4);
    memcpy(&s[5], (uint8_t*) &ya, 4);
    memcpy(&s[9], (uint8_t*) &za, 4);
    memcpy(&s[13], (uint8_t*) &xg, 4);
    memcpy(&s[17], (uint8_t*) &yg, 4);
    memcpy(&s[21], (uint8_t*) &zg, 4);
    memcpy(&s[25], (uint8_t*) &xm, 4);
    memcpy(&s[29], (uint8_t*) &ym, 4);
    memcpy(&s[33], (uint8_t*) &zm, 4);
#ifdef DEBUG_MODE
    Serial.print("[IMU]\tAcceleration value: x: ");
    Serial.print(xa);
    Serial.print(", y: ");
    Serial.print(ya);
    Serial.print(", z: ");
    Serial.println(za);
    Serial.print("[IMU]\tGyroscope value: x: ");
    Serial.print(xg);
    Serial.print(", y: ");
    Serial.print(yg);
    Serial.print(", z: ");
    Serial.println(zg);
    Serial.print("[IMU]\tMagnetic Field value: x: ");
    Serial.print(xm);
    Serial.print(", y: ");
    Serial.print(ym);
    Serial.print(", z: ");
    Serial.println(zm);
#endif
    IMU_NOTIFY_CHR_UID.writeValue(s, sizeof(s));
    uint32_t time2 = millis();
    int32_t delta = del_imu - (time2 - time1);
    if (delta > 0) {
      ThisThread::sleep_for(delta);
    } else {
#ifdef DEBUG_MODE
      Serial.print("[IMU]\t!!!Time overflow: ");
      Serial.println(delta);
#endif
      ThisThread::yield();
    }
  }
}

//routine for B-LUX-V30B sensor ----------------------------------------------

void bluxv30() {
  blux_i2c_obj.begin();
  for (;;) {
    uint32_t time1 = micros() / 1000;
    Adafruit_BusIO_Register data_reg = Adafruit_BusIO_Register(&blux_i2c_obj, 0x00, 4, LSBFIRST);
    uint32_t val = data_reg.read();
    uint8_t s[5] = {0,};
    float lux = ((float)val * 1.4) / 1000.0;
    memcpy(&s[1], &lux, sizeof(lux));
#ifdef DEBUG_MODE
    Serial.print("[LUX]\tSensor value: ");
    Serial.println(lux);
#endif
    BLUX_NOTIFY_CHR_UID.writeValue(s, sizeof(s));
    uint32_t time2 = micros() / 1000;
    int32_t delta = del_bluxv30 - (time2 - time1);
    if (delta > 0) {
      ThisThread::sleep_for(delta);
    } else {
#ifdef DEBUG_MODE
      Serial.print("[LUX]\t!!!Time overflow: ");
      Serial.println(delta);
#endif
      ThisThread::yield();
    }
  }
}
//routine for analog thermistor  ---------------------------------------------

void therm() {
  pinMode(thermAnalogIn, INPUT);
  for (;;) {
    uint32_t time1 = millis();
    thermSensVal = analogRead(thermAnalogIn);
    uint8_t s[6];
    if (therm_conn) {
      s[0] = 0xFF;
    } else {
      s[0] = 0x01;
    }
    s[1] = 0x00;
    memcpy(&s[2], &thermSensVal, sizeof(thermSensVal));
#ifdef DEBUG_MODE
    Serial.print("[THERM]\tSensor raw value: ");
    Serial.println(thermSensVal);
#endif
    THERM_NOTIFY_CHR_UID.writeValue(s, sizeof(s));
    uint32_t time2 = millis();
    int32_t delta = del_therm - (time2 - time1);
    if (delta > 0) {
      ThisThread::sleep_for(delta);
    } else {
#ifdef DEBUG_MODE
      Serial.print("[THERM]\t!!!Time overflow: ");
      Serial.println(delta);
#endif
      ThisThread::yield();
    }
  }
}

//routine for analog PH sensor -----------------------------------------------

void ph() {
  pinMode(phAnalogIn, INPUT);
  for (;;) {
    uint32_t time1 = micros() / 1000;
    phSensVal = analogRead(phAnalogIn);
    uint8_t s[6];
    if (ph_conn) {
      s[0] = 0xFF;
    } else {
      s[0] = 0x01;
    }
    s[1] = 0x00;
    memcpy(&s[2], &phSensVal, sizeof(phSensVal));
#ifdef DEBUG_MODE
    Serial.print("[PH]\tSensor raw value: ");
    Serial.println(phSensVal);
#endif
    PH_NOTIFY_CHR_UID.writeValue(s, sizeof(s));
    uint32_t time2 = micros() / 1000;
    int32_t delta = del_ph - (time2 - time1);
    if (delta > 0) {
      ThisThread::sleep_for(delta);
    } else {
#ifdef DEBUG_MODE
      Serial.print("[PH]\t!!!Time overflow: ");
      Serial.println(delta);
#endif
      ThisThread::yield();
    }
  }
}

//routine for analog pressure sensor -----------------------------------------

void pressure() {
  pinMode(pressureAnalogIn, INPUT);
  for (;;) {
    uint32_t time1 = micros() / 1000;
    pressureSensVal = analogRead(pressureAnalogIn);
    uint8_t s[6];
    if (pressure_conn) {
      s[0] = 0xFF;
    } else {
      s[0] = 0x01;
    }
    s[1] = 0x00;
    memcpy(&s[2], &pressureSensVal, sizeof(pressureSensVal));
#ifdef DEBUG_MODE
    Serial.print("[PRES]\tSensor raw value: ");
    Serial.println(pressureSensVal);
#endif
    MPX57000P_NOTIFY_CHR_UID.writeValue(s, sizeof(s));
    uint32_t time2 = micros() / 1000;
    int32_t delta = del_pressure - (time2 - time1);
    if (delta > 0) {
      ThisThread::sleep_for(delta);
    } else {
#ifdef DEBUG_MODE
      Serial.print("[PRES]\t!!!Time overflow: ");
      Serial.println(delta);
#endif
      ThisThread::yield();
    }
  }
}
//routine for analog voltage sensor ------------------------------------------

void voltage() {
  pinMode(voltageAnalogIn, INPUT);
  for (;;) {
    uint32_t time1 = micros() / 1000;
    voltageSensVal = analogRead(voltageAnalogIn);
    uint8_t s[6];
    if (voltage_conn) {
      s[0] = 0xFF;
    } else {
      s[0] = 0x01;
    }
    s[1] = 0x00;
    memcpy(&s[2], &voltageSensVal, sizeof(voltageSensVal));
#ifdef DEBUG_MODE
    Serial.print("[VOLT]\tSensor raw value: ");
    Serial.println(voltageSensVal);
#endif
    VOLT_NOTIFY_CHR_UID.writeValue(s, sizeof(s));
    uint32_t time2 = micros() / 1000;
    int32_t delta = del_voltage - (time2 - time1);
    if (delta > 0) {
      ThisThread::sleep_for(delta);
    } else {
#ifdef DEBUG_MODE
      Serial.print("[VOLT]\t!!!Time overflow: ");
      Serial.println(delta);
#endif
      ThisThread::yield();
    }
  }
}

//routine for analog magnet sensor -------------------------------------------

void magnet() {
  pinMode(magnetAnalogIn, INPUT);
  for (;;) {
    uint32_t time1 = micros() / 1000;
    magnetSensVal = analogRead(magnetAnalogIn);
    uint8_t s[6];
    if (magnet_conn) {
      s[0] = 0xFF;
    } else {
      s[0] = 0x01;
    }
    s[1] = 0x00;
    memcpy(&s[2], &magnetSensVal, sizeof(magnetSensVal));
#ifdef DEBUG_MODE
    Serial.print("[MAGN]\tSensor raw value: ");
    Serial.println(magnetSensVal);
#endif
    MAGNET_NOTIFY_CHR_UID.writeValue(s, sizeof(s));
    uint32_t time2 = micros() / 1000;
    int32_t delta = del_magnet - (time2 - time1);
    if (delta > 0) {
      ThisThread::sleep_for(delta);
    } else {
#ifdef DEBUG_MODE
      Serial.print("[MAGN]\t!!!Time overflow: ");
      Serial.println(delta);
#endif
      ThisThread::yield();
    }
  }
}

//routine for analog current sensor ------------------------------------------

void current() {
  pinMode(currentAnalogIn, INPUT);
  for (;;) {
    uint32_t time1 = micros() / 1000;
    currentSensVal = analogRead(currentAnalogIn);
    uint8_t s[6];
    if (current_conn) {
      s[0] = 0xFF;
    } else {
      s[0] = 0x01;
    }
    s[1] = 0x00;
    memcpy(&s[2], &currentSensVal, sizeof(currentSensVal));
#ifdef DEBUG_MODE
    Serial.print("[CURR]\tSensor raw value: ");
    Serial.println(currentSensVal);
#endif
    ACS712_NOTIFY_CHR_UID.writeValue(s, sizeof(s));
    uint32_t time2 = micros() / 1000;
    int32_t delta = del_current - (time2 - time1);
    if (delta > 0) {
      ThisThread::sleep_for(delta);
    } else {
#ifdef DEBUG_MODE
      Serial.print("[CURR]\t!!!Time overflow: ");
      Serial.println(delta);
#endif
      ThisThread::yield();
    }
  }
}

//routine for BLE handlers ---------------------------------------------------

void BLEconnectHandler(BLEDevice central) {
#ifdef DEBUG_MODE
  Serial.print("[DEBUG]\tConnected central: ");
  Serial.println(central.address());
#endif
}

void BLEdisconnectHandler(BLEDevice central) {
#ifdef DEBUG_MODE
  Serial.print("[DEBUG]\tDisconnected central: ");
  Serial.println(central.address());
#endif
}

void BLEwriteDS18B20Handler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("[DEBUG]\tWrite event");
#endif
  uint8_t n_before = ds18b20_n;     //remembering last _n value to prevent exceptions
  uint8_t s[5];
  for (uint8_t i = 0; i < 5; i++) {
    s[i] = (uint8_t)DS18B20_SEND_CHR_UID.value()[i];
  }
  ds18b20_n = s[0];
  del_ds18b20 = (uint32_t)(s[1] << 24) | (uint32_t)(s[2] << 16) | (uint32_t)(s[3] << 8) | (uint32_t)s[4];
  if (ds18b20_n && !n_before) { //thread isn't running and we're launching it
    ds18b20Thread = new Thread;
    ds18b20Thread->start(ds18b20);
  } else if (!ds18b20_n && n_before) {  //thread is running and we're stopping it
    ds18b20Thread->terminate();
    delete ds18b20Thread;
  }
#ifdef DEBUG_MODE //code below isn't necessary, but it's good for debugging
  else if (!ds18b20_n && !n_before) { //we can't stop thread, if it isn't running
    Serial.println("[ERROR]\tIllegal event: stopping non-existing thread of ds18b20!");
  } else if (ds18b20 && n_before) { //we can't run already running thread
    Serial.println("[ERROR]\tIllegal event: launching another thread of ds18b20!");
  }
  Serial.print("[DEBUG]\tdelay_ds18b20 = ");
  Serial.println(del_ds18b20);
#endif
}

void BLEwriteTCS34725Handler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("[DEBUG]\tWrite event");
#endif
  uint8_t n_before = tcs34725_n;
  uint8_t s[5];
  for (uint8_t i = 0; i < 5; i++) {
    s[i] = (uint8_t)TCS34725_SEND_CHR_UID.value()[i];
  }
  tcs34725_n = s[0];
  del_tcs34725 = (uint32_t)(s[1] << 24) | (uint32_t)(s[2] << 16) | (uint32_t)(s[3] << 8) | (uint32_t)s[4];
  if (tcs34725_n && !n_before) {
    tcs34725Thread = new Thread;
    tcs34725Thread->start(tcs34725);
  } else if (!tcs34725_n && n_before) {
    tcs34725Thread->terminate();
    delete tcs34725Thread;
  }
#ifdef DEBUG_MODE
  else if (!tcs34725_n && !n_before) {
    Serial.println("[ERROR]\tIllegal event: stopping non-existing thread of tcs34725!");
  } else if (tcs34725_n && n_before) {
    Serial.println("[ERROR]\tIllegal event: launching another thread of tcs34725!");
  }
  Serial.print("[DEBUG]\tdelay_tcs34725 = ");
  Serial.println(del_tcs34725);
#endif
}

void BLEwriteLPS22HBHandler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("[DEBUG]\tWrite event");
#endif
  uint8_t n_before = lps22hb_n;
  uint8_t s[5];
  for (uint8_t i = 0; i < 5; i++) {
    s[i] = (uint8_t)LPS22HB_SEND_CHR_UID.value()[i];
  }
  lps22hb_n = s[0];
  del_lps22hb = (uint32_t)(s[1] << 24) | (uint32_t)(s[2] << 16) | (uint32_t)(s[3] << 8) | (uint32_t)s[4];
  if (lps22hb_n && !n_before) {
    lps22hbThread = new Thread;
    lps22hbThread->start(lps22hb);
  } else if (!lps22hb_n && n_before) {
    lps22hbThread->terminate();
    delete lps22hbThread;
  }
#ifdef DEBUG_MODE
  else if (!lps22hb_n && !n_before) {
    Serial.println("[ERROR]\tIllegal event: stopping non-existing thread of lps22hb!");
  } else if (lps22hb_n && n_before) {
    Serial.println("[ERROR]\tIllegal event: launching another thread of lps22hb!");
  }
  Serial.print("[DEBUG]\tdelay_lps22hb = ");
  Serial.println(del_lps22hb);
#endif
}

void BLEwriteIMUHandler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("[DEBUG]\tWrite event");
#endif
  uint8_t n_before = imu_n;
  uint8_t s[5];
  for (uint8_t i = 0; i < 5; i++) {
    s[i] = (uint8_t)IMU_SEND_CHR_UID.value()[i];
  }
  imu_n = s[0];
  del_imu = (uint32_t)(s[1] << 24) | (uint32_t)(s[2] << 16) | (uint32_t)(s[3] << 8) | (uint32_t)s[4];
  if (imu_n && !n_before) {
    imuThread = new Thread;
    imuThread->start(lsm9ds1);
  } else if (!imu_n && n_before) {
    imuThread->terminate();
    delete imuThread;
  }
#ifdef DEBUG_MODE
  else if (!imu_n && !n_before) {
    Serial.println("[ERROR]\tIllegal event: stopping non-existing thread of lsm9ds1!");
  } else if (imu_n && n_before) {
    Serial.println("[ERROR]\tIllegal event: launching another thread of lsm9ds1!");
  }
  Serial.print("[DEBUG]\tdelay_imu = ");
  Serial.println(del_imu);
#endif
}

void BLEwriteHTS221Handler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("[DEBUG]\tWrite event");
#endif
  uint8_t n_before = hts221_n;
  uint8_t s[5];
  for (uint8_t i = 0; i < 5; i++) {
    s[i] = (uint8_t)HTS_SEND_CHR_UID.value()[i];
  }
  hts221_n = s[0];
  del_hts221 = (uint32_t)(s[1] << 24) | (uint32_t)(s[2] << 16) | (uint32_t)(s[3] << 8) | (uint32_t)s[4];
  if (hts221_n && !n_before) {
    hts221Thread = new Thread;
    hts221Thread->start(hts221);
  } else if (!hts221_n && n_before) {
    hts221Thread->terminate();
    delete hts221Thread;
  }
#ifdef DEBUG_MODE
  else if (!hts221_n && !n_before) {
    Serial.println("[ERROR]\tIllegal event: stopping non-existing thread of hts221!");
  } else if (hts221_n && n_before) {
    Serial.println("[ERROR]\tIllegal event: launching another thread of hts221!");
  }
  Serial.print("[DEBUG]\tdelay_hts221 = ");
  Serial.println(del_hts221);
#endif
}

void BLEwriteMAGNETHandler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("[DEBUG]\tWrite event");
#endif
  uint8_t n_before = magnet_n;
  uint8_t s[6];
  for (uint8_t i = 0; i < 6; i++) {
    s[i] = (uint8_t)MAGNET_SEND_CHR_UID.value()[i];
  }
  magnet_n = s[0];
  del_magnet = (uint32_t)(s[1] << 24) | (uint32_t)(s[2] << 16) | (uint32_t)(s[3] << 8) | (uint32_t)s[4];
  if (magnet_n && !n_before) {
    switch (s[5]) {
      case 0xFF:
        if (magnetAnalogIn == A0) {
          A0Thread = new Thread;
          A0Thread->start(magnet);
        } else if (magnetAnalogIn == A1) {
          A1Thread = new Thread;
          A1Thread->start(magnet);
        }
        break;
      case 0x00:
        magnetAnalogIn = A0;
        magnet_conn_override = true;
        A0Thread = new Thread;
        A0Thread->start(magnet);
        break;
      case 0x01:
        magnetAnalogIn = A1;
        magnet_conn_override = true;
        A1Thread = new Thread;
        A1Thread->start(magnet);
        break;
    }
  } else if (!magnet_n && n_before) {
    if (magnetAnalogIn == A0) {
      magnet_conn_override = false;
      A0Thread->terminate();
      delete A0Thread;
    } else if (magnetAnalogIn == A1) {
      magnet_conn_override = false;
      A1Thread->terminate();
      delete A1Thread;
    }
#ifdef DEBUG_MODE
  } else if (!magnet_n && !n_before) {
    Serial.println("[ERROR]\tIllegal event: stopping non-existing thread of magnet!");
  } else if (magnet_n && n_before) {
    Serial.println("[ERROR]\tIllegal event: launching another thread of magnet!");
  }
  Serial.print("[DEBUG]\tdelay_magnet = ");
  Serial.println(del_magnet);
#else
  }
#endif
}

void BLEwriteMAX31855Handler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("[DEBUG]\tWrite event");
#endif
  uint8_t n_before = max31855_n;
  uint8_t s[5];
  for (uint8_t i = 0; i < 5; i++) {
    s[i] = (uint8_t)MAX31855K_SEND_CHR_UID.value()[i];
  }
  max31855_n = s[0];
  del_max31855 = (uint32_t)(s[1] << 24) | (uint32_t)(s[2] << 16) | (uint32_t)(s[3] << 8) | (uint32_t)s[4];
  if (max31855_n && !n_before) {
    max31855Thread = new Thread;
    max31855Thread->start(max31855);
  } else if (!max31855_n && n_before) {
    max31855Thread->terminate();
    delete max31855Thread;
  }
#ifdef DEBUG_MODE
  else if (!max31855_n && !n_before) {
    Serial.println("[ERROR]\tIllegal event: stopping non-existing thread of tcs34725!");
  } else {
    Serial.println("[ERROR]\tIllegal event: launching another thread of tcs34725!");
  }
  Serial.print("[DEBUG]\tdelay_max31855 = ");
  Serial.println(del_max31855, DEC);
#endif
}

void BLEwriteBLUXV30Handler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("[DEBUG]\tWrite event");
#endif
  uint8_t n_before = bluxv30_n;
  uint8_t s[5];
  for (uint8_t i = 0; i < 5; i++) {
    s[i] = (uint8_t)BLUX_SEND_CHR_UID.value()[i];
  }
  //?memcpy(&s, &(BLUX_SEND_CHR_UID.value()), sizeof(BLUX_SEND_CHR_UID.value()));
  bluxv30_n = s[0];
  del_bluxv30 = (uint32_t)(s[1] << 24) | (uint32_t)(s[2] << 16) | (uint32_t)(s[3] << 8) | (uint32_t)s[4];
  if (bluxv30_n && !n_before) {
    bluxv30Thread = new Thread;
    bluxv30Thread->start(bluxv30);
  } else if (!bluxv30_n && n_before) {
    bluxv30Thread->terminate();
    delete bluxv30Thread;
  }
#ifdef DEBUG_MODE
  else if (!bluxv30_n && !n_before) {
    Serial.println("[ERROR]\tIllegal event: stopping non-existing thread of tcs34725!");
  } else {
    Serial.println("[ERROR]\tIllegal event: launching another thread of tcs34725!");
  }
  Serial.print("[DEBUG]\tdelay_bluxv30 = ");
  Serial.println(del_bluxv30, DEC);
#endif
}

void BLEwriteTDSHandler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("[DEBUG]\tWrite event");
#endif
  uint8_t n_before = tds_n;
  uint8_t s[6];
  for (uint8_t i = 0; i < 6; i++) {
    s[i] = (uint8_t)TDS_SEND_CHR_UID.value()[i];
  }
  tds_n = s[0];
  del_tds = (uint32_t)(s[1] << 24) | (uint32_t)(s[2] << 16) | (uint32_t)(s[3] << 8) | (uint32_t)s[4];
  if (tds_n && !n_before) {
    switch (s[5]) {
      case 0xFF:
        if (tdsAnalogIn == A0) {
          A0Thread = new Thread;
          A0Thread->start(tds);
        } else if (tdsAnalogIn == A1) {
          A1Thread = new Thread;
          A1Thread->start(tds);
        }
        break;
      case 0x00:
        tdsAnalogIn = A0;
        tds_conn_override = true;
        A0Thread = new Thread;
        A0Thread->start(tds);
        break;
      case 0x01:
        tdsAnalogIn = A1;
        tds_conn_override = true;
        A1Thread = new Thread;
        A1Thread->start(tds);
        break;
    }
  } else if (!tds_n && n_before) {
    if (tdsAnalogIn == A0) {
      tds_conn_override = false;
      A0Thread->terminate();
      delete A0Thread;
    } else if (tdsAnalogIn == A1) {
      tds_conn_override = false;
      A1Thread->terminate();
      delete A1Thread;
    }
#ifdef DEBUG_MODE
  } else if (!tds_n && !n_before) {
    Serial.println("[ERROR]\tIllegal event: stopping non-existing thread of tds!");
  } else if (tds_n && n_before) {
    Serial.println("[ERROR]\tIllegal event: launching another thread of tds!");
  }
  Serial.print("[DEBUG]\tdelay_tds = ");
  Serial.println(del_tds);
#else
  }
#endif
}

void BLEwritePHHandler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("[DEBUG]\tWrite event");
#endif
  uint8_t n_before = ph_n;
  uint8_t s[6];
  for (uint8_t i = 0; i < 6; i++) {
    s[i] = (uint8_t)PH_SEND_CHR_UID.value()[i];
  }
  ph_n = s[0];
  del_ph = (uint32_t)(s[1] << 24) | (uint32_t)(s[2] << 16) | (uint32_t)(s[3] << 8) | (uint32_t)s[4];
  if (ph_n && !n_before) {
    switch (s[5]) {
      case 0xFF:
        if (phAnalogIn == A0) {
          A0Thread = new Thread;
          A0Thread->start(ph);
        } else if (phAnalogIn == A1) {
          A1Thread = new Thread;
          A1Thread->start(ph);
        }
        break;
      case 0x00:
        phAnalogIn = A0;
        ph_conn_override = true;
        A0Thread = new Thread;
        A0Thread->start(ph);
        break;
      case 0x01:
        phAnalogIn = A1;
        ph_conn_override = true;
        A1Thread = new Thread;
        A1Thread->start(ph);
        break;
    }
  } else if (!ph_n && n_before) {
    if (phAnalogIn == A0) {
      ph_conn_override = false;
      A0Thread->terminate();
      delete A0Thread;
    } else if (phAnalogIn == A1) {
      ph_conn_override = false;
      A1Thread->terminate();
      delete A1Thread;
    }
#ifdef DEBUG_MODE
  } else if (!ph_n && !n_before) {
    Serial.println("[ERROR]\tIllegal event: stopping non-existing thread of ph!");
  } else if (ph_n && n_before) {
    Serial.println("[ERROR]\tIllegal event: launching another thread of ph!");
  }
  Serial.print("[DEBUG]\tdelay_ph = ");
  Serial.println(del_ph);
#else
  }
#endif
}

void BLEwriteTHERMHandler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("[DEBUG]\tWrite event");
#endif
  uint8_t n_before = therm_n;
  uint8_t s[6];
  for (uint8_t i = 0; i < 6; i++) {
    s[i] = (uint8_t)THERM_SEND_CHR_UID.value()[i];
  }
  therm_n = s[0];
  del_therm = (uint32_t)(s[1] << 24) | (uint32_t)(s[2] << 16) | (uint32_t)(s[3] << 8) | (uint32_t)s[4];
  if (therm_n && !n_before) {
    switch (s[5]) {
      case 0xFF:
        if (thermAnalogIn == A0) {
          A0Thread = new Thread;
          A0Thread->start(therm);
        } else if (thermAnalogIn == A1) {
          A1Thread = new Thread;
          A1Thread->start(therm);
        }
        break;
      case 0x00:
        thermAnalogIn = A0;
        therm_conn_override = true;
        A0Thread = new Thread;
        A0Thread->start(therm);
        break;
      case 0x01:
        thermAnalogIn = A1;
        therm_conn_override = true;
        A1Thread = new Thread;
        A1Thread->start(therm);
        break;
    }
  } else if (!therm_n && n_before) {
    if (thermAnalogIn == A0) {
      therm_conn_override = false;
      A0Thread->terminate();
      delete A0Thread;
    } else if (thermAnalogIn == A1) {
      therm_conn_override = false;
      A1Thread->terminate();
      delete A1Thread;
    }
#ifdef DEBUG_MODE
  } else if (!therm_n && !n_before) {
    Serial.println("[ERROR]\tIllegal event: stopping non-existing thread of therm!");
  } else if (therm_n && n_before) {
    Serial.println("[ERROR]\tIllegal event: launching another thread of therm!");
  }
  Serial.print("[DEBUG]\tdelay_therm = ");
  Serial.println(del_therm);
#else
  }
#endif
}

void BLEwritePRESHandler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("[DEBUG]\tWrite event");
#endif
  uint8_t n_before = pressure_n;
  uint8_t s[6];
  for (uint8_t i = 0; i < 6; i++) {
    s[i] = (uint8_t)MPX57000P_SEND_CHR_UID.value()[i];
  }
  pressure_n = s[0];
  del_pressure = (uint32_t)(s[1] << 24) | (uint32_t)(s[2] << 16) | (uint32_t)(s[3] << 8) | (uint32_t)s[4];
  if (pressure_n && !n_before) {
    switch (s[5]) {
      case 0xFF:
        if (pressureAnalogIn == A0) {
          A0Thread = new Thread;
          A0Thread->start(pressure);
        } else if (pressureAnalogIn == A1) {
          A1Thread = new Thread;
          A1Thread->start(pressure);
        }
        break;
      case 0x00:
        pressureAnalogIn = A0;
        pressure_conn_override = true;
        A0Thread = new Thread;
        A0Thread->start(pressure);
        break;
      case 0x01:
        pressureAnalogIn = A1;
        pressure_conn_override = true;
        A1Thread = new Thread;
        A1Thread->start(pressure);
        break;
    }
  } else if (!pressure_n && n_before) {
    if (pressureAnalogIn == A0) {
      pressure_conn_override = false;
      A0Thread->terminate();
      delete A0Thread;
    } else if (pressureAnalogIn == A1) {
      pressure_conn_override = false;
      A1Thread->terminate();
      delete A1Thread;
    }
#ifdef DEBUG_MODE
  } else if (!pressure_n && !n_before) {
    Serial.println("[ERROR]\tIllegal event: stopping non-existing thread of pressure!");
  } else if (pressure_n && n_before) {
    Serial.println("[ERROR]\tIllegal event: launching another thread of pressure!");
  }
  Serial.print("[DEBUG]\tdelay_pressure = ");
  Serial.println(del_pressure);
#else
  }
#endif
}

void BLEwriteVOLTHandler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("[DEBUG]\tWrite event");
#endif
  uint8_t n_before = voltage_n;
  uint8_t s[6];
  for (uint8_t i = 0; i < 6; i++) {
    s[i] = (uint8_t)VOLT_SEND_CHR_UID.value()[i];
  }
  voltage_n = s[0];
  del_voltage = (uint32_t)(s[1] << 24) | (uint32_t)(s[2] << 16) | (uint32_t)(s[3] << 8) | (uint32_t)s[4];
  if (voltage_n && !n_before) {
    switch (s[5]) {
      case 0xFF:
        if (voltageAnalogIn == A0) {
          A0Thread = new Thread;
          A0Thread->start(voltage);
        } else if (voltageAnalogIn == A1) {
          A1Thread = new Thread;
          A1Thread->start(voltage);
        }
        break;
      case 0x00:
        voltageAnalogIn = A0;
        voltage_conn_override = true;
        A0Thread = new Thread;
        A0Thread->start(voltage);
        break;
      case 0x01:
        voltageAnalogIn = A1;
        voltage_conn_override = true;
        A1Thread = new Thread;
        A1Thread->start(voltage);
        break;
    }
  } else if (!voltage_n && n_before) {
    if (voltageAnalogIn == A0) {
      voltage_conn_override = false;
      A0Thread->terminate();
      delete A0Thread;
    } else if (voltageAnalogIn == A1) {
      voltage_conn_override = false;
      A1Thread->terminate();
      delete A1Thread;
    }
#ifdef DEBUG_MODE
  } else if (!voltage_n && !n_before) {
    Serial.println("[ERROR]\tIllegal event: stopping non-existing thread of voltage!");
  } else if (voltage_n && n_before) {
    Serial.println("[ERROR]\tIllegal event: launching another thread of voltage!");
  }
  Serial.print("[DEBUG]\tdelay_voltage = ");
  Serial.println(del_voltage);
#else
  }
#endif
}

void BLEwriteCURRHandler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("[DEBUG]\tWrite event");
#endif
  uint8_t n_before = current_n;
  uint8_t s[6];
  for (uint8_t i = 0; i < 6; i++) {
    s[i] = (uint8_t)ACS712_SEND_CHR_UID.value()[i];
  }
  current_n = s[0];
  del_current = (uint32_t)(s[1] << 24) | (uint32_t)(s[2] << 16) | (uint32_t)(s[3] << 8) | (uint32_t)s[4];
  if (current_n && !n_before) {
    switch (s[5]) {
      case 0xFF:
        if (currentAnalogIn == A0) {
          A0Thread = new Thread;
          A0Thread->start(current);
        } else if (currentAnalogIn == A1) {
          A1Thread = new Thread;
          A1Thread->start(current);
        }
        break;
      case 0x00:
        currentAnalogIn = A0;
        current_conn_override = true;
        A0Thread = new Thread;
        A0Thread->start(current);
        break;
      case 0x01:
        currentAnalogIn = A1;
        current_conn_override = true;
        A1Thread = new Thread;
        A1Thread->start(current);
        break;
    }
  } else if (!current_n && n_before) {
    if (currentAnalogIn == A0) {
      current_conn_override = false;
      A0Thread->terminate();
      delete A0Thread;
    } else if (currentAnalogIn == A1) {
      current_conn_override = false;
      A1Thread->terminate();
      delete A1Thread;
    }
#ifdef DEBUG_MODE
  } else if (!current_n && !n_before) {
    Serial.println("[ERROR]\tIllegal event: stopping non-existing thread of current!");
  } else if (current_n && n_before) {
    Serial.println("[ERROR]\tIllegal event: launching another thread of current!");
  }
  Serial.print("[DEBUG]\tdelay_current = ");
  Serial.println(del_current);
#else
  }
#endif
}

//routine for analog sensor selection ----------------------------------------

void analogSensorMux() {
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  for (;;) {
    uint16_t a0_sens_id = analogRead(A2), at_sens_id = analogRead(A3);
    if ((400 < a0_sens_id && a0_sens_id < 860) && (!magnet_conn_override)) {
      magnetAnalogIn = A0;
      magnet_conn = true;
    } else if ((400 < at_sens_id && at_sens_id < 860) && (!magnet_conn_override)) {
      magnetAnalogIn = A1;
      magnet_conn = true;
    } else {
      magnet_conn = false;
    }
    if ((2901 < a0_sens_id && a0_sens_id < 3220) && (!tds_conn_override)) {
      tdsAnalogIn = A0;
      tds_conn = true;
    } else if ((2901 < at_sens_id && at_sens_id < 3220) && (!tds_conn_override)) {
      tdsAnalogIn = A1;
      tds_conn = true;
    } else {
      tds_conn = false;
    }
    if ((2421 < a0_sens_id && a0_sens_id < 2900) && (!ph_conn_override)) {
      phAnalogIn = A0;
      ph_conn = true;
    } else if ((2421 < at_sens_id && at_sens_id < 2900) && (!ph_conn_override)) {
      phAnalogIn = A1;
      ph_conn = true;
    } else {
      ph_conn = false;
    }
    if ((861 < a0_sens_id && a0_sens_id < 1420) && (!pressure_conn_override)) {
      pressureAnalogIn = A0;
      pressure_conn = true;
    } else if ((861 < at_sens_id && at_sens_id < 1420) && (!pressure_conn_override)) {
      pressureAnalogIn = A1;
      pressure_conn = true;
    } else {
      pressure_conn = false;
    }
    if ((1421 < a0_sens_id && a0_sens_id < 1820) && (!voltage_conn_override)) {
      voltageAnalogIn = A0;
      voltage_conn = true;
    } else if ((1421 < at_sens_id && at_sens_id < 1820) && (!voltage_conn_override)) {
      voltageAnalogIn = A1;
      voltage_conn = true;
    } else {
      voltage_conn = false;
    }
    if ((1821 < a0_sens_id && a0_sens_id < 2420) && (!current_conn_override)) {
      currentAnalogIn = A0;
      current_conn = true;
    } else if ((1821 < at_sens_id && at_sens_id < 2420) && (!current_conn_override)) {
      currentAnalogIn = A1;
      current_conn = true;
    } else {
      current_conn = false;
    }
    if ((3221 < a0_sens_id && a0_sens_id < 3600) && (!therm_conn_override)) {
      thermAnalogIn = A0;
      therm_conn = true;
    } else if ((3221 < at_sens_id && at_sens_id < 3600) && (!therm_conn_override)) {
      thermAnalogIn = A1;
      therm_conn = true;
    } else {
      therm_conn = false;
    }
#ifdef DEBUG_SENS_MUX
    Serial.print("a0_sens_id: ");
    Serial.print(a0_sens_id);
    Serial.print("  a1_sens_id: ");
    Serial.println(at_sens_id);
#endif
    ThisThread::sleep_for(100);
  }
}

//routine for strobing status signals ----------------------------------------

void strobing() {
  for (;;) {
    strobe = ~strobe;
    STROBE_CHR_UID.writeValue(strobe, sizeof(strobe));
    ThisThread::sleep_for(1000);
  }
}
void setup() {
  analogReadResolution(12);
#ifdef DEBUG_MODE
  Serial.begin(9600);
  while (!Serial) delay(1);
  Serial.println();
  Serial.print("Multisensor software ");
  Serial.print(SOFTVERSION);
  Serial.println(" init... ");
  Serial.print("[DEBUG]\tStarting BLE... ");
#endif
  if (BLE.begin()) {
    Serial.println("DONE.");
    Serial.print("[DEBUG]\tLoading BLE name and service... ");
    BLE.setLocalName("LAB33");
    BLE.setAdvertisedService(labService);
    labService.addCharacteristic(DS18B20_SEND_CHR_UID);
    labService.addCharacteristic(DS18B20_NOTIFY_CHR_UID);
    labService.addCharacteristic(TCS34725_SEND_CHR_UID);
    labService.addCharacteristic(TCS34725_NOTIFY_CHR_UID);
    labService.addCharacteristic(MAGNET_SEND_CHR_UID);
    labService.addCharacteristic(MAGNET_NOTIFY_CHR_UID);
    labService.addCharacteristic(MAX31855K_SEND_CHR_UID);
    labService.addCharacteristic(MAX31855K_NOTIFY_CHR_UID);
    labService.addCharacteristic(BLUX_SEND_CHR_UID);
    labService.addCharacteristic(BLUX_NOTIFY_CHR_UID);
    labService.addCharacteristic(TDS_SEND_CHR_UID);
    labService.addCharacteristic(TDS_NOTIFY_CHR_UID);
    labService.addCharacteristic(PH_SEND_CHR_UID);
    labService.addCharacteristic(PH_NOTIFY_CHR_UID);
    labService.addCharacteristic(MPX57000P_SEND_CHR_UID);
    labService.addCharacteristic(MPX57000P_NOTIFY_CHR_UID);
    labService.addCharacteristic(VOLT_SEND_CHR_UID);
    labService.addCharacteristic(VOLT_NOTIFY_CHR_UID);
    labService.addCharacteristic(ACS712_SEND_CHR_UID);
    labService.addCharacteristic(ACS712_NOTIFY_CHR_UID);
    labService.addCharacteristic(IMU_SEND_CHR_UID);
    labService.addCharacteristic(IMU_NOTIFY_CHR_UID);
    labService.addCharacteristic(HTS_SEND_CHR_UID);
    labService.addCharacteristic(HTS_NOTIFY_CHR_UID);
    labService.addCharacteristic(LPS22HB_SEND_CHR_UID);
    labService.addCharacteristic(LPS22HB_NOTIFY_CHR_UID);
    labService.addCharacteristic(THERM_SEND_CHR_UID);
    labService.addCharacteristic(THERM_NOTIFY_CHR_UID);
    labService.addCharacteristic(STROBE_CHR_UID);
    BLE.addService(labService);
    BLE.setEventHandler(BLEConnected, BLEconnectHandler);
    BLE.setEventHandler(BLEDisconnected, BLEdisconnectHandler);
    DS18B20_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteDS18B20Handler);
    TCS34725_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteTCS34725Handler);
    MAGNET_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteMAGNETHandler);
    MAX31855K_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteMAX31855Handler);
    BLUX_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteBLUXV30Handler);
    TDS_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteTDSHandler);
    PH_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwritePHHandler);
    MPX57000P_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwritePRESHandler);
    VOLT_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteVOLTHandler);
    ACS712_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteCURRHandler);
    IMU_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteIMUHandler);
    HTS_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteHTS221Handler);
    LPS22HB_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteLPS22HBHandler);
    THERM_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteTHERMHandler);
    DS18B20_SEND_CHR_UID.setValue("");
    DS18B20_NOTIFY_CHR_UID.setValue("");
    TCS34725_SEND_CHR_UID.setValue("");
    TCS34725_NOTIFY_CHR_UID.setValue("");
    MAGNET_SEND_CHR_UID.setValue("");
    MAGNET_NOTIFY_CHR_UID.setValue("");
    MAX31855K_SEND_CHR_UID.setValue("");
    MAX31855K_NOTIFY_CHR_UID.setValue("");
    BLUX_SEND_CHR_UID.setValue("");
    BLUX_NOTIFY_CHR_UID.setValue("");
    TDS_SEND_CHR_UID.setValue("");
    TDS_NOTIFY_CHR_UID.setValue("");
    PH_SEND_CHR_UID.setValue("");
    PH_NOTIFY_CHR_UID.setValue("");
    MPX57000P_SEND_CHR_UID.setValue("");
    MPX57000P_NOTIFY_CHR_UID.setValue("");
    VOLT_SEND_CHR_UID.setValue("");
    VOLT_NOTIFY_CHR_UID.setValue("");
    ACS712_SEND_CHR_UID.setValue("");
    ACS712_NOTIFY_CHR_UID.setValue("");
    IMU_SEND_CHR_UID.setValue("");
    IMU_NOTIFY_CHR_UID.setValue("");
    HTS_SEND_CHR_UID.setValue("");
    HTS_NOTIFY_CHR_UID.setValue("");
    LPS22HB_SEND_CHR_UID.setValue("");
    LPS22HB_NOTIFY_CHR_UID.setValue("");
    THERM_SEND_CHR_UID.setValue("");
    THERM_NOTIFY_CHR_UID.setValue("");
    STROBE_CHR_UID.setValue(0x00);
    BLE.advertise();
#ifdef DEBUG_MODE
    Serial.println("DONE.");
  } else {
    Serial.println("ERROR");
  }
#else
  }
#endif
  sensMuxThread.start(analogSensorMux);   //starting sensors selection thread
  strobingThread.start(strobing);
}

void loop() {
  BLE.poll();
}

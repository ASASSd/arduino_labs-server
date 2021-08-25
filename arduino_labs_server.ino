//comment line below to disable debug mode
#define DEBUG_MODE

#define SOFTVERSION "v0.4.3-armhf-debug"

//    ANALOG magnetic includes    //

uint8_t magnetAnalogIn = A0;
uint16_t sensval = 0;
uint16_t bval = 0;
const float k = 100.0 / (3.3 / 2);
const float kfx = (3.3 / 2) / 512 * k;
float outval = 0;
uint32_t del_magnet = 1000;

//      BLE includes          //

#include <ArduinoBLE.h>
BLEService labService("2086C901-9167-4F23-8A7A-F514BD665227");
BLECharacteristic TCS34725_SEND_CHR_UID("B807621A-19E3-40E0-B4F5-AEDCFE28C7CA", BLERead | BLEWrite, 5, true);
BLECharacteristic TCS34725_NOTIFY_CHR_UID("48606B19-820D-48D0-91B8-DF6A4F8DCBD4", BLERead | BLENotify, 17, true);
BLECharacteristic DS18B20_SEND_CHR_UID("09EB425D-8627-4FCB-AEA8-638BCF3F73F7", BLERead | BLEWrite, 5, true);
BLECharacteristic DS18B20_NOTIFY_CHR_UID("8103F9B3-C91E-47CD-8634-4B7D8F4D018D", BLERead | BLENotify, 5, true);
BLECharacteristic MAGNET_SEND_CHR_UID("3B75281E-00A0-4424-84C5-4C549CC1AE82", BLERead | BLEWrite, 5, true);
BLECharacteristic MAGNET_NOTIFY_CHR_UID("EF8A1B0B-1005-4DAD-B49D-75F84488E52C", BLERead | BLENotify, 5, true);
BLECharacteristic MAX31855K_SEND_CHR_UID("2DAF3C9C-CABA-461C-9FBC-1839D6F4E5B9", BLERead | BLEWrite, 5, true);
BLECharacteristic MAX31855K_NOTIFY_CHR_UID("F449E6B7-FE1E-45FF-AEBB-DCFC914DEB42", BLERead | BLENotify, 5, true);
uint8_t ds18b20_n = 0, tcs34725_n = 0, magnet_n = 0, max31855_n = 0;
bool magnet_conn = false, voltage_conn = false;

//      DS18B20 includes      //

#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(7);  //for ds18b20
DallasTemperature dallas(&oneWire);
uint32_t del_ds18b20 = 1000;

//      TCS34725 includes     //

#include <Wire.h>
#include "Adafruit_TCS34725.h"
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
uint32_t del_tcs34725 = 1000;

//      MAX31855 includes     //

#include <SPI.h>
#include "Adafruit_MAX31855.h"
// Default connection is using software SPI, but comment and uncomment one of
// the two examples below to switch between software SPI and hardware SPI:

// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.
//#define MAXDO   3
//#define MAXCS   4
//#define MAXCLK  5

// initialize the Thermocouple
//Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

// Example creating a thermocouple instance with hardware SPI
// on a given CS pin.
const uint8_t MAXCS = 10;
Adafruit_MAX31855 thermocouple(MAXCS);

// Example creating a thermocouple instance with hardware SPI
// on SPI1 using specified CS pin.
//#define MAXCS   10
//Adafruit_MAX31855 thermocouple(MAXCS, SPI1);
uint32_t del_max31855 = 1000;

//     BAT3U TDS includes     //

//launching second (of two UARTs of Arduino nano 33 BLE) UART on any GPIO pins
UART mytds(digitalPinToPinName(9), digitalPinToPinName(10), NC, NC);
//command - data request from sensor
uint8_t command[] = {0xA0, 0x00, 0x00, 0x00, 0x00, 0xA0};
//msgs - where we store reply from sensor
uint8_t msgs[14];
uint8_t msg = 0x00;
int hexToDec (int MSB, int LSB) {
  int dec = 0;
  dec = (MSB << 8) | LSB;
  return dec;
}
uint32_t del_bat3u = 1000;

//       RTOS includes         //

#include <rtos.h>
#include <mbed.h>
using namespace mbed;
using namespace rtos;
Thread sensMuxThread;
Thread* max31855Thread;
Thread* tcs34725Thread;
Thread* ds18b20Thread;
Thread* A0Thread;
Thread* A1Thread;

//routine for thermocouple ---------------------------------------------------

void max31855() {
  thermocouple.begin();
  for (;;) {
    float c = thermocouple.readCelsius();
    uint8_t s[5];
    s[0] = 0x00;
    memcpy(&s[1], &c, sizeof(c));
#ifdef DEBUG_MODE
    Serial.print("Ambient Temp = ");
    Serial.println(thermocouple.readInternal());
    if (isnan(c)) {
      Serial.println("Something wrong with thermocouple!");
    } else {
      Serial.print("Thermocouple temp = ");
      Serial.println(c);
    }
#endif
    MAX31855K_NOTIFY_CHR_UID.writeValue(s, 5);
    ThisThread::sleep_for(del_max31855);
  }
}

//routine for ds18b20 temperature sensor -------------------------------------

void ds18b20() {
  dallas.begin();
  for (;;) {
    dallas.requestTemperatures();      //read temp from ds18b20
    uint16_t temp = (int)dallas.getTempCByIndex(0);
    uint8_t s[5];
    s[0] = 0x00;
    s[1] = (uint8_t)(temp & 0x00FF);
    s[2] = (uint8_t)((temp & 0xFF00) >> 8);
    s[3] = 0x00;
    s[4] = 0x00;
#ifdef DEBUG_MODE
    Serial.print("Initializing DS18B20 sensor... ");
    Serial.println("DONE.");
    Serial.print("Temperature is ");
    Serial.println(temp);
#endif
    DS18B20_NOTIFY_CHR_UID.writeValue(s, 5);
    ThisThread::sleep_for(del_ds18b20);
  }
}

//routine for colorimeter tcs34725 -------------------------------------------

void tcs34725() {
  tcs.begin();
  for (;;) {
    uint16_t r, g, b, c, colorTemp, lux;
    uint8_t s[17];
    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
    s[0] = 0x00;
    s[1] = (uint8_t)(r & 0x00FF);
    s[2] = (uint8_t)((r & 0xFF00) >> 8);
    s[3] = 0x00;
    s[4] = 0x00;
    s[5] = (uint8_t)(g & 0x00FF);
    s[6] = (uint8_t)((g & 0xFF00) >> 8);
    s[7] = 0x00;
    s[8] = 0x00;
    s[9] = (uint8_t)(b & 0x00FF);
    s[10] = (uint8_t)((b & 0xFF00) >> 8);
    s[11] = 0x00;
    s[12] = 0x00;
    s[13] = (uint8_t)(colorTemp & 0x00FF);
    s[14] = (uint8_t)((colorTemp & 0xFF00) >> 8);
    s[15] = 0x00;
    s[16] = 0x00;
#ifdef DEBUG_MODE
    Serial.print("Color Temp: "); Serial.print(colorTemp, DEC); Serial.print(" K - ");
    Serial.print("Lux: "); Serial.print(lux, DEC); Serial.print(" - ");
    Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
    Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
    Serial.print("B: "); Serial.print(b, DEC); Serial.print(" ");
    Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
    Serial.println(" ");
    Serial.println(r, HEX);
    Serial.println(s[1], HEX);
    Serial.println(s[2], HEX);
#endif
    TCS34725_NOTIFY_CHR_UID.writeValue(s, 17);
    ThisThread::sleep_for(del_tcs34725);
  }
}

//routine for BitAtom Bat3U TDS sensor ---------------------------------------

void bat3u() {
  //sending command
  mytds.write(command, 6);
  int i = 0;
  while (mytds.available()) {
    msg = mytds.read();
    //storing data in array
    msgs[i] = msg;
    i++;
  }
  int tds = hexToDec(msgs[9], msgs[10]);  //bytes 9 and 10 contain TDS
  int ntc = hexToDec(msgs[11], msgs[12]); //bytes 11 and 12 contain temperature
#ifdef DEBUG_MODE
  Serial.println();
  Serial.print("TDS value : ");
  Serial.print(tds);
  Serial.print(", temperature : ");
  //hotfix from chinese site
  Serial.println(ntc / 100.0);
#endif
}

//routine for analog magnetic sensor -----------------------------------------

void magnet() {
  pinMode(magnetAnalogIn, INPUT);
  for (;;) {
    sensval = analogRead(magnetAnalogIn);
    bval = sensval - 512;
    outval = bval * kfx;
    outval *= -1;
    uint8_t s[5];
    s[0] = 0x00;
    memcpy(&s[1], &outval, sizeof(outval));
#ifdef DEBUG_MODE
    Serial.print("Sensor raw value: ");
    Serial.print(sensval);
    Serial.print("\tMagnet value: ");
    Serial.println(outval);
#endif
    MAGNET_NOTIFY_CHR_UID.writeValue(s, 5);
    ThisThread::sleep_for(del_magnet);
  }
}

//routine for BLE handlers ---------------------------------------------------

void BLEconnectHandler(BLEDevice central) {
#ifdef DEBUG_MODE
  Serial.print("Connected central: ");
  Serial.println(central.address());
#endif
}

void BLEdisconnectHandler(BLEDevice central) {
#ifdef DEBUG_MODE
  Serial.print("Disconnected central: ");
  Serial.println(central.address());
#endif
}

void BLEwriteDS18B20Handler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("Write event");
#endif
  uint8_t n_before = ds18b20_n;     //remembering last _n value
  ds18b20_n = (uint8_t)DS18B20_SEND_CHR_UID.value()[0];   //to prevent exceptions
  if (ds18b20_n && !n_before) { //thread isn't running and we're launching it
    ds18b20Thread = new Thread;
    ds18b20Thread->start(ds18b20);
  } else if (!ds18b20_n && n_before) {  //thread is running and we're stopping it
    ds18b20Thread->terminate();
    delete ds18b20Thread;
  }
#ifdef DEBUG_MODE //code below isn't necessary, but it's good for debugging
  else if (!ds18b20_n && !n_before) { //we can't stop thread, if it isn't running
    Serial.println("!Illegal event: stopping non-existing thread of ds18b20!");
  } else {  //we can't run already running thread
    Serial.println("!Illegal event: launching another thread of ds18b20!");
  }
#endif
}

void BLEwriteTCS34725Handler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("Write event");
#endif
  uint8_t n_before = tcs34725_n;
  uint8_t s[5];
  for (int i = 0; i < 5; i++) {
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
    Serial.println("!Illegal event: stopping non-existing thread of tcs34725!");
  } else {
    Serial.println("!Illegal event: launching another thread of tcs34725!");
  }
  Serial.println(del_tcs34725, DEC);
#endif
}

void BLEwriteMAGNETHandler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("Write event");
#endif
  uint8_t n_before = magnet_n;
  magnet_n = (uint8_t)MAGNET_SEND_CHR_UID.value()[0];
  if (magnet_n && !n_before && magnet_conn) {
    if (magnetAnalogIn == A0) {
      A0Thread = new Thread;
      A0Thread->start(magnet);
    } else if (magnetAnalogIn == A1) {
      A1Thread = new Thread;
      A1Thread->start(magnet);
    }
  } else if (!magnet_n && n_before && magnet_conn) {
    if (magnetAnalogIn == A0) {
      A0Thread->terminate();
      delete A0Thread;
    } else if (magnetAnalogIn == A1) {
      A1Thread->terminate();
      delete A1Thread;
    }
  } else if (magnet_n && !n_before && !magnet_conn) {
    MAGNET_SEND_CHR_UID.setValue("");
    magnet_n = 0;
#ifdef DEBUG_MODE
    Serial.println("!Illegal event: sensor is not connected!");
  } else if (!magnet_n && !n_before) {
    Serial.println("!Illegal event: stopping non-existing thread of magnet!");
  } else if (magnet_n && n_before) {
    Serial.println("!Illegal event: launching another thread of magnet!");
  }
#else
  }
#endif
}

void BLEwriteMAX31855Handler(BLEDevice central, BLECharacteristic characteristic) {
#ifdef DEBUG_MODE
  Serial.println("Write event");
#endif
  uint8_t n_before = max31855_n;
  uint8_t s[5];
  for (int i = 0; i < 5; i++) {
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
    Serial.println("!Illegal event: stopping non-existing thread of tcs34725!");
  } else {
    Serial.println("!Illegal event: launching another thread of tcs34725!");
  }
  Serial.println(del_max31855, DEC);
#endif
}

//routine for analog sensor selection ----------------------------------------

void analogSensorMux() {
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  for (;;) {
    int _a0_sens_id = analogRead(A2), _a1_sens_id = analogRead(A3);
    if (460 < _a0_sens_id && _a0_sens_id < 540) {
      magnetAnalogIn = A0;
      magnet_conn = true;
    } else if (460 < _a1_sens_id && _a1_sens_id < 540) {
      magnetAnalogIn = A1;
      magnet_conn = true;
    } else {
      magnet_conn = false;
    }
    ThisThread::sleep_for(100);
  }
}

void setup() {
#ifdef DEBUG_MODE
  Serial.begin(9600);
  while (!Serial) delay(1);
  Serial.println();
  Serial.print("Testing software ");
  Serial.print(SOFTVERSION);
  Serial.println(" init... ");
  Serial.print("Starting BLE... ");
#endif
  if (BLE.begin()) {
    Serial.println("DONE.");
    Serial.print("Loading BLE name and service... ");
    BLE.setLocalName("LAB33");
    BLE.setAdvertisedService(labService);
    //here we should add characteristics to our service
    labService.addCharacteristic(DS18B20_SEND_CHR_UID);
    labService.addCharacteristic(DS18B20_NOTIFY_CHR_UID);
    labService.addCharacteristic(TCS34725_SEND_CHR_UID);
    labService.addCharacteristic(TCS34725_NOTIFY_CHR_UID);
    labService.addCharacteristic(MAGNET_SEND_CHR_UID);
    labService.addCharacteristic(MAGNET_NOTIFY_CHR_UID);
    labService.addCharacteristic(MAX31855K_SEND_CHR_UID);
    labService.addCharacteristic(MAX31855K_NOTIFY_CHR_UID);
    BLE.addService(labService);
    BLE.setEventHandler(BLEConnected, BLEconnectHandler);
    BLE.setEventHandler(BLEDisconnected, BLEdisconnectHandler);
    DS18B20_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteDS18B20Handler);
    TCS34725_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteTCS34725Handler);
    MAGNET_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteMAGNETHandler);
    MAX31855K_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteMAX31855Handler);
    DS18B20_SEND_CHR_UID.setValue("");
    DS18B20_NOTIFY_CHR_UID.setValue("");
    TCS34725_SEND_CHR_UID.setValue("");
    TCS34725_NOTIFY_CHR_UID.setValue("");
    MAGNET_SEND_CHR_UID.setValue("");
    MAGNET_NOTIFY_CHR_UID.setValue("");
    MAX31855K_SEND_CHR_UID.setValue("");
    MAX31855K_NOTIFY_CHR_UID.setValue("");
    BLE.advertise();
#ifdef DEBUG_MODE
    Serial.println("DONE.");
  } else {
    Serial.println("ERROR");
  }
#else
  }
#endif
  sensMuxThread.start(analogSensorMux);   //starting sensors hotplug thread
}

void loop() {
  BLE.poll();
  /*
     ds18b20Thread = new Thread;
     ds18b20Thread->start(thermocpl);
     while (ds18b20Thread->get_state() != Thread::Deleted);
     delete ds18b20Thread;
  */
}
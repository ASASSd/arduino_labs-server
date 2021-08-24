//comment line below to disable debug output
#define DEBUG_OUTPUT

//    ANALOG magnetic includes    //

int magnetAnalogIn = A0;
int sensval = 0;
int bval = 0;
const double k = 100.0 / (3.3 / 2);
const double kfx = (3.3 / 2) / 512 * k;
double outval = 0;
uint32_t del_magnet = 1000;

//      BLE includes          //

#include <ArduinoBLE.h>
BLEService labService("2086C901-9167-4F23-8A7A-F514BD665227");
BLECharacteristic TCS34725_SEND_CHR_UID("B807621A-19E3-40E0-B4F5-AEDCFE28C7CA", BLERead | BLEWrite, 5, true);
BLECharacteristic TCS34725_NOTIFY_CHR_UID("48606B19-820D-48D0-91B8-DF6A4F8DCBD4", BLERead | BLENotify, 17, true);
BLELongCharacteristic DS18B20_SEND_CHR_UID("09EB425D-8627-4FCB-AEA8-638BCF3F73F7", BLERead | BLEWrite);
BLEIntCharacteristic DS18B20_NOTIFY_CHR_UID("8103F9B3-C91E-47CD-8634-4B7D8F4D018D", BLERead | BLENotify);
BLELongCharacteristic MAGNET_SEND_CHR_UID("3B75281E-00A0-4424-84C5-4C549CC1AE82", BLERead | BLEWrite);
BLEIntCharacteristic MAGNET_NOTIFY_CHR_UID("EF8A1B0B-1005-4DAD-B49D-75F84488E52C", BLERead | BLENotify);
byte ds18b20_n = 0, tcs34725_n = 0, magnet_n = 0;
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
#define MAXCS   10
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
byte command[] = {0xA0, 0x00, 0x00, 0x00, 0x00, 0xA0};
//msgs - where we store reply from sensor
byte msgs[14];
byte msg = 0x00;
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
Thread* sensThread;

//routine for thermocouple ---------------------------------------------------

void max31855() {
  thermocouple.begin();
  for (;;) {
    Serial.print("Ambient Temp = ");
    Serial.println(thermocouple.readInternal());
    double c = thermocouple.readCelsius();

    if (isnan(c)) {
      Serial.println("Something wrong with thermocouple!");
    } else {
      Serial.print("Thermocouple temp = ");
      Serial.println(c);
    }
    ThisThread::sleep_for(del_max31855);
  }
}

//routine for ds18b20 temperature sensor -------------------------------------

void ds18b20() {
  for (;;) {
    Serial.print("Initializing DS18B20 sensor... ");
    dallas.begin();
    Serial.println("DONE.");
    dallas.requestTemperatures();              //read temp from ds18b20
    byte temp = (int)dallas.getTempCByIndex(0);
    DS18B20_NOTIFY_CHR_UID.setValue(temp);
    Serial.print("Temperature is ");
    Serial.println(DS18B20_NOTIFY_CHR_UID.value());
    ThisThread::sleep_for(del_ds18b20);
  }
}

//routine for colorimeter tcs34725 -------------------------------------------

void tcs34725() {
  for (;;) {
    tcs.begin();
    uint16_t r, g, b, c, colorTemp, lux;
    float rf, gf, bf;
    uint8_t s[9];
    tcs.getRawData(&r, &g, &b, &c);
    colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
    lux = tcs.calculateLux(r, g, b);
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
#ifdef DEBUG_OUTPUT
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
#ifdef DEBUG_OUTPUT
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
    MAGNET_NOTIFY_CHR_UID.setValue(outval);
#ifdef DEBUG_OUTPUT
    Serial.print("Sensor raw value: ");
    Serial.println(sensval);
    Serial.print("Magnet value: ");
    Serial.println(outval);
#endif
    ThisThread::sleep_for(del_magnet);
  }
}

//routine for BLE ------------------------------------------------------------

void BLEconnectHandler(BLEDevice central) {
  Serial.print("Connected central: ");
  Serial.println(central.address());
}

void BLEdisconnectHandler(BLEDevice central) {
  Serial.print("Disconnected central: ");
  Serial.println(central.address());
}

void BLEwriteDS18B20Handler(BLEDevice central, BLECharacteristic characteristic) {
  Serial.println("Write event");
  int n_before = ds18b20_n;     //remembering last _n value
  ds18b20_n = DS18B20_SEND_CHR_UID.value();   //to prevent exceptions
  if (ds18b20_n && !n_before) { //thread isn't running and we're launching it
    sensThread = new Thread;
    sensThread->start(ds18b20);
  } else if (!ds18b20_n && n_before) {  //thread is running and we're stopping it
    sensThread->terminate();
    delete sensThread;
  }
#ifdef DEBUG_OUTPUT //code below isn't necessary, but it's good for debugging
  else if (!ds18b20_n && !n_before) { //we can't stop thread, if it isn't running
    Serial.println("!Illegal event: stopping non-existing thread of ds18b20!");
  } else {  //we can't run already running thread
    Serial.println("!Illegal event: launching another thread of ds18b20!");
  }
#endif
}

void BLEwriteTCS34725Handler(BLEDevice central, BLECharacteristic characteristic) {
  Serial.println("Write event");
  int n_before = tcs34725_n;
  uint8_t s[9];
  for (int i = 0; i < 9; i++) {
    s[i] = TCS34725_SEND_CHR_UID.value()[i];
  }
  tcs34725_n = (int)s[0];
  del_tcs34725 = (uint32_t)(s[1] << 24) | (uint32_t)(s[2] << 16) | (uint32_t)(s[3] << 8) | (uint32_t)s[4];
  if (tcs34725_n && !n_before) {
    sensThread = new Thread;
    sensThread->start(tcs34725);
  } else if (!tcs34725_n && n_before) {
    sensThread->terminate();
    delete sensThread;
  }
#ifdef DEBUG_OUTPUT
  else if (!tcs34725_n && !n_before) {
    Serial.println("!Illegal event: stopping non-existing thread of tcs34725!");
  } else {
    Serial.println("!Illegal event: launching another thread of tcs34725!");
  }
  Serial.println(del_tcs34725, DEC);
#endif
}

void BLEwriteMAGNETHandler(BLEDevice central, BLECharacteristic characteristic) {
  Serial.println("Write event");
  int n_before = magnet_n;
  magnet_n = MAGNET_SEND_CHR_UID.value();
  if (magnet_n && !n_before && magnet_conn) {
    sensThread = new Thread;
    sensThread->start(magnet);
  } else if (!magnet_n && n_before && magnet_conn) {
    sensThread->terminate();
    delete sensThread;
  }
#ifdef DEBUG_OUTPUT
  else if (!magnet_n && !n_before && !magnet_conn) {
    Serial.println("!Illegal event: stopping non-existing thread of magnet!");
  } else if (magnet_n && !n_before && !magnet_conn) {
    Serial.println("!Illegal event: sensor not connected!");
    MAGNET_SEND_CHR_UID.setValue(0);
    magnet_n = 0;
  } else if (magnet_n && n_before && magnet_conn) {
    Serial.println("!Illegal event: launching another thread of magnet!");
  }
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
#ifdef DEBUG_OUTPUT
  Serial.begin(9600);
  while (!Serial) delay(1);
  Serial.println();
  Serial.println("Testing software v0.3.5 init... ");
#endif
  Serial.print("Starting BLE... ");
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
    BLE.addService(labService);
    BLE.setEventHandler(BLEConnected, BLEconnectHandler);
    BLE.setEventHandler(BLEDisconnected, BLEdisconnectHandler);
    DS18B20_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteDS18B20Handler);
    TCS34725_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteTCS34725Handler);
    MAGNET_SEND_CHR_UID.setEventHandler(BLEWritten, BLEwriteMAGNETHandler);
    DS18B20_SEND_CHR_UID.setValue(0);
    DS18B20_NOTIFY_CHR_UID.setValue(0);
    TCS34725_SEND_CHR_UID.setValue("");
    TCS34725_NOTIFY_CHR_UID.setValue("");
    MAGNET_SEND_CHR_UID.setValue(0);
    MAGNET_NOTIFY_CHR_UID.setValue(0);
    BLE.advertise();
    Serial.println("DONE.");
  } else {
    Serial.println("ERROR");
  }
  sensMuxThread.start(analogSensorMux);   //starting sensors hotplug thread
}

void loop() {
  BLE.poll();
  /*
     sensThread = new Thread;
     sensThread->start(thermocpl);
     while (sensThread->get_state() != Thread::Deleted);
     delete sensThread;
  */
}

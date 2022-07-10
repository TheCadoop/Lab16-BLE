/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/cademissner/Desktop/IoT/Lab16-BLE/src/Lab16-BLE.ino"
void setup();
void loop();
#line 1 "/Users/cademissner/Desktop/IoT/Lab16-BLE/src/Lab16-BLE.ino"
SYSTEM_MODE(MANUAL);
SYSTEM_THREAD(ENABLED);

#include "oled-wing-adafruit.h"

const size_t UART_TX_BUF_SIZE = 20;

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);

const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

OledWingAdafruit display;

bool connectMessageSent = false;
bool disconnect = false;

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
  for (size_t i = 0; i < len; i++) {
    Serial.println(data[i]);
  }

  if (data[0] == 49) {
    display.clearDisplay();
    display.setCursor(0, 5);
    display.println("1 Recieved");
    display.display();
  } else if (data[0] == 48) {
    display.clearDisplay();
    display.display();

    disconnect = true;
  }
}

void setup() {
  Serial.begin(9600);

  display.setup();
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 5);
  display.setTextColor(WHITE);
  display.display();

  RGB.control(true);

  BLE.on();

  BLE.addCharacteristic(txCharacteristic);
  BLE.addCharacteristic(rxCharacteristic);

  BleAdvertisingData data;
  data.appendServiceUUID(serviceUuid);
  data.appendLocalName("cade-ble");  // worked on first flash but doesnt anymore  
  BLE.advertise(&data);

  while (!Serial.isConnected()) {};
}

void loop() {
  display.display();

  Serial.println("hi");
  Serial.println(BLE.advertising());

  if (!BLE.connected()) {
    RGB.color(255, 255, 0);
    connectMessageSent = false;
  } else {
    if (disconnect) {
      BLE.disconnect();
      disconnect = false;
    } else {
      RGB.color(0, 140, 255);

      if (!connectMessageSent) {
        connectMessageSent = true;

        delay(3000);
        uint8_t txBuf[UART_TX_BUF_SIZE];
        String message = "Connected";
        message.toCharArray((char *)txBuf, message.length() + 1);
        txCharacteristic.setValue(txBuf, message.length() + 1);
      }
    }
  }
}
#include "HomeSpan.h"

// Definisci i pin per il sensore di apertura/chiusura porta e la sirena
const int pinDoorSensor = 15;  // Cambia questo valore con il pin corretto
const int pinRelay = 13;       // Cambia questo valore con il pin corretto

// Classe per il sensore di porta
struct DoorSensor : Service::ContactSensor {
  SpanCharacteristic *contactState;

  DoorSensor() : Service::ContactSensor() {
    contactState = new Characteristic::ContactSensorState();
    pinMode(pinDoorSensor, INPUT_PULLUP);  // Configura il pin con pull-up interno
  }

  void loop() {
    int state = digitalRead(pinDoorSensor);
    contactState->setVal(state == HIGH ? 1 : 0);  // 1 = aperto, 0 = chiuso
  }
};

// Classe per il controllo del relè (sirena)
struct Siren : Service::Switch {
  SpanCharacteristic *onState;

  Siren() : Service::Switch() {
    onState = new Characteristic::On();
    pinMode(pinRelay, OUTPUT);
    digitalWrite(pinRelay, LOW);  // Assicurati che il relè sia spento all'avvio
  }

  boolean update() override {
    bool isOn = onState->getNewVal();
    digitalWrite(pinRelay, isOn ? HIGH : LOW);
    return true;  // Indica che l'aggiornamento è andato a buon fine
  }
};

void setup() {
  Serial.begin(115200);
  homeSpan.setPairingCode("11122333");
  homeSpan.setQRID("111-22-333");
  homeSpan.setWifiCredentials("WIFI_NAME","WIFI_PASSWORD"); // change credentials
  homeSpan.begin(Category::Bridges, "HomeSpan Door Sensor and Siren");


  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Name("Door Sensor");
  new Characteristic::Manufacturer("Your Company");
  new Characteristic::SerialNumber("123-456-789");
  new Characteristic::Model("DS-1");
  new Characteristic::FirmwareRevision("1.0");
  new Characteristic::Identify();

  new DoorSensor();

  new SpanAccessory();
  new Service::AccessoryInformation();
  new Characteristic::Name("Siren");
  new Characteristic::Manufacturer("Your Company");
  new Characteristic::SerialNumber("987-654-321");
  new Characteristic::Model("SR-1");
  new Characteristic::FirmwareRevision("1.0");
  new Characteristic::Identify();

  new Siren();
}

void loop() {
  homeSpan.poll();
}

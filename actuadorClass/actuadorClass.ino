#include <WiFi.h>

class WiFiClientHandler {
private:
  const char * ssid;
  const char * password;
  const char * serverAddress;
  int serverPort;
  WiFiClient client;

public:
  WiFiClientHandler(const char * ssid, const char * password, const char * serverAddress, int serverPort) 
    : ssid(ssid), password(password), serverAddress(serverAddress), serverPort(serverPort) {}

  bool connectWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println(".");
    }
    Serial.println("WiFi connection established");
    return true;
  }

  bool connectServer() {
    if (client.connect(serverAddress, serverPort)) {
      Serial.println("Successful connection to server");
      return true;
    } else {
      Serial.println("Connection failed");
      return false;
    }
  }

  String fetchData() {
    client.print("GET");
    delay(100);
    String response = client.readString();
    client.stop();
    return response;
  }
};

class Led {
private:
  int pin;

public:
  Led(int pin) : pin(pin) {}

  void initialize() {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  void turnOn() {
    digitalWrite(pin, HIGH);
  }

  void turnOff() {
    digitalWrite(pin, LOW);
  }
};

WiFiClientHandler wifiHandler("GalaxyA54", "72726315", "192.168.137.165", 8000);

const int LED_GREEN = 33;
const int LED_YELLOW = 32;
const int LED_RED = 25;

Led greenLED(LED_GREEN);
Led yellowLED(LED_YELLOW);
Led redLED(LED_RED);

void updateLedsState(const String &value) {
  if (value == "RED") {
    greenLED.turnOff();
    redLED.turnOn();
    yellowLED.turnOff();
  } else if (value == "YELLOW") {
    greenLED.turnOff();
    redLED.turnOff();
    yellowLED.turnOn();
  } else if (value == "GREEN") {
    greenLED.turnOn();
    redLED.turnOff();
    yellowLED.turnOff();
  }
}

void setup() {
  Serial.begin(115200);

  greenLED.initialize();
  yellowLED.initialize();
  redLED.initialize();

  wifiHandler.connectWiFi();
}

void loop() {
  if (wifiHandler.connectServer()) {
    String value = wifiHandler.fetchData();
    
    Serial.print("Received value: ");
    Serial.println(value);

    updateLedsState(value);
  }
  delay(2000);
}

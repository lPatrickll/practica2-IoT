#include <WiFi.h>

#define TRIG_PIN 4
#define ECHO_PIN 2

class Ultrasonic {
private:
  int triggerPin;
  int echoPin;

public:
  Ultrasonic(int triggerPin, int echoPin) : triggerPin(triggerPin), echoPin(echoPin) {
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
  }

  float measureDistance() {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    float distance_cm = 0.01723 * duration; // Convert time to distance in cm
    return distance_cm;
  }
};

class WiFiClientHandler {
private:
  const char *ssid;
  const char *password;
  const char *serverAddress;
  int serverPort;
  float distance;

public:
  WiFiClientHandler(const char *ssid, const char *password, const char *serverAddress, int serverPort)
      : ssid(ssid), password(password), serverAddress(serverAddress), serverPort(serverPort) {}

  bool connectWiFi() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    Serial.println("\nWiFi connection established");
    Serial.print("Local IP address: ");
    Serial.println(WiFi.localIP());
    return true;
  }

  bool connectServer() {
    WiFiClient client;
    if (client.connect(serverAddress, serverPort)) {
      Serial.println("Successful server connection");
      return true;
    } else {
      Serial.println("Connection failed");
      return false;
    }
  }

  void sendData(float distance) {
    WiFiClient client;
    if (client.connect(serverAddress, serverPort)) {
      Serial.println("Connection successful.");
      String response = "distance=";
      response += distance;
      client.print(response);
      client.stop();
    } else {
      Serial.println("Connection failed.");
    }
  }

  void updateDistance(float newDistance) {
    distance = newDistance;
  }

  float getDistance() {
    return distance;
  }
};

Ultrasonic ultrasonicSensor(TRIG_PIN, ECHO_PIN);
WiFiClientHandler wifiHandler("TP-Link_65A0", "-8690478_P@.trick", "192.168.0.158", 8000);

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to WiFi: ");
  wifiHandler.connectWiFi();
}

void loop() {
  float distance = ultrasonicSensor.measureDistance();
  Serial.print("distance: ");
  Serial.println(distance);

  wifiHandler.updateDistance(distance);
  wifiHandler.sendData(distance);

  delay(2000);
}

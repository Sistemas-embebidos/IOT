#include <Arduino.h>

#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>

#include <MPU9250_asukiaaa.h>
#include <MQTTClient.h>

#define FIREBASE_HOST "https://gicsafe-cdp.firebaseio.com/"
#define FIREBASE_AUTH "JzjC8HuWCp2xvqE37n71488Gv9XuWQkRTsFsnnl5"
#define WIFI_SSID "PruebaTBA"
#define WIFI_PASSWORD "pruebaTBA"

#define SDA_PIN 21
#define SCL_PIN 22

MQTTClient client;


#define SECRET_CH_ID 875856      
#define SECRET_WRITE_APIKEY "IDDX6EA2VCAUIOPD"

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

#include "Configuracion.h"
#include "ThingSpeak.h"

#define TERMINAL 1
#define APP 1
#define FIREBASE 0
#define THINGSPEAK 0
#define MQTT_ 0

FirebaseData firebaseData;

MPU9250_asukiaaa mySensor;

float aX, aY, aZ, aSqrt;
float gX, gY, gZ;
float mDirection;
uint16_t mX, mY, mZ;
int T;

WiFiClient wifiClient;

uint64_t sendEntry = 0;
uint32_t times = 1;
uint32_t ack = 0;

String path = "/Embebidos/prueba";
String jsonStr = "";

void inline handler (void);


void setup() {
  Wire.begin(D6, D5); /* join i2c bus with SDA=D1 and SCL=D2 of NodeMCU */

  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();

  initSerial();
  //initWiFi();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  if (FIREBASE == 1) {
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
  
    //Set database read timeout to 1 minute (max 15 minutes)
    Firebase.setReadTimeout(firebaseData, 1000 * 60);
    //tiny, small, medium, large and unlimited.
    //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
    Firebase.setwriteSizeLimit(firebaseData, "tiny");
  }
  if (THINGSPEAK == 1) {
    ThingSpeak.begin(wifiClient);
  }
  delay(2 * 1000);

  Serial.println("MQTT > Conectando a broker");
  client.begin("broker.shiftr.io", wifiClient);
  connect();

  randomSeed(analogRead(0));

  delay(10);
}

void inline handler (void) {

  uint32_t heap = ESP.getFreeHeap();
  //Serial.printf("<%u>\r\n",ESP.getFreeHeap());
  if (heap < 2500)
  {
    Serial.printf("<%u>\r\n", ESP.getFreeHeap());
    Serial.println("Cantidad de heap critico! desconectando MQTT");
    //mqttClient.disconnect();
    //  Serial.printf("<%u>\r\n",ESP.getFreeHeap());
  }

  timer0_write(ESP.getCycleCount() + timer0_preload * my_delay);
}

void connect() {
  while (!client.connect("GICSAFE", "fb033abd", "e2ebc570ab73e0b5")) {
    Serial.print(".");
  }

  Serial.println("MQTT > Conectado!");
  client.subscribe("CDP");
}

void MQTT_send(void) {

  client.publish("Temperatura", String(T));
  client.publish("Magnetometro/m_x", String(mX));
  client.publish("Magnetometro/m_y", String(mY));
  client.publish("Magnetometro/m_z", String(mZ));
  client.publish("Giroscopio/g_x", String(gX));
  client.publish("Giroscopio/g_y", String(gY));
  client.publish("Giroscopio/g_z", String(gZ));
  client.publish("Aceleracion/a_x", String(aX));
  client.publish("Aceleracion/a_y", String(aY));
  client.publish("Aceleracion/a_z", String(aZ));

  Serial.print("MQTT > Publicado!");
}

void Thingspeak_send(void) {
  ThingSpeak.setField(1, T);
  ThingSpeak.setField(2, mX);
  ThingSpeak.setField(3, gZ);
  ThingSpeak.setField(4, aY);

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("ThingSpeak > Enviado");
  }
  else {
    Serial.println("ThingSpeak > ERROR " + String(x));
  }
}

void Firebase_send(void) {

  Firebase.setDouble(firebaseData, path + "/a_x", aX);
  Firebase.setDouble(firebaseData, path + "/a_y", aY);
  Firebase.setDouble(firebaseData, path + "/a_z", aZ);
  Firebase.setDouble(firebaseData, path + "/g_x", gX);
  Firebase.setDouble(firebaseData, path + "/g_y", gY);
  Firebase.setDouble(firebaseData, path + "/g_z", gZ);
  Firebase.setDouble(firebaseData, path + "/m_x", mX);
  Firebase.setDouble(firebaseData, path + "/m_y", mY);
  Firebase.setDouble(firebaseData, path + "/m_z", mZ);
  Firebase.setDouble(firebaseData, path + "/Temperatura", T);
  Firebase.setDouble(firebaseData, path + "/Times", times);

}

void imprimir_aceleracion(void) {
  Serial.println("a_x: " + String(aX));
  Serial.println("a_y: " + String(aY));
  Serial.println("a_z: " + String(aZ));
  Serial.println();
}

void crear_dato(void) {

  if (mySensor.accelUpdate() == 0) {
    aX = mySensor.accelX();
    aY = mySensor.accelY();
    aZ = mySensor.accelZ();
    aSqrt = mySensor.accelSqrt();
  }
  if (mySensor.gyroUpdate() == 0) {
    gX = mySensor.gyroX();
    gY = mySensor.gyroY();
    gZ = mySensor.gyroZ();
  }
  if (mySensor.magUpdate() == 0) {
    mX = mySensor.magX();
    mY = mySensor.magY();
    mZ = mySensor.magZ();
    mDirection = mySensor.magHorizDirection();
  }



  T = random(25, 30);



  Serial.printf("%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|", gX, gY, gZ, aX, aY, aZ, mX, mY);
  Serial.println(T);
}

void leer_aceleracion(void) {

  if (mySensor.accelUpdate() == 0) {
    aX = mySensor.accelX();
    aY = mySensor.accelY();
    aZ = mySensor.accelZ();
    aSqrt = mySensor.accelSqrt();
  } else {
    Serial.println("No se pudo leer el acelerometro");
  }
}

void imprimir_giroscopio(void) {
  Serial.println("g_x: " + String(gX));
  Serial.println("g_y: " + String(gY));
  Serial.println("g_z: " + String(gZ));
  Serial.println();
}

void leer_giroscopio(void) {

  if (mySensor.gyroUpdate() == 0) {
    gX = mySensor.gyroX();
    gY = mySensor.gyroY();
    gZ = mySensor.gyroZ();
  } else {
    Serial.println("No se pudo leer el giroscopio");
  }
}

void imprimir_magnetometro(void) {
  Serial.println("m_x: " + String(mX));
  Serial.println("m_y: " + String(mY));
  Serial.println("m_z: " + String(mZ));
  Serial.println("Direccion horizontal: " + String(mDirection));
  Serial.println();
}

void leer_magnetometro(void) {

  if (mySensor.magUpdate() == 0) {
    mX = mySensor.magX();
    mY = mySensor.magY();
    mZ = mySensor.magZ();
    mDirection = mySensor.magHorizDirection();
  } else {
    Serial.println("No se pudo leer el magnetometro");
  }
}

void loop() {

  client.loop();
  if (!client.connected()) {
    connect();
  };

  checkList();    // Handle devices list

  if (millis() - sendEntry > sendtime_var * 1000) {
    sendEntry = millis();
    Calculate();
    leer_aceleracion();
    leer_giroscopio();
    leer_magnetometro();

    if (TERMINAL == 1) {
      imprimir_aceleracion();
      imprimir_giroscopio();
      imprimir_magnetometro();
    }
    if (APP == 1) {
      crear_dato();
    }
    if (FIREBASE == 1) {
      Firebase_send();
      times++;
    }
    if (THINGSPEAK == 1) {
      Thingspeak_send();
    }
    if (MQTT_ == 1) {
      MQTT_send();
    }



  }


}



void Calculate() {
  uint32_t data;

  data = random(0, 10);
  if (TERMINAL == 1) {
    Serial.println("---------| " + String(data) + " |--------");
  }
}


void checkList() {

  uint16_t i;

  // Check console and print list if \n char is detected
  char c;
  while (Serial.available() > 0) {
    c = Serial.read();
    if (c == '1') {
      if (TERMINAL == 1) {
        Serial.println("Encender Led 1");
      }
      return;
    }
    if (c == '2') {
      if (TERMINAL == 1) {
        Serial.println("Encender Led 2");
      }
      return;
    }
    if (c == '3') {
      if (TERMINAL == 1) {
        Serial.println("Encender Led 3");
      }
      return;
    }
    if (c == '4') {
      Serial.println("Encender Led 4");
      return;
    }
    if (c == '5') {
      Serial.println("Hola mundoooo!");
      return;
    }
  }
}

String getTime() {
  String date = "";
  uint32_t timeNow = millis();

  uint16_t days = timeNow / day ;
  uint8_t hours = (timeNow % day) / hour;
  uint8_t minutes = ((timeNow % day) % hour) / minute ;
  uint8_t seconds = (((timeNow % day) % hour) % minute) / second;

  date += (days < 10) ? ('0' + String(days)) : (String(days));
  date += ':';
  date += (hours < 10) ? ('0' + String(hours)) : (String(hours));
  date += ':';
  date += (minutes < 10) ? ('0' + String(minutes)) : (String(minutes));
  date += ':';
  date += (seconds < 10) ? ('0' + String(seconds)) : (String(seconds));

  return date;
}

void initSerial() {
  Serial.begin(115200);
  String resetInfo = ESP.getResetReason();

  if (TERMINAL == 1) {
    Serial.setDebugOutput(SERIAL_SET_DEBUG_OUTPUT);
    Serial.printf("\n\n     %s v%s\n\n", ESP_NAME, VERSION);
    Serial.println("********** Crashes ***********");
    Serial.printf("Reset Reason:       %s\n", resetInfo.c_str());
    Serial.println("********** Firmware ***********");
    Serial.printf("STA MAC:            %s\n", WiFi.macAddress().c_str());
    Serial.printf("AP MAC:             %s\n", WiFi.softAPmacAddress().c_str());
    Serial.printf("Chip ID:            %6X\n", ESP.getChipId());
    Serial.printf("Sketch size:        %s\n", prettyBytes(ESP.getSketchSize()).c_str());
    Serial.printf("Free Sketch size:   %s\n", prettyBytes(ESP.getFreeSketchSpace()).c_str());
    Serial.printf("Chip size:          %s\n", prettyBytes(ESP.getFlashChipRealSize()).c_str());
    Serial.printf("Heap size:          %s\n", prettyBytes(ESP.getFreeHeap()).c_str());
    Serial.printf("SDK version:        %s\n", ESP.getSdkVersion());
    Serial.printf("CPU frequency:      %d MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("Boot Mode           %u\n", ESP.getBootMode());
    Serial.printf("Boot Version:       %u\n", ESP.getBootVersion());
    Serial.println("******** Configuration *********");
    Serial.printf("Max devices:        %d\n", MAX_DEVICES);
    Serial.printf("Packet size:        %d\n", chop_var);
    Serial.printf("Min RSSI:           %d\n", min_rssi_var);
    Serial.printf("Max time:           %d segundos\n", list_timeout_var);
    Serial.printf("Min seen:           %d veces\n", reported_var);
    Serial.printf("Send each:          %d segundos\n", sendtime_var);
    Serial.println("************ Train  ************");
    Serial.printf("Line:               %s\n", String(lines[LINE]).c_str());
    Serial.printf("Formation:          %d\n", TRAIN);
    Serial.printf("Car:                %d\n", CAR);
    Serial.println("*******************************");
  }
}

uint32_t getUptimeSecs() {
  static uint32_t uptime = 0;
  static uint32_t previousMillis = 0;
  uint32_t now = millis();

  uptime += (now - previousMillis) / 1000UL;
  previousMillis = now;
  return uptime;
}

String prettyBytes(uint32_t bytes) {

  const char *suffixes[7] = {"B", "KB", "MB", "GB", "TB", "PB", "EB"};
  uint8_t s = 0;
  double count = bytes;

  while (count >= 1024 && s < 7) {
    s++;
    count /= 1024;
  }
  if (count - floor(count) == 0.0) {
    return String((int) count) + suffixes[s];
  } else {
    return String(round(count * 10.0) / 10.0, 1) + suffixes[s];
  };
}

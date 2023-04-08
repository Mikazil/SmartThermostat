#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <DHT.h>
#include <FastBot.h>
#include "EspMQTTClient.h"

unsigned long timing;
EspMQTTClient client(
  "Official WIFI",
  "25102009",
  "mqtt.dealgate.ru",  // MQTT Broker server ip
  "mikazil",           // Can be omitted if not needed
  "32740238",          // Can be omitted if not needed
  "TestClient"         // Client name that uniquely identify your device
);

//Переменные
#define DHTPIN 4
float temperature, humidity;

DHT dht(DHTPIN, DHT11);
const char* ssid = "Official WIFI";
const char* password = "25102009";


ESP8266WebServer server(80);
FastBot bot("6014815330:AAFla1qhdPpp87zKIKyhx-KUxLpqHbaQp6E");


void setup() {

  Serial.begin(115200);
  delay(100);
  dht.begin();
  setupwlan();


  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");

  bot.attach(newMsg);
}

void onConnectionEstablished() {
  client.publish("climat/temp", String(dht.readTemperature()));
  client.publish("climat/hum", String(dht.readHumidity()));

}

void newMsg(FB_msg& msg) {
  if (msg.text == "/openmenu") bot.showMenu("Показать температуру", msg.chatID);
  else if (msg.text == "Показать температуру") {
    bot.sendMessage("Температура дома: " + String(temperature) + "°C", msg.chatID);
    bot.sendMessage("Влажность дома: " + String(humidity) + "%", msg.chatID);
  } else {
    bot.sendMessage("Чумба, я только учусь, а ты такое сложное просишь. Давай ограничимся командой \"Показать температуру\"", msg.chatID);
  }
}

void loop() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  server.handleClient();
  bot.tick();
  client.loop();
}

void handle_OnConnect() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  server.send(200, "text/html", SendHTML(temperature, humidity));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float temperature, float humidity) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Weather Station</title>\n";
  ptr += "<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap-icons@1.10.2/font/bootstrap-icons.css\" integrity=\"sha384-b6lVK+yci+bfDmaY1u0zE8YYJt0TZxLEAFyYSLHId4xoVvsrQu3INevFKo+Xir8e\" crossorigin=\"anonymous\">\n";
  //CSS STYLE
  ptr += "<style>@import url('https://fonts.googleapis.com/css2?family=Ubuntu:wght@300;400&display=swap');\n";
  ptr += "html { font-family: 'Ubuntu', sans-serif; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{height: 100vh; background-color: #dcebf3;} h1 {color: #444444;margin: 50px auto 30px; font-weight: 400;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px; font-weight: 300;}\n";
  ptr += ".container {  display: flex;  flex-direction: row;  justify-content: space-between;  align-items: stretch;  height: 200px;}\n";
  ptr += "div.temp{ flex: 0 0 47%;  background-color: #ffffff;  width: 25vh; height: 25vh; position: relative; border-radius: 35px;}\n";
  ptr += "div.hum{  flex: 0 0 47%;  background-color: #ffffff;  width: 25vh; height: 25vh;  position: relative; border-radius: 35px;}\n";
  ptr += "</style>\n";
  //ПРОДОЛЖЕНИЕ ДОКУМЕНТА
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP8266 Weather Station</h1>\n";

  //Блоки с температурой
  ptr += "<div class=\"container\" id=\"webpage\">\n";

  //Температура
  ptr += "<div class=\"temp\">\n";
  ptr += "<div style=\"margin-top: 10%; font-size: 64px;\"><i class=\"bi bi-thermometer\"></i></div>\n";
  ptr += "<p>";
  ptr += temperature;
  ptr += "&deg;C</p>";
  ptr += "<a style=\"font-size: 12px;\"> Temperature </a>\n";
  ptr += "</div>\n";

  //Влажность
  ptr += "<div class=\"hum\">\n";
  ptr += "<div style=\"margin-top: 10%; font-size: 64px;\"><i class=\"bi bi-droplet\"></i></div>\n";
  ptr += "<p>";
  ptr += humidity;
  ptr += "%</p>";
  ptr += "<a style=\"font-size: 12px;\"> Humidity </a>\n";
  ptr += "</div>\n";

  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

void setupwlan() {
  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");
  Serial.println(WiFi.localIP());
}

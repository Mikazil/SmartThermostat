#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <DHT.h>
#include <FastBot.h>
#define DHTPIN 4
float temperature, humidity;
int grass;
DHT dht(DHTPIN, DHT11);
const char* ssid = "Official WIFI";
const char* password = "25102009";
ESP8266WebServer server(80);
FastBot bot("6014815330:AAFla1qhdPpp87zKIKyhx-KUxLpqHbaQp6E");
bool led = false;
void setup() {
  Serial.begin(115200);
  delay(100);
  dht.begin();
  setupwlan();
  pinMode(D4, OUTPUT);
  server.on("/", handle_OnConnect);
  server.on("/led", handle_OnConnectled);
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP server started");
  bot.attach(newMsg);

}

void newMsg(FB_msg& msg) {
  Serial.println(msg.toString());
  if (msg.text == "/openmenu") bot.showMenu("Показать температуру", msg.chatID);
  else if (msg.text == "Показать температуру") {
    bot.sendMessage("Температура дома: " + String(temperature) + "°C", msg.chatID);
    bot.sendMessage("Влажность дома: " + String(humidity) + "%", msg.chatID);
    bot.sendMessage("Влажность почвы: " + String(map(analogRead(0), 1024, 280, 0, 100)) + "%", msg.chatID);
    if (map(analogRead(0), 1024, 280, 0, 100) <= 35) {
      bot.sendMessage("Необходимо полить цветок!", msg.chatID);
    }
  } else {
    bot.sendMessage("Чумба, я только учусь, а ты такое сложное просишь. Давай ограничимся командой \"Показать температуру\"", msg.chatID);
  }
}

void loop() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  server.handleClient();
  bot.tick();
}

void handle_OnConnect() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  server.send(200, "text/html", SendHTML(temperature, humidity));
}

void ledcontrol() {
  if (led){
    digitalWrite(D4, LOW);
    led = false;
    }
  else{
    digitalWrite(D4, HIGH);
    led = true;
  }   
}

void handle_OnConnectled() {
  ledcontrol();
  server.send(200, "text/html"); 
  }

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float temperature, float humidity) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Weather Station</title>\n";
  ptr += "<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap-icons@1.10.2/font/bootstrap-icons.css\" integrity=\"sha384-b6lVK+yci+bfDmaY1u0zE8YYJt0TZxLEAFyYSLHId4xoVvsrQu3INevFKo+Xir8e\" crossorigin=\"anonymous\">\n";
  ptr += "<link rel=\"apple-touch-icon\" sizes=\"180x180\" href=\"https://raw.githubusercontent.com/Mikazil/SmartThermostat/main/apple-touch-icon.png\">\n";
  ptr += "<meta name=\"apple-mobile-web-app-title\" content=\"Smart Temp\">\n";
  ptr += "<meta name=\"application-name\" content=\"Smart Temp\">\n";
  ptr += "<link rel=\"icon\" type=\"image/png\" sizes=\"32x32\" href=\"https://github.com/Mikazil/SmartThermostat/blob/main/favicon-32x32.png?raw=true\">\n";
  ptr += "<link rel=\"icon\" type=\"image/png\" sizes=\"16x16\" href=\"https://github.com/Mikazil/SmartThermostat/blob/main/favicon-16x16.png?raw=true\">\n";
  //CSS STYLE
  ptr += "<style>@import url('https://fonts.googleapis.com/css2?family=Roboto:wght@100;300;400;500;700;900&display=swap');\n";
  ptr += "html { font-family: 'Roboto', sans-serif; display: inline-block; margin: 0px auto;}\n";
  ptr += "body{height: 110vh; background-color: #dcebf3;} h1 {color: #444444;margin: 50px auto 30px; font-weight: 400;}\n";
  ptr += "h1{text-align: left;  font-size: 42px;  padding-left: 20px; padding-bottom: 15%;}\n";
  ptr += "p {font-size: 21px;color: #444444;margin-bottom: 10px; font-weight: 400; width: 100%;}\n";
  ptr += ".container1 {width: 51px;  height: 31px; position: relative;}\n";
  ptr += ".checkbox1 {opacity: 0; width: 0; height: 0; position: absolute;}\n";
  ptr += ".switch1 {width: 100%; height: 100%; display: block; background-color: #e9e9eb; border-radius: 16px; cursor: pointer; transition: all 0.2s ease-out;}\n";
  ptr += ".slider1 {width: 27px; height: 27px; position: absolute; left: calc(50% - 27px/2 - 10px); top: calc(50% - 27px/2); border-radius: 50%; background: #FFFFFF; box-shadow: 0px 3px 8px rgba(0, 0, 0, 0.15), 0px 3px 1px rgba(0, 0, 0, 0.06); transition: all 0.2s ease-out; cursor: pointer;}\n";
  ptr += ".checkbox1:checked + .switch1 {background-color: #34C759;}\n";
  ptr += ".checkbox1:checked + .switch1 .slider1 {left: calc(50% - 27px/2 + 10px); top: calc(50% - 27px/2);}\n";

  ptr += ".container2 {width: 51px;  height: 31px; position: relative;}\n";
  ptr += ".checkbox2 {opacity: 0; width: 0; height: 0; position: absolute;}\n";
  ptr += ".switch2 {width: 100%; height: 100%; display: block; background-color: #e9e9eb; border-radius: 16px; cursor: pointer; transition: all 0.2s ease-out;}\n";
  ptr += ".slider2 {width: 27px; height: 27px; position: absolute; left: calc(50% - 27px/2 - 10px); top: calc(50% - 27px/2); border-radius: 50%; background: #FFFFFF; box-shadow: 0px 3px 8px rgba(0, 0, 0, 0.15), 0px 3px 1px rgba(0, 0, 0, 0.06); transition: all 0.2s ease-out; cursor: pointer;}\n";
  ptr += ".checkbox2:checked + .switch2 {background-color: #34C759;}\n";
  ptr += ".checkbox2:checked + .switch2 .slider2 {left: calc(50% - 27px/2 + 10px); top: calc(50% - 27px/2);}\n";    

  ptr += ".container3 {width: 51px;  height: 31px; position: relative;}\n";
  ptr += ".checkbox3 {opacity: 0; width: 0; height: 0; position: absolute;}\n";
  ptr += ".switch3 {width: 100%; height: 100%; display: block; background-color: #e9e9eb; border-radius: 16px; cursor: pointer; transition: all 0.2s ease-out;}\n";
  ptr += ".slider3 {width: 27px; height: 27px; position: absolute; left: calc(50% - 27px/2 - 10px); top: calc(50% - 27px/2); border-radius: 50%; background: #FFFFFF; box-shadow: 0px 3px 8px rgba(0, 0, 0, 0.15), 0px 3px 1px rgba(0, 0, 0, 0.06); transition: all 0.2s ease-out; cursor: pointer;}\n";
  ptr += ".checkbox3:checked + .switch3 {background-color: #34C759;}\n";
  ptr += ".checkbox3:checked + .switch3 .slider3 {left: calc(50% - 27px/2 + 10px); top: calc(50% - 27px/2);}\n";  
  
  ptr += ".container4 {width: 51px;  height: 31px; position: relative;}\n";
  ptr += ".checkbox4 {opacity: 0; width: 0; height: 0; position: absolute;}\n";
  ptr += ".switch4 {width: 100%; height: 100%; display: block; background-color: #e9e9eb; border-radius: 16px; cursor: pointer; transition: all 0.2s ease-out;}\n";
  ptr += ".slider4 {width: 27px; height: 27px; position: absolute; left: calc(50% - 27px/2 - 10px); top: calc(50% - 27px/2); border-radius: 50%; background: #FFFFFF; box-shadow: 0px 3px 8px rgba(0, 0, 0, 0.15), 0px 3px 1px rgba(0, 0, 0, 0.06); transition: all 0.2s ease-out; cursor: pointer;}\n";
  ptr += ".checkbox4:checked + .switch4 {background-color: #34C759;}\n";
  ptr += ".checkbox4:checked + .switch4 .slider4 {left: calc(50% - 27px/2 + 10px); top: calc(50% - 27px/2);}\n";  

  ptr += "div.grass{height: 50px;  background: white;  border-radius: 13px;  display: flex; margin-bottom: 1.5%;}\n";
  ptr += "div.block{background-color: white; border-radius: 15px; width: 49%; margin-bottom: 2%; height: 16vh; display: flex; justify-content: space-between; text-align: left; flex-wrap: wrap;}\n";  
  ptr += "div.blocks{display: flex; display: flex; justify-content: space-between; text-align: center; flex-wrap: wrap; margin-top: 12%;}\n";
  ptr += "</style>\n";

  //ПРОДОЛЖЕНИЕ ДОКУМЕНТА
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP8266 Weather Station</h1>\n";

  //Блоки с температурой
  //ptr += "<div class=\"container\" id=\"webpage\">\n";

  //Температура старая
  /*
  ptr += "<div class=\"temp\">\n";
  ptr += "<div style=\"margin-top: 10%; font-size: 32px;\"><i class=\"bi bi-thermometer\"></i></div>\n";
  ptr += "<p>";
  ptr += temperature;
  ptr += "&deg;C</p>";
  ptr += "<a style=\"font-size: 12px;\"> Temperature </a>\n";
  ptr += "</div>\n";*/

  //температура новая
  ptr += "<div class=\"grass\">\n";
  ptr += "<a class=\"name\" style=\"font-size: 20px; text-align: left; padding: 3.5%;\"> Температура воздуха <span style=\"  color: #a3a3a3; font-size: 15px;  font-weight: 700;\">/C</span></a>\n";
  ptr += "<a style=\"position: inherit;  text-align: right;  font-size: 35px;  padding: 1.5%;  margin-left: auto;  margin-right: 2%;\">";
  ptr += int(temperature);
  ptr += "</a>\n";
  ptr += "</div>\n";

  /*Влажность cтарая
  ptr += "<div class=\"hum\">\n";
  ptr += "<div style=\"font-size: 32px; height: 0;\"><i class=\"bi bi-power\"></i></div>\n";
  ptr += "<p>Relay1</p>";
  ptr += "</div>\n";
  ptr += "</div>\n";
  */

  ptr += "<div class=\"grass\">\n";
  ptr += "<a class=\"name\" style=\"font-size: 20px; text-align: left; padding: 3.5%;\"> Влажность воздуха <span style=\"  color: #a3a3a3; font-size: 15px;  font-weight: 700;\">/%</span></a>\n";
  ptr += "<a style=\"position: inherit;  text-align: right;  font-size: 35px;  padding: 1.5%;  margin-left: auto;  margin-right: 2%;\">";
  ptr += int(humidity);
  ptr += "</a>\n";
  ptr += "</div>\n";

  //Влажность почвы
  ptr += "<div class=\"grass\">\n";
  ptr += "<a class=\"name\" style=\"font-size: 20px; text-align: left; padding: 3.5%;\"> Влажность почвы <span style=\"  color: #a3a3a3; font-size: 15px;  font-weight: 700;\">/%</span></a>\n";
  ptr += "<a style=\"position: inherit;  text-align: right;  font-size: 35px;  padding: 1.5%;  margin-left: auto;  margin-right: 2%;\">";
  ptr += String(map(analogRead(0), 1024, 280, 0, 100));
  ptr += "</a>\n";
  //ptr += "</div>\n";
  ptr += "</div>\n"; 

    
  ptr += "<div class=\"blocks\">\n";
  
  ptr += "<div class=\"block1 block\">\n";
  ptr += "<div style=\"font-size: 32px; height: 0;\"><i class=\"bi bi-power\"></i></div>\n";
  ptr += "<p>Реле 1</p>";
  ptr += "  <div class=\"container1\">\n";
  ptr += "   <input type=\"checkbox\" class=\"checkbox1\" id=\"checkbox1\">\n";
  ptr += "   <label class=\"switch1\" for=\"checkbox1\">\n";
  ptr += "   <span class=\"slider1\"></span>\n";
  ptr += "   </label>\n";
  ptr += " </div>\n";
  ptr += "</div>\n";

  ptr += "<div class=\"block2 block\">\n";
  ptr += "<div style=\"font-size: 32px; height: 0;\"><i class=\"bi bi-power\"></i></div>\n";
  ptr += "<p>Реле 2</p>";
  ptr += "  <div class=\"container2\">\n";
  ptr += "   <input type=\"checkbox\" class=\"checkbox2\" id=\"checkbox2\">\n";
  ptr += "   <label class=\"switch2\" for=\"checkbox2\">\n";
  ptr += "   <span class=\"slider2\"></span>\n";
  ptr += "   </label>\n";
  ptr += " </div>\n";
  ptr += "</div>\n";
  
  ptr += "<div class=\"block3 block\">\n";
  ptr += "<div style=\"font-size: 32px; height: 0;\"><i class=\"bi bi-power\"></i></div>\n";
  ptr += "<p>Реле 3</p>";
  ptr += "  <div class=\"container3\">\n";
  ptr += "   <input type=\"checkbox\" class=\"checkbox3\" id=\"checkbox3\">\n";
  ptr += "   <label class=\"switch3\" for=\"checkbox3\">\n";
  ptr += "   <span class=\"slider3\"></span>\n";
  ptr += "   </label>\n";
  ptr += " </div>\n";
  ptr += "</div>\n";

  ptr += "<div class=\"block4 block\">\n";
  ptr += "<div style=\"font-size: 32px; height: 0;\"><i class=\"bi bi-power\"></i></div>\n";
  ptr += "<p>Реле 4</p>";
  ptr += "  <div class=\"container4\">\n";
  ptr += "   <input type=\"checkbox\" class=\"checkbox4\" id=\"checkbox4\">\n";
  ptr += "   <label class=\"switch4\" for=\"checkbox4\">\n";
  ptr += "   <span class=\"slider4\"></span>\n";
  ptr += "   </label>\n";
  ptr += " </div>\n";
  ptr += "</div>\n";
  
  ptr += "</div>\n";    
  //конец документа
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
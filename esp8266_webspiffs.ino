#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
//LCD OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

//SSID PASSWORD
const char* ssid = "Muthi Shop"; 
const char* password = "ditfamut123"; 

// PORT WEBSITE 
AsyncWebServer server(80); 

// INI STATIC IP
IPAddress IP(192,168,100,100); 
IPAddress SUBNET(255,255,255,0); 
IPAddress GATEWAY(192,168,100,1); 

String ledState;
String ledState2;
String ribetcuy;
String parameter;
String parameter2;
String paramoled;
String message;

String processor(const String& var){
  if(var == "STATE"){
    if(digitalRead(2)){
      ledState = "";
    }
    else{
      ledState = "checked";
    }
    return ledState;
  }
  if(var == "STATE2"){
    if(digitalRead(D3)){
      ledState2 = "";
    }
    else{
      ledState2 = "checked";
    }
    return ledState2;
  }
  if(var == "OLEDSTATE"){
    if(digitalRead(D3)){
      ribetcuy = "placeholder='"+paramoled+"'";
  }
    else{
      ribetcuy = "placeholder='"+paramoled+"'";
    }
    return ribetcuy;
  }
  
  return String();
}

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
// VOID VOID AN //
void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(D3, OUTPUT);
  //oled
  oled();
  // INITIALIZE SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  //Wifi
  wifi();

  //server
  iniserver();

  
}

void loop() {
    message += paramoled; //convert char to string
    display.clearDisplay();
      if (message.length() > 0 && message.length() <= 8) { //messages between 1 and 5 characters activates text-size 3
        display.setTextSize(3);
      }
    
      if (message.length() > 9 &&  message.length() <= 20) { //messages between 6 and 20 characters activates text-size 2
        display.setTextSize(2);
      }
    
      if (message.length() >= 21) { //messages longer than 21 characters activates text-size 1
        display.setTextSize(1);
      }
  
    display.setCursor(0, 0);  //start printing at top left corner
    display.println(message); //prints the content of the message string on the display
    display.display(); //transfers the buffer to the display
    message = ""; //reset string after each print
}
//---------------------------------------------//
void wifi(){
// Mengatur WiFi 
  WiFi.begin(ssid, password); 
  WiFi.config(IP,GATEWAY,SUBNET);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

// Status Connect
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(10);
  display.setCursor(0, 15);
  display.println(ssid);
  display.setCursor(0, 30);
  display.println(WiFi.localIP());
  display.display();
  delay(2000);
  display.clearDisplay();
  
  
}


void iniserver(){
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
     request->send(SPIFFS, "/index.html", String(), false, processor);
  });
   server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam("state")) {
      parameter = request->getParam("state")->value();
      digitalWrite(2, parameter.toInt());
    }
    if (request->hasParam("state2")) {
      parameter2 = request->getParam("state2")->value();
      digitalWrite(D3, parameter2.toInt());
    }
    if (request->hasParam("oled")){
        paramoled = request->getParam("oled")->value();
        Serial.println(paramoled);
    }
    request->send(200, "text/plain", "OK");
    });
  server.begin();
}

void oled(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
  Serial.println(F("SSD1306 allocation failed"));
  for(;;);
  }
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.println("Initialalizing");
  display.display(); 
}

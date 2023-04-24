#include <WiFi.h>
#include <AsyncTCP.h>
#include <HardwareSerial.h>

// Stolen from: 
// https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/SimpleWiFiServer/SimpleWiFiServer.ino

#define RXp2 16
#define TXp2 17

// Drive Instructions
const char *CMD_FORWARD = "forward;";
const char *CMD_BACKWARD = "backward;";
const char *CMD_STOP = "stop;";

// WiFi constants
const char* ssid = "iPhone 13";
const char* pass = "12345678";

AsyncServer server(8888);
HardwareSerial Arduino(2);


void setup() {
  Serial.begin(9600);
  // Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
  Arduino.begin(115200);

  Serial.println("Entering setup();");

  // Setup WiFi
  Arduino.println();
  Arduino.println();
  Arduino.print("Connecting to ");
  Arduino.println(ssid);
  
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Arduino.print(".");
    Serial.println("Awaiting wifi connection;");
  }

  Arduino.println("");
  Arduino.println("WiFi connected.");
  Arduino.print("IP address: ");
  Arduino.println(WiFi.localIP());


  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.onClient([](void * arg, AsyncClient * client) {
    client->onData(onData);

    client->onDisconnect([](void * arg, AsyncClient * client) {
      Serial.println("Client disconnected");
    });
  }, nullptr);

  // Route for root / web page
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send_P(200, "text/html", content, processor);
  //   AsyncWebParameter *action_param = request->getParam("action");
  //   AsyncWebParameter *power_ratio_param = request->getParam("ratio");
  //   AsyncWebParameter *speed_param = request->getParam("speed");
  //   const char *action = action_param->value().c_str();
  //   const char *power_ratio = power_ratio_param->value().c_str();
  //   const char *speed = speed_param->value().c_str();

  //   // TODO: Pass values to arduino

  //   // if (strcmp(value, "forward") == 0) {
  //   //   Serial.println(CMD_FORWARD);
  //   //   // Serial2.println(255);
  //   // } else if (strcmp(value, "backward") == 0) {

  //   //   Serial.println(CMD_BACKWARD);
  //   //   // Serial2.println(255);
  //   // } else {
  //   //   Serial.println(CMD_STOP);
  //   //   // Serial.printf("Invalid action received (%s)\n", value);
  //   // }
  // });
  
  // Begin the server
  server.begin();
}



void loop() {
  // if (Serial.peek() != -1) {
  //   Arduino.println(Serial.readString());
  // }
}

void onData(void *arg, AsyncClient* client, void *data, size_t len) {
  String message = String((char*)data);
  String delimiter = ",";
  
  int powerRatio = message.substring(0, message.indexOf(delimiter)).toInt();
  message.remove(0, message.indexOf(delimiter) + delimiter.length());
  
  int direction = message.substring(0, message.indexOf(delimiter)).toInt();
  message.remove(0, message.indexOf(delimiter) + delimiter.length());
  
  int speed = message.toInt();

  Serial.printf("Received data: powerRatio=%d, direction=%d, speed=%d\n", powerRatio, direction, speed);

  // send response to client
  client->write("ACK", 3);

  // Do something with the parsed data
}

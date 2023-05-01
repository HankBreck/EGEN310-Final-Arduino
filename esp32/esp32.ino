#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

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

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);

  Serial.println("Entering setup();");

  // Setup WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    // Serial.println("Awaiting wifi connection;");
  }

  Serial.println("");
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
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", content, processor);
    AsyncWebParameter *action_param = request->getParam("action");
    AsyncWebParameter *power_ratio_param = request->getParam("ratio");
    AsyncWebParameter *speed_param = request->getParam("speed");
    if (action_param == NULL) {
      Serial.println("Action is null");
    }
    if (power_ratio_param == NULL) {
      Serial.println("Ratio is null");
    }
    if (speed_param == NULL) {
      Serial.println("Speed is null");
    }
    // Serial.printf("Action: %s\nRatio: %s\nSpeed: %s\n", action_param->value(), power_ratio_param->value(), speed_param->value());
    long action = action_param->value().toInt();
    long power_ratio = power_ratio_param->value().toInt();
    long speed = speed_param->value().toInt();

    // TODO: Pass values to arduino
    Serial2.printf("%d;%d;%d;\n", action, power_ratio, speed);
    // if (strcmp(value, "forward") == 0) {
    //   Serial.println(CMD_FORWARD);
    //   // Serial2.println(255);
    // } else if (strcmp(value, "backward") == 0) {
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
  // Serial.println(Serial2.readString());
  if (Serial2.peek() != -1) {
    Serial.println(Serial2.readString());
  }
  ws.cleanupClients();
}

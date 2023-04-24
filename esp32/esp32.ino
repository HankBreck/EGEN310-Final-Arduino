#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <HardwareSerial.h>

// Stolen from: 
// https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/examples/SimpleWiFiServer/SimpleWiFiServer.ino

// this sample code provided by www.programmingboss.com
#define RXp2 16
#define TXp2 17

// Drive Instructions
const char *CMD_FORWARD = "forward;";
const char *CMD_BACKWARD = "backward;";
const char *CMD_STOP = "stop;";

// WiFi constants
// const char* ssid = "Malicious Mesh Network";
// const char* pass = "casper-1";
const char* ssid = "iPhone 13";
const char* pass = "12345678";

// WiFiServer server(80);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

HardwareSerial Arduino(2);

const char content[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
    <title>ESP Web Server</title>
  </head>
  <body>
    <div>
    <h1>ESP WebSocket Server</h1>
  </div>
  </body>
</html>
)rawliteral";

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle") == 0) {
      Serial.println("");
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    Serial.println("Received STATE message.");
  }
  return String();
}

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

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", content, processor);
    AsyncWebParameter *action_param = request->getParam("action");
    AsyncWebParameter *power_ratio_param = request->getParam("ratio");
    AsyncWebParameter *speed_param = request->getParam("speed");
    const char *action = action_param->value().c_str();
    const char *power_ratio = power_ratio_param->value().c_str();
    const char *speed = speed_param->value().c_str();

    // TODO: Pass values to arduino

    // if (strcmp(value, "forward") == 0) {
    //   Serial.println(CMD_FORWARD);
    //   // Serial2.println(255);
    // } else if (strcmp(value, "backward") == 0) {

    //   Serial.println(CMD_BACKWARD);
    //   // Serial2.println(255);
    // } else {
    //   Serial.println(CMD_STOP);
    //   // Serial.printf("Invalid action received (%s)\n", value);
    // }
  });
  
  // Begin the server
  server.begin();
}

void loop() {
  // Serial.println(Serial2.readString());
  if (Serial.peek() != -1) {
    Arduino.println(Serial.readString());
  }
  ws.cleanupClients();
}
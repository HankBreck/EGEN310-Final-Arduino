#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
// #include <HardwareSerial.h>

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
const char* ssid = "iPhone 13";
const char* pass = "12345678";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// HardwareSerial Printer(2);

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

  initWebSocket();

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
    Serial.printf("Action: %s\nRatio: %s\nSpeed: %s\n", action_param->value(), power_ratio_param->value(), speed_param->value());
    long action = action_param->value().toInt();
    long power_ratio = power_ratio_param->value().toInt();
    long speed = speed_param->value().toInt();

    Serial2.printf("%d;%d;%d;\n", action, power_ratio, speed);
  });

  server.on("/cut", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", content, processor);
    Serial2.println("3;");
  });
  
  // Begin the server
  server.begin();
}

void loop() {
  // Serial.println(Serial2.readString());
  if (Serial2.peek() != -1) {
    delay(500);
    Serial.printf("Arduino: %s\n", Serial2.readStringUntil('\n').c_str());
  }
  ws.cleanupClients();
}
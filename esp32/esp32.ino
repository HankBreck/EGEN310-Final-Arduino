#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

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

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
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
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "noop", processor);
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
    long action = action_param->value().toInt();
    long power_ratio = power_ratio_param->value().toInt();
    long speed = speed_param->value().toInt();

    // Send serialized drive command to Arduino
    Serial2.printf("%d;%d;%d;\n", action, power_ratio, speed);
    delay(500);
  });

  server.on("/cut", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "noop", processor);
    // Send cut command to Arduino
    Serial2.println("3;");
  });
  
  // Begin the server
  server.begin();
}

void loop() {
  // Pass Arduino Serial output to ESP-32 output for testing
  if (Serial2.peek() != -1) {
    Serial.println(Serial2.readString().c_str());
  }
  ws.cleanupClients();
}

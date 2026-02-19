#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>

////////////////////////////////////
// Wifi Config
////////////////////////////////////
const char *wifiName = "Dedicated Wham";
const char *wifiPassword = "MinecraftServer";

WiFiServer server(80);

// ***** Remember *****
// For Tilt: less than 90 looks up, greater than 90 looks down (start position is 90)
// For Pan: less than 94 looks right (from camera angle), greater than 94 looks left. (start position is 94)

////////////////////////////////////
// Servo Objects
////////////////////////////////////
Servo tilt;
Servo pan;

////////////////////////////////////
// Configs (Can adjust as we need )
////////////////////////////////////

// GPIO Pins
const int tiltPin = 4;
const int panPin = 5;

// Center/ Starting locs for servos
const int tiltCentre = 90;
const int panCentre = 94;

// Min/Max angles for tilt *Important
const int tiltMin = tiltCentre - 45; // 45 (Up)
const int tiltMax = tiltCentre + 30; // 120 (Down)

// Min/Max angles for Pan *Important
const int panMin = panCentre - 45; // 49 (Right)
const int panMax = panCentre + 45; // 139 (Left)

////////////////////////////////////
// functions
////////////////////////////////////

void testMovements();
int clampAngle(int, int, int);
void setTilt(int);
void setPan(int);

void wifiConnect();
void wifiTest();

void setup()
{

  Serial.begin(115200);
  delay(1500);

  wifiConnect();
  delay(1000);

  Serial.println("In Setup");
  tilt.attach(tiltPin);
  pan.attach(panPin);
  delay(1000);

  Serial.println("Zeroing motors");
  tilt.write(tiltCentre);
  delay(200);
  pan.write(panCentre);
}

// Main loop
void loop()
{
  wifiTest();
  // testMovements();
}

// Simple function to just test movements
void testMovements()
{

  // center
  tilt.write(tiltCentre);
  pan.write(panCentre);
  delay(1500);

  // tilt min / max at center pan
  tilt.write(tiltMin);
  delay(1500);
  tilt.write(tiltMax);
  delay(1500);

  // pan min / max at center tilt
  tilt.write(tiltCentre);
  pan.write(panMin);
  delay(1500);
  pan.write(panMax);
  delay(1500);

  // Corner test
  tilt.write(tiltMin);
  pan.write(panMin);
  delay(1500);
  tilt.write(tiltMin);
  pan.write(panMax);
  delay(1500);
  tilt.write(tiltMax);
  pan.write(panMin);
  delay(1500);
  tilt.write(tiltMax);
  pan.write(panMax);
  delay(1500);
}


// If angle for tilt/pan > or < min/max angles as defined in config
// new angle just equals min/max
int clampAngle(int angle, int minAngle, int maxAngle)
{
  if (angle < minAngle)
    return minAngle;
  if (angle > maxAngle)
    return maxAngle;
  return angle;
}


// Moves tilt servo to specified angle
void setTilt(int angle)
{
  int setAngle = clampAngle(angle, tiltMin, tiltMax);
  tilt.write(setAngle);
}


// Moves pan servo to specified angle
void setPan(int angle)
{
  int setAngle = clampAngle(angle, panMin, panMax);
  pan.write(setAngle);
}


// Called in setup. Connects esp32 to wifi
// AI generated
void wifiConnect()
{
  Serial.println("Connecting to WiFi...");
  WiFi.begin(wifiName, wifiPassword);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}


// AI generated 
void wifiTest()
{
  WiFiClient client = server.available();

  if (client)
  {
    String request = client.readStringUntil('\r');
    client.flush();

    Serial.println(request);

    // Example: GET /pan?angle=120
    if (request.indexOf("/pan?angle=") != -1)
    {
      int start = request.indexOf("=") + 1;
      int end = request.indexOf(" ", start);
      int angle = request.substring(start, end).toInt();
      setPan(angle);
    }

    if (request.indexOf("/tilt?angle=") != -1)
    {
      int start = request.indexOf("=") + 1;
      int end = request.indexOf(" ", start);
      int angle = request.substring(start, end).toInt();
      setTilt(angle);
    }

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Connection: close");
    client.println();
    client.println("OK");
    client.stop();
  }
}

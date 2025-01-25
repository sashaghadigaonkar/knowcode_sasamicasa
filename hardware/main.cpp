#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "Gautam";
const char *password = "crawizzz";

const int relayPin = 18;
const int relayPin2 = 5;
const int acsPin = 34;

const int irSensorRight = 14;

WebServer server(80);

unsigned long timerDuration = 0;
unsigned long relayOnTime = 0;
bool relaysActive = false;

const float sensitivity = 0.1;
const float vRef = 3.3;
const int adcResolution = 4096;
const float zeroCurrentOffset = vRef / 2;
float accumulatedEnergy = 0.0;
unsigned long previousMillis = 0;
const unsigned long interval = 1000;

const char *webpage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>choose time period</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <div class="left">
        <h2>Energy Monitoring</h2>
        <p id="energy">Energy: Loading...</p>
    </div>
    <div>
        <h1>Set a timer for your stay here.</h1>
        <input id="timer" type="number" placeholder="Enter time in seconds" min="1">
        <button onclick="setTimer()">Turn ON Relays</button>
        <p id="status">Status: Relays are OFF</p>
        <p id="countdown">Countdown: --</p>
    </div>
   <script src="script.js"></script> 
</body>
</html>
)rawliteral";

void handleRoot()
{
    server.send(200, "text/html", webpage);
}

void handleSetTimer()
{
    if (server.hasArg("duration"))
    {
        timerDuration = server.arg("duration").toInt() * 1000;
        relayOnTime = millis();
        relaysActive = true;
        digitalWrite(relayPin, LOW);
        digitalWrite(relayPin2, LOW);
        String response = "Status: Relays turned ON for " + String(timerDuration / 1000) + " seconds";
        server.send(200, "text/plain", response);
        Serial.println(response);
    }
    else
    {
        server.send(400, "text/plain", "Invalid request: duration parameter missing");
    }
}

void handleEnergy()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
        int adcValue = analogRead(acsPin);
        float voltage = adcValue * (vRef / adcResolution);
        float current = (voltage - zeroCurrentOffset) / sensitivity;
        current = abs(current);
        float power = current * 230.0;
        accumulatedEnergy += (power * (interval / 60000.0));
        previousMillis = currentMillis;
    }
    String response = "Energy: " + String(accumulatedEnergy, 2) + " J";
    server.send(200, "text/plain", response);
    Serial.println(response);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Initializing ESP32...");

    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, HIGH);
    pinMode(relayPin2, OUTPUT);
    digitalWrite(relayPin2, HIGH);

    pinMode(irSensorRight, INPUT);

    int threshold = 1500;

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");

    Serial.print("Web server accessible at: http://");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/set_timer", handleSetTimer);
    server.on("/energy", handleEnergy);

    server.begin();
    Serial.println("Web server started");
}

void loop()
{
    server.handleClient();

    if (relaysActive && millis() - relayOnTime >= timerDuration)
    {
        if (digitalRead(irSensorRight) == LOW)
        {
            relayOnTime = millis();
            Serial.println("IR sensor detected no object; timer reset");
        }
        else
        {
            digitalWrite(relayPin, HIGH);
            digitalWrite(relayPin2, HIGH);
            relaysActive = false;
            Serial.println("Relays turned OFF after timer elapsed");
        }
    }
}

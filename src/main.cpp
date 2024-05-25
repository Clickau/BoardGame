#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>

const char *ssid = "CubeWifi";
const char *password = "mycoolcubepassword";

IPAddress local_IP(192, 168, 1, 184);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 1, 0);

WebServer server;
Adafruit_NeoPixel pixels[5] = {{1, 21, NEO_RGB +NEO_KHZ400}, {1, 19, NEO_RGB +NEO_KHZ400},{1, 18, NEO_RGB +NEO_KHZ400},{1, 5, NEO_RGB +NEO_KHZ400},{1, 15, NEO_RGB +NEO_KHZ400}};

uint32_t colors[5];

void handleRoot()
{
    Serial.println("root");
    WiFiClient client = server.client();
    server.send(200, "text/html", "OK");

    String data;
    for (int i = 0; i < 5; i++)
    {
        data = client.readStringUntil('\n');
        colors[i] = data.toInt();
        Serial.println(colors[i], HEX);
        pixels[i].setPixelColor(0, colors[i]);
        pixels[i].show();
    }
}


void setup()
{
    for (int i = 0; i < 5; i++)
    {
        pixels[i].begin();
        pixels[i].setPixelColor(0, 0xffffff);
        pixels[i].show();
    }


    Serial.begin(115200);
    WiFi.config(local_IP, gateway, subnet);
    WiFi.begin(ssid, password);
    while (!WiFi.isConnected()) delay(100);
    Serial.println("Connected!");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.begin();
}

void loop()
{
    server.handleClient();
}
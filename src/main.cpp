#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "UdpServer.h"

// Set WiFi credentials
#define WIFI_SSID "Vodafone-A70678530"
#define WIFI_PASS "casaRossatti"

// Log
Logger _logger = Logger();

// UDP variables
UdpServer _udpServerService = UdpServer(_logger);

void setup()
{
  // put your setup code here, to run once:
  _logger.Log("Starting...");

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Connecting to WiFi...
  _logger.LogInline("Connecting to ");
  _logger.Log(WIFI_SSID);

  // Loop continuously while WiFi is not connected
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    _logger.LogInline(".");
  }

  // Connected to WiFi
  _logger.Log();
  _logger.LogInline("Connected! IP address: ");
  _logger.Log(WiFi.localIP().toString());
  _logger.Log(WiFi.macAddress());

  _udpServerService.Start();
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (!_udpServerService.ParsePacket())
  {
    delay(1000);
    return;
  }

  _logger.Log(_udpServerService.GetLastPacket());

  auto packet = _udpServerService.GetLastPacket();

  // PROBABLY not a magic packet
  if (packet[0] != 0xFF)
    return;

  auto receivedMacAddress = _udpServerService.ExtractMacAddressFromMagicPacket(packet);

  _logger.LogInline("Requested a wake on Lan packet for ");
  _logger.Log(String(receivedMacAddress.c_str()));

  if (!receivedMacAddress.compare(WiFi.macAddress().c_str()))
    return;

  _logger.Log("MAC Match, a valid request has been processed, broadcasting to subnet!");

  //Setting here the mac addresses to wake up
  _udpServerService.SendWakeOnLan(IPAddress(192,168,1,255),"D8:BB:C1:9D:81:C3");
}
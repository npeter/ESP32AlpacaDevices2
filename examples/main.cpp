/**************************************************************************************************
  Filename:       main.cpp
  Revised:        $Date: 2024-01-30$
  Revision:       $Revision: 01 $

  Description:    ASCOM Alpaca ESP32 Server Test

  Copyright 2024 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/


// commend/uncommend to enable/disable device testsing with templates
#define TEST_COVER_CALIBRATOR     // create CoverCalibrator device
#define TEST_SWITCH               // create Switch device
#define TEST_OBSERVING_CONDITIONS // create ObservingConditions device

//#define TEST_RESTART              // only for testing

#include <Arduino.h>
//#include <ESP_WiFiManager.h>TODO
#include <ESPmDNS.h>
#include "Credentials.h"

#include <AlpacaDebug.h>
#include <AlpacaServer.h>

#ifdef TEST_COVER_CALIBRATOR
#include "CoverCalibrator.h"
CoverCalibrator coverCalibrator;
#endif

#ifdef TEST_SWITCH
#include "Switch.h"
Switch switchDevice;
#endif

#ifdef TEST_OBSERVING_CONDITIONS
#include "ObservingConditions.h"
ObservingConditions observingConditions;
#endif

#define PIN_WIFI_LED 15 // if lolin_s2_mini

// ASCOM Alpaca server with discovery
AlpacaServer alpaca_server(ALPACA_MNG_SERVER_NAME, ALPACA_MNG_MANUFACTURE, ALPACA_MNG_MANUFACTURE_VERSION, ALPACA_MNG_LOCATION);

void setupWifi()
{
  pinMode(PIN_WIFI_LED, OUTPUT);

  Serial.println("\n# Starting WiFi");

  //ESP_WiFiManager ESP_wifiManager("ESP32_Alpaca_test");
  //ESP_wifiManager.setConnectTimeout(30);
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(DEFAULT_SSID, DEFAULT_PWD);

  WiFi.waitForConnectResult();
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("# Failed to connect");
  }
  else
  {
    Serial.print("# Local IP: ");
    Serial.println(WiFi.localIP());
    digitalWrite(PIN_WIFI_LED, HIGH);
    if (!MDNS.begin("HOSTNAME"))
    {
      Serial.println("# Error starting mDNS");
      return;
    }
  }
}

#ifdef TEST_RESTART
// ========================================================================
// SW Restart
bool restart = false;                          // enable/disable
uint32_t g_restart_start_time_ms = 0xFFFFFFFF; // Timer for countdown
uint32_t const k_RESTART_DELAY_MS = 10000;     // Restart Delay

/**
 * SetRestart
 */
void ActivateRestart()
{
  restart = true;
  g_restart_start_time_ms = millis();
}

/*
 */
void checkForRestart()
{
  if (alpaca_server.GetResetRequest() || restart)
  {
    uint32_t timer_ms = millis() - g_restart_start_time_ms;
    uint32_t coun_down_sec = (k_RESTART_DELAY_MS - timer_ms) / 1000;

    if (timer_ms >= k_RESTART_DELAY_MS)
    {
      ESP.restart();
    }
  }
  else
  {
    g_restart_start_time_ms = millis();
  }
}
#endif

void setup()
{
  Serial.begin(115200);
  delay(5000);  // time to detect USB device 
  
  Serial.printf("BigPet ESP32 ALPACA Server Test started ...\n");
  setupWifi();
  alpaca_server.Begin();

#ifdef TEST_COVER_CALIBRATOR
  coverCalibrator.Begin();
  alpaca_server.AddDevice(&coverCalibrator);
#endif

#ifdef TEST_SWITCH
  switchDevice.Begin();
  alpaca_server.AddDevice(&switchDevice);
#endif

#ifdef TEST_OBSERVING_CONDITIONS
  observingConditions.Begin();
  alpaca_server.AddDevice(&observingConditions);
#endif

  alpaca_server.LoadSettings();
}

void loop()
{
#ifdef TEST_RESTART
  checkForRestart();
#endif

  alpaca_server.Loop();

#ifdef TEST_COVER_CALIBRATOR
  coverCalibrator.Loop();
#endif

#ifdef TEST_SWITCH
  switchDevice.Loop();
#endif

#ifdef TEST_OBSERVING_CONDITIONS
  observingConditions.Loop();
#endif

  delay(100);
}



/**************************************************************************************************
  Filename:       Switch.cpp
  Revised:        $Date: 2024-01-26$
  Revision:       $Revision: 01 $
  Description:    ASCOM Alpaca Switch Device Template

  Copyright 2024 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#include "Switch.h"

const uint32_t k_num_of_switch_devices = 4;

SwitchDevice_t init_switch_device[k_num_of_switch_devices] = {
    {true, true, "Switch-0", "Relay 0 (read/write)", 0.0, 0.0, 10.0, 1.0},
    {false, false, "Switch-1", "Temperature (read only)", 20.0, -50.0, 50.0, 0.1},
    {true, false, "Switch 2", "Door closed (read only) - fixed init", 0.0, 0.0, 1.0, 1.0},
    {false, true, "Switch-3", "Heater (read/write) - fixed init", 0.0, 0.0, 100.0, 0.5}};

Switch::Switch() : AlpacaSwitch(k_num_of_switch_devices)
{
}

void Switch::Begin()
{
  for (uint32_t u = 0; u < k_num_of_switch_devices; u++)
  {
    InitSwitchInitBySetup(u, init_switch_device[u].init_by_setup);
    InitSwitchCanWrite(u, init_switch_device[u].can_write);
    InitSwitchName(u, init_switch_device[u].name);
    InitSwitchDescription(u, init_switch_device[u].description);
    InitSwitchValue(u, init_switch_device[u].value);
    InitSwitchMinValue(u, init_switch_device[u].min_value);
    InitSwitchMaxValue(u, init_switch_device[u].max_value);
    InitSwitchStep(u, init_switch_device[u].step);
  }

  AlpacaSwitch::Begin();

  // SLOG_PRINTF(SLOG_INFO, "REGISTER handler for \"%s\"\n", "/setup/v1/switch/0/setup");
  // _p_alpaca_server->getServerTCP()->on("/setup/v1/switch/0/setup", HTTP_GET, [this](AsyncWebServerRequest *request)
  //                                      { DBG_REQ; _alpacaGetPage(request, FOCUSER_SETUP_URL); DBG_END; });



  // Init switches
}

void Switch::Loop()
{  
  // TODO
  // This is only an example with funny HW to simulate some inputs

  // Get physical data and set value
  double temperature = 11.0 + static_cast<double>(millis()%10)/10;
  SetSwitchValue(1, temperature); // double

  bool door_closed = ((millis()/10000)%2) == 0 ? true : false;
  SetSwitch(2, door_closed);  // bool
}

/**
 * This methode is called by AlpacaSwitch to manipulate physical device 
 */
const bool Switch::_writeSwitchValue(uint32_t id, double value) {
  bool result = false;  // wrong id or invalid value

  // TODO check id 
  // TODO write to physical device, GPIO, etc
  result = true; 
  SLOG_DEBUG_PRINTF("id=%d value=%f result=%s", id, value, result?"true":"false");

  return result;
}

void Switch::AlpacaReadJson(JsonObject &root)
{
  DBG_JSON_PRINTFJ(SLOG_NOTICE, root, "BEGIN (root=<%s>) ...\n", _ser_json_);
  AlpacaSwitch::AlpacaReadJson(root);

  char title[32] = "";
  for (uint32_t u = 0; u < GetMaxSwitch(); u++)
  {
    if (GetSwitchInitBySetup(u))
    {
      snprintf(title, sizeof(title), "Configuration Device %d", u);
      if (JsonObject obj_config = root[title])
      {
        InitSwitchName(u, obj_config["Name"] | GetSwitchName(u));
        InitSwitchDescription(u, obj_config["Description"] | GetSwitchDescription(u));
        InitSwitchCanWrite(u, obj_config["CanWrite"] | GetSwitchCanWrite(u));
        InitSwitchMinValue(u, obj_config["MinValue"] | GetSwitchMinValue(u));
        InitSwitchMaxValue(u, obj_config["MaxValue"] | GetSwitchMaxValue(u));
        InitSwitchStep(u, obj_config["Step"] | GetSwitchStep(u));
        DBG_JSON_PRINTFJ(SLOG_NOTICE, obj_config,"... title=%s obj_config=<%s> \n", title, _ser_json_);

      }
    }
  }
  SLOG_PRINTF(SLOG_NOTICE, "... END\n");
}

void Switch::AlpacaWriteJson(JsonObject &root)
{
  DBG_JSON_PRINTFJ(SLOG_NOTICE, root, "BEGIN root=%s ...\n", _ser_json_);
  AlpacaSwitch::AlpacaWriteJson(root);

  char title[32] = "";

  // prepare Config
  for (uint32_t u = 0; u < GetMaxSwitch(); u++)
  {
    if (GetSwitchInitBySetup(u))
    {
      snprintf(title, sizeof(title), "Configuration Device %d", u);
      JsonObject obj_config = root[title].to<JsonObject>();
      {
        char s[128];
        snprintf(s, sizeof(s), "%s", GetSwitchName(u));
        obj_config["Name"] = s;
      }
      {
        char s[128];
        snprintf(s, sizeof(s), "%s", GetSwitchDescription(u));
        obj_config["Description"] = s;
      }
      obj_config["CanWrite"] = GetSwitchCanWrite(u);
      obj_config["MinValue"] = GetSwitchMinValue(u);
      obj_config["MaxValue"] = GetSwitchMaxValue(u);
      obj_config["Step"] = GetSwitchStep(u);

      DBG_JSON_PRINTFJ(SLOG_NOTICE, obj_config, "... title=%s (obj_config=<%s>)\n", title, _ser_json_);
    }
  }

  // Prepare states
  for (uint32_t u = 0; u < GetMaxSwitch(); u++)
  {
    // #add # for read only
    snprintf(title, sizeof(title), "#States_Device_%d", u);
    JsonObject obj_state = root[title].to<JsonObject>();
    if (obj_state)
    {
      if (!GetSwitchInitBySetup(u))
      {
        {
          char s[128];
          snprintf(s, sizeof(s), "%s", GetSwitchName(u));
          obj_state["Name"] = s;
        }
        {
          char s[128];
          snprintf(s, sizeof(s), "%s", GetSwitchDescription(u));
          obj_state["Description"] = s;
        }
        obj_state["CanWrite"] = GetSwitchCanWrite(u);
        obj_state["Value"] = GetSwitchValue(u);
        obj_state["MinValue"] = GetSwitchMinValue(u);
        obj_state["MaxValue"] = GetSwitchMaxValue(u);
        obj_state["Step"] = GetSwitchStep(u);
      }
      obj_state["Value"] = GetSwitchValue(u);
      DBG_JSON_PRINTFJ(SLOG_NOTICE, obj_state, "... title=%s (obj_state=<%s>)\n", title, _ser_json_);
    }
  }

  DBG_JSON_PRINTFJ(SLOG_NOTICE, root, "... END \"%s\"\n", _ser_json_);
}

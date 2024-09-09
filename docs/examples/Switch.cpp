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
    {true, true, "Switch 0", "My Switch 0 read/write", 0.0, 0.0, 10.0, 1.0},
    {false, false, "Switch 1", "My Switch 1 read only", 20.0, -50.0, 50.0, 0.1},
    {true, false, "Switch 2", "My Switch 2 read only - fixed init", 0.0, 0.0, 1.0, 1.0},
    {false, true, "Switch 3", "My Switch 3 read/write - fixed init", 0.0, 0.0, 100.0, 0.01}};

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
  // Init switches
}

void Switch::Loop()
{ // empty
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
  SLOG_PRINTF(SLOG_NOTICE, "BEGIN ...\n");
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

  DBG_JSON_PRINTFJ(SLOG_NOTICE, root, "... END \"%s\"\n");
}

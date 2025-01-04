/**************************************************************************************************
  Description: ASCOM Alpaca Switch Device Template
  Copyright 2024-2025 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#pragma once
#include "AlpacaSwitch.h"

// comment/uncomment to enable/disable debugging
// #define DEBUG_SWITCH

class Switch : public AlpacaSwitch
{
private:
    // optional Alpaca service: to be implemented if needed
    const bool _putAction(const char *const action, const char *const parameters, char *string_response, size_t string_response_size) { return false; }
    const bool _putCommandBlind(const char *const command, const char *const raw, bool &bool_response) { return false; };
    const bool _putCommandBool(const char *const command, const char *const raw, bool &bool_response) { return false; };
    const bool _putCommandString(const char *const command_str, const char *const raw, char *string_response, size_t string_response_size) { return false; };

    const bool _writeSwitchValue(uint32_t id, double value);

    void AlpacaReadJson(JsonObject &root);
    void AlpacaWriteJson(JsonObject &root);

#ifdef DEBUG_SWITCH
    void DebugSwitchDevice(uint32_t id);
#endif

public:
    Switch();
    void Begin();
    void Loop();
};
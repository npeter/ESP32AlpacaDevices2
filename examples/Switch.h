/**************************************************************************************************
  Filename:       Switch.h
  Revised:        $Date: 2024-01-26$
  Revision:       $Revision: 01 $
  Description:    ASCOM Alpaca Switch Device Template

  Copyright 2024 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#pragma once
#include "AlpacaSwitch.h"

// comment/uncomment to enable/disable debugging
// #define DEBUG_SWITCH

class Switch : public AlpacaSwitch
{
private:
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
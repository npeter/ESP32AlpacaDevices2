/**************************************************************************************************
  Filename:       Switch.h
  Revised:        $Date: 2024-01-26$
  Revision:       $Revision: 01 $
  Description:    ASCOM Alpaca Switch Device Template

  Copyright 2024 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#pragma once
#include "AlpacaSwitch.h"

class Switch : public AlpacaSwitch
{
private:
    const bool _writeSwitchValue(uint32_t id, double value)
    {
        // write to physical device
        return true;
    }

    void AlpacaReadJson(JsonObject &root);
    void AlpacaWriteJson(JsonObject &root);

public:
    Switch();
    void Begin();
    void Loop();
};
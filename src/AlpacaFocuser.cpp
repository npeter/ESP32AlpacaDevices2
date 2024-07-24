/**************************************************************************************************
  Filename:       AlpacaFocuser.cpp
  Revised:        $Date: 2024-07-24$
  Revision:       $Revision: 01 $
  Description:    Common ASCOM Alpaca Focuser V3

  Copyright 2024 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#include "AlpacaFocuser.h"

AlpacaFocuser::AlpacaFocuser()
{
    strlcpy(_device_type, ALPACA_FOCUSER_DEVICE_TYPE, sizeof(_device_type));
    strlcpy(_device_description, ALPACA_FOCUSER_DESCRIPTION, sizeof(_device_description));
    strlcpy(_driver_info, ALPACA_FOCUSER_DRIVER_INFO, sizeof(_driver_info));
    strlcpy(_driver_version, ALPACA_FOCUSER_DRIVER_VERSION, sizeof(_driver_version));
    _device_interface_version = ALPACA_FOCUSER_INTERFACE_VERSION;
}

void AlpacaFocuser::Begin()
{
    AlpacaDevice::Begin();
}

void AlpacaFocuser::RegisterCallbacks()
{
    AlpacaDevice::RegisterCallbacks();

    // this->createCallBack(LHF(_alpacaGetMaxSwitch), HTTP_GET, "maxswitch", false);
    // this->createCallBack(LHF(_alpacaGetCanWrite), HTTP_GET, "canwrite", false);
    // this->createCallBack(LHF(_alpacaGetSwitch), HTTP_GET, "getswitch", false);
    // this->createCallBack(LHF(_alpacaGetSwitchDescription), HTTP_GET, "getswitchdescription", false);
    // this->createCallBack(LHF(_alpacaGetSwitchName), HTTP_GET, "getswitchname", false);
    // this->createCallBack(LHF(_alpacaGetSwitchValue), HTTP_GET, "getswitchvalue", false);
    // this->createCallBack(LHF(_alpacaGetMinSwitchValue), HTTP_GET, "minswitchvalue", false);
    // this->createCallBack(LHF(_alpacaGetMaxSwitchValue), HTTP_GET, "maxswitchvalue", false);
    // this->createCallBack(LHF(_alpacaGetSwitchStep), HTTP_GET, "switchstep", false);

    // this->createCallBack(LHF(_alpacaPutSetSwitch), HTTP_GET, "setswitch", false);
    // this->createCallBack(LHF(_alpacaPutSetSwitchName), HTTP_GET, "setswitchname", false);
    // this->createCallBack(LHF(_alpacaPutSetSwitchValue), HTTP_GET, "setswitchvalue", false);
}
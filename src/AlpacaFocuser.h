/**************************************************************************************************
  Filename:       AlpacaFocuser.h
  Revised:        $Date: 2024-07-24$
  Revision:       $Revision: 01 $
  Description:    Common ASCOM Alpaca Focuser V3

  Copyright 2024 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#pragma once
#include "AlpacaDevice.h"



class AlpacaFocuser : public AlpacaDevice
{
private:


private:
    void _alpacaGetAdminPage(AsyncWebServerRequest *request);

    void _alpacaGetAbsolut(AsyncWebServerRequest *request);
    void _alpacaGetIsMoving(AsyncWebServerRequest *request);
    void _alpacaGetMaxIncrement(AsyncWebServerRequest *request);
    void _alpacaGetMaxStep(AsyncWebServerRequest *request);
    void _alpacaGetPosition(AsyncWebServerRequest *request);
    void _alpacaGetStepSize(AsyncWebServerRequest *request);
    void _alpacaGetTempComp(AsyncWebServerRequest *request);
    void _alpacaGetTempCompAvailable(AsyncWebServerRequest *request);
    void _alpacaGetTemperature(AsyncWebServerRequest *request);

    void _alpacaPutTempComp(AsyncWebServerRequest *request);
    void _alpacaPutHalt(AsyncWebServerRequest *request);
    void _alpacaPutMove(AsyncWebServerRequest *request);

#ifdef ALPACA_FOCUSER_PUT_ACTION_IMPLEMENTED
    void AlpacaPutAction(AsyncWebServerRequest *request);
    virtual const bool _putAction(const char *const action, const char *const parameters)=0;
#endif

    virtual const bool _putTempComp(bool temp_comp) = 0;
    virtual const bool _putHalt() = 0;
    virtual const bool _putMove(int32_t position) = 0;

    virtual const bool _getAbsolut() = 0;
    virtual const bool _getIsMoving() = 0;
    virtual const int32_t _getMaxIncrement() = 0;
    virtual const int32_t _getMaxStep() = 0;
    virtual const int32_t _getPosition() = 0;
    virtual const double _getStepSize() = 0;
    virtual const bool _getTempComp() = 0;
    virtual const bool _getTempCompAvailable() = 0;
    virtual const double _getTemperature() = 0;
    
protected:
    AlpacaFocuser();
    void Begin();
    void RegisterCallbacks();

public:
};
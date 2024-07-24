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
    bool _absolut = true;   // TODO define init values
    bool _is_moving = false;
    int32_t _max_increment = 1000; 
    int32_t _max_step = 1000; 
    int32_t _position = 0;
    int32_t _step_size = 1; 
    bool _temp_comp = false;
    bool _temp_comp_available = false;
    double _temperature = 99.0; 

private:
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

    virtual bool _putTempComp(bool temp_comp) = 0;
    virtual bool _putHalt() = 0;
    virtual bool _putMove(int32_t position) = 0;

protected:
    AlpacaFocuser();
    void Begin();
    void RegisterCallbacks();

    // const size_t GetMaxSwitch() { return _max_switch_devices; };

    // // geter!
    const bool GetAbsolut() { return _absolut; };
    const bool GetIsMoving() { return _is_moving; };
    const int32_t GetMaxIncrement() { return _max_increment; };
    const int32_t GetMaxStep() { return _max_step; };
    const int32_t GetPosition() { return _position; };
    const int32_t GetStepSize() { return _step_size; };
    const bool GetTempComp() { return _temp_comp; };
    const bool GetTempCompAvailable() { return _temp_comp_available; };
    const double GetTemperature() { return _temperature; };
    
     // setters ... only for initialization 
    void InitAbsolut(bool absolut) { _absolut = absolut; };
    void InitIsMoving(bool is_moving) { _is_moving = is_moving; };
    void InitMaxIncrement(int32_t max_increment) { _max_increment = max_increment; };
    void InitMaxStep(int32_t max_step) { _max_step = max_step; };
    void InitPosition(int32_t position) { _position = position; };
    void InitStepSize(int32_t step_size) { _step_size = step_size; };
    void InitTempComp(bool temp_comp) { _temp_comp = temp_comp; };
    void InitTempCompAvailable(bool temp_comp_available) { _temp_comp_available = temp_comp_available; };
    void InitTemperatur(double temperature) { _temperature = temperature; };


    // setter
    const bool SetIsMoving(bool is_moving) { return _is_moving; }
    // const bool SetSwitch(uint32_t id, bool bool_value);



public:
};
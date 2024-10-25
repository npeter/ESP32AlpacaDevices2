/**************************************************************************************************
  Filename:       AlpacaCoverCalibrator.cpp
  Revised:        $Date: 2024-01-14$
  Revision:       $Revision: 01 $
  Description:    Common ASCOM Alpaca CoverCalibrator V1

  Copyright 2024 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#include "AlpacaCoverCalibrator.h"

// init static members
const char *const AlpacaCoverCalibrator::kAlpacaCalibratorStatusStr[7] = {"NotPresent", "Off", "NotReady", "On", "Unknown", "Error", "Invalid"};
const char *const AlpacaCoverCalibrator::k_alpaca_cover_status_str[7] = {"NotPresent", "Closed", "NotReady", "Open", "Unknown", "Error", "Invalid"};

AlpacaCoverCalibrator::AlpacaCoverCalibrator()
{
    strlcpy(_device_type, ALPACA_COVER_CALIBRATOR_DEVICE_TYPE, sizeof(_device_type));
    strlcpy(_device_description, ALPACA_COVER_CALIBRATOR_DESCRIPTION, sizeof(_device_description));
    strlcpy(_driver_info, ALPACA_COVER_CALIBRATOR_DRIVER_INFO, sizeof(_driver_info));
    strlcpy(_device_and_driver_version, esp32_alpaca_device_library_version, sizeof(_device_and_driver_version));
    _device_interface_version = ALPACA_COVER_CALIBRATOR_INTERFACE_VERSION;

    _brightness = 0;
}

void AlpacaCoverCalibrator::Begin()
{
    snprintf(_device_and_driver_version, sizeof(_device_and_driver_version), "%s/%s", _getFirmwareVersion(), esp32_alpaca_device_library_version);
    AlpacaDevice::Begin();
}

void AlpacaCoverCalibrator::RegisterCallbacks()
{
    AlpacaDevice::RegisterCallbacks();

    this->createCallBack(LHF(AlpacaPutAction), HTTP_PUT, "action");
    this->createCallBack(LHF(AlpacaPutCommandBlind), HTTP_PUT, "commandblind");
    this->createCallBack(LHF(AlpacaPutCommandBool), HTTP_PUT, "commandbool");
    this->createCallBack(LHF(AlpacaPutCommandString), HTTP_PUT, "commandstring");

    this->createCallBack(LHF(_alpacaGetBrightness), HTTP_GET, "brightness");
    this->createCallBack(LHF(_alpacaGetCalibratorState), HTTP_GET, "calibratorstate");
    this->createCallBack(LHF(_alpacaGetCoverState), HTTP_GET, "coverstate");
    this->createCallBack(LHF(_alpacaGetMaxBrightness), HTTP_GET, "maxbrightness");

    this->createCallBack(LHF(_alpacaPutCalibratorOff), HTTP_PUT, "calibratoroff");
    this->createCallBack(LHF(_alpacaPutCalibratorOn), HTTP_PUT, "calibratoron");
    this->createCallBack(LHF(_alpacaPutCloseCover), HTTP_PUT, "closecover");
    this->createCallBack(LHF(_alpacaPutHaltCover), HTTP_PUT, "haltcover");
    this->createCallBack(LHF(_alpacaPutOpenCover), HTTP_PUT, "opencover");
}

#ifdef ALPACA_COVER_CALIBRATOR_PUT_ACTION_IMPLEMENTED
void AlpacaCoverCalibrator::AlpacaPutAction(AsyncWebServerRequest *request)
{

    DBG_DEVICE_PUT_ACTION_REQ
    _service_counter++;
    uint32_t client_idx = 0;
    char action[128] = {0};
    char parameters[128] = {0};

    _alpaca_server->RspStatusClear(_rsp_status);

    try
    {
        if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0)
            throw(&_rsp_status);

        if (_alpaca_server->GetParam(request, "Action", action, sizeof(action), Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Action");

        if (_alpaca_server->GetParam(request, "Parameters", parameters, sizeof(parameters), Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Parameters");

        if (!_putAction(action, parameters))
            _alpaca_server->ThrowRspStatusActionNotImplemented(request, _rsp_status, action, parameters);
    }
    catch (AlpacaRspStatus_t *rspStatus)
    { // empty
    }

    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};
#endif

#ifdef ALPACA_COVER_CALIBRATOR_PUT_COMMAND_BLIND_IMPLEMENTED
void AlpacaCoverCalibrator::AlpacaPutCommandBlind(AsyncWebServerRequest *request)
{

    DBG_DEVICE_PUT_ACTION_REQ
    _service_counter++;
    uint32_t client_idx = 0;
    char command[128] = {0};
    char raw[16] = {0};

    _alpaca_server->RspStatusClear(_rsp_status);

    try
    {
        if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0)
            throw(&_rsp_status);

        if (_alpaca_server->GetParam(request, "Command", command, sizeof(command), Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Action");

        if (_alpaca_server->GetParam(request, "Raw", raw, sizeof(raw), Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Parameters");

        if (!_putCommandBlind(command, raw))
            _alpaca_server->ThrowRspStatusActionNotImplemented(request, _rsp_status, command, raw);
    }
    catch (AlpacaRspStatus_t *rspStatus)
    { // empty
    }

    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};
#endif

#ifdef ALPACA_COVER_CALIBRATOR_PUT_COMMAND_BOOL_IMPLEMENTED
void AlpacaCoverCalibrator::AlpacaPutCommandBool(AsyncWebServerRequest *request)
{

    DBG_DEVICE_PUT_ACTION_REQ
    _service_counter++;
    uint32_t client_idx = 0;
    char command[128] = {0};
    char raw[16] = {0};
    bool bool_response = false;

    _alpaca_server->RspStatusClear(_rsp_status);

    try
    {
        if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0)
            throw(&_rsp_status);

        if (_alpaca_server->GetParam(request, "Command", command, sizeof(command), Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Action");

        if (_alpaca_server->GetParam(request, "Raw", raw, sizeof(raw), Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Parameters");

        if (!_putCommandBool(command, raw, bool_response))
            _alpaca_server->ThrowRspStatusActionNotImplemented(request, _rsp_status, command, raw);

        _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, bool_response);            
    }
    catch (AlpacaRspStatus_t *rspStatus)
    { 
        _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    }

    DBG_END
};
#endif

#ifdef ALPACA_COVER_CALIBRATOR_PUT_COMMAND_STRING_IMPLEMENTED
void AlpacaCoverCalibrator::AlpacaPutCommandString(AsyncWebServerRequest *request)
{

    DBG_DEVICE_PUT_ACTION_REQ
    _service_counter++;
    uint32_t client_idx = 0;
    char command[128] = {0};
    char raw[16] = {0};
    char string_response[128] = { 0 };

    _alpaca_server->RspStatusClear(_rsp_status);

    try
    {
        if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0)
            throw(&_rsp_status);

        if (_alpaca_server->GetParam(request, "Command", command, sizeof(command), Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Action");

        if (_alpaca_server->GetParam(request, "Raw", raw, sizeof(raw), Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Parameters");

        if (!_putCommandString(command, raw, string_response, sizeof(string_response)))
            _alpaca_server->ThrowRspStatusActionNotImplemented(request, _rsp_status, command, raw);

        _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, string_response);            
    }
    catch (AlpacaRspStatus_t *rspStatus)
    { 
        _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    }

    DBG_END
};
#endif


void AlpacaCoverCalibrator::_alpacaGetBrightness(AsyncWebServerRequest *request)
{
    DBG_CC_GET_BRIGHTNESS
    _service_counter++;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, GetBrightness());
    DBG_END
}

void AlpacaCoverCalibrator::_alpacaGetCalibratorState(AsyncWebServerRequest *request)
{
    DBG_CC_GET_CALIBRATOR_STATE
    _service_counter++;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (int32_t)GetCalibratorState());
    DBG_END
}

void AlpacaCoverCalibrator::_alpacaGetCoverState(AsyncWebServerRequest *request)
{
    DBG_CC_GET_COVER_STATE
    _service_counter++;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (int32_t)GetCoverState());
    DBG_END
}

void AlpacaCoverCalibrator::_alpacaGetMaxBrightness(AsyncWebServerRequest *request)
{
    DBG_CC_GET_MAX_BRIGHTNESS
    _service_counter++;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, GetMaxBrightness());
    DBG_END
}

void AlpacaCoverCalibrator::_alpacaPutCalibratorOff(AsyncWebServerRequest *request)
{
    DBG_CC_PUT_CALIBRATOR_OFF
    _service_counter++;
    uint32_t client_idx = 0;
    _alpaca_server->RspStatusClear(_rsp_status);

    try
    {
        if (GetCalibratorState() == AlpacaCalibratorStatus_t::kNotPresent)
            _alpaca_server->ThrowRspStatusDeviceNotImplemented(request, _rsp_status, "Calibrator");

        if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0)
            throw(&_rsp_status);

        _calibratorOff();
    }
    catch (AlpacaRspStatus_t *rspStatus)
    { // empty
    }

    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
}

void AlpacaCoverCalibrator::_alpacaPutCalibratorOn(AsyncWebServerRequest *request)
{
    DBG_CC_PUT_CALIBRATOR_ON
    _service_counter++;
    uint32_t client_idx = 0;
    int32_t brightness = -1;
    _alpaca_server->RspStatusClear(_rsp_status);

    try
    {
        if (GetCalibratorState() == AlpacaCalibratorStatus_t::kNotPresent)
            _alpaca_server->ThrowRspStatusDeviceNotImplemented(request, _rsp_status, "Calibrator");

        if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0)
            throw(&_rsp_status);

        if (_alpaca_server->GetParam(request, "Brightness", brightness, Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Brigthness");

        if (_calibratorOn(brightness) == false)
            _alpaca_server->ThrowRspStatusParameterInvalidValue(request, _rsp_status, "Brightness", brightness);
    }
    catch (AlpacaRspStatus_t *rspStatus)
    { // empty
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
}

void AlpacaCoverCalibrator::_alpacaPutCloseCover(AsyncWebServerRequest *request)
{
    DBG_CC_PUT_CLOSE_COVER
    _service_counter++;
    uint32_t client_idx = 0;
    _alpaca_server->RspStatusClear(_rsp_status);

    try
    {
        if (GetCoverState() == AlpacaCoverStatus_t::kNotPresent)
            _alpaca_server->ThrowRspStatusDeviceNotImplemented(request, _rsp_status, "Cover");

        client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict);
        _closeCover();
    }
    catch (AlpacaRspStatus_t *rspStatus)
    { // empty
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
}

void AlpacaCoverCalibrator::_alpacaPutHaltCover(AsyncWebServerRequest *request)
{
    DBG_CC_PUT_HALT_COVER
    _service_counter++;
    uint32_t client_idx = 0;
    int32_t brightness = -1;
    _alpaca_server->RspStatusClear(_rsp_status);
    try
    {
        if (GetCoverState() == AlpacaCoverStatus_t::kNotPresent)
            _alpaca_server->ThrowRspStatusDeviceNotImplemented(request, _rsp_status, "Cover");

        client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict);
        _haltCover();
    }
    catch (AlpacaRspStatus_t *rspStatus)
    { // empty
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
}

void AlpacaCoverCalibrator::_alpacaPutOpenCover(AsyncWebServerRequest *request)
{
    DBG_CC_PUT_OPEN_COVER
    _service_counter++;
    uint32_t client_idx = 0;
    int32_t brightness = -1;
    _alpaca_server->RspStatusClear(_rsp_status);
    try
    {
        if (GetCoverState() == AlpacaCoverStatus_t::kNotPresent)
            _alpaca_server->ThrowRspStatusDeviceNotImplemented(request, _rsp_status, "Cover");

        client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict);
        _openCover();
    }
    catch (AlpacaRspStatus_t *rspStatus)
    { // empty
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
}

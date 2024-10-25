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
    strlcpy(_device_and_driver_version, esp32_alpaca_device_library_version, sizeof(_device_and_driver_version));
    _device_interface_version = ALPACA_FOCUSER_INTERFACE_VERSION;
}

void AlpacaFocuser::Begin()
{
    snprintf(_device_and_driver_version, sizeof(_device_and_driver_version), "%s/%s", _getFirmwareVersion(), esp32_alpaca_device_library_version);
    AlpacaDevice::Begin();
}

void AlpacaFocuser::RegisterCallbacks()
{
    AlpacaDevice::RegisterCallbacks();

#ifdef ALPACA_FOCUSER_PUT_ACTION_IMPLEMENTED
    this->createCallBack(LHF(AlpacaPutAction), HTTP_PUT, "action");
#endif
#ifdef ALPACA_FOCUSER_PUT_COMMAND_BLIND_IMPLEMENTED
    this->createCallBack(LHF(AlpacaPutCommandBlind), HTTP_PUT, "commandblind");
#endif
#ifdef ALPACA_FOCUSER_PUT_COMMAND_BOOL_IMPLEMENTED
    this->createCallBack(LHF(AlpacaPutCommandBool), HTTP_PUT, "commandbool");
#endif
#ifdef ALPACA_FOCUSER_PUT_COMMAND_STRING_IMPLEMENTED
    this->createCallBack(LHF(AlpacaPutCommandString), HTTP_PUT, "commandstring");
#endif

    this->createCallBack(LHF(_alpacaGetAbsolut), HTTP_GET, "absolute");
    this->createCallBack(LHF(_alpacaGetIsMoving), HTTP_GET, "ismoving");
    this->createCallBack(LHF(_alpacaGetMaxIncrement), HTTP_GET, "maxincrement");
    this->createCallBack(LHF(_alpacaGetMaxStep), HTTP_GET, "maxstep");
    this->createCallBack(LHF(_alpacaGetPosition), HTTP_GET, "position");
    this->createCallBack(LHF(_alpacaGetStepSize), HTTP_GET, "stepsize");
    this->createCallBack(LHF(_alpacaGetTempComp), HTTP_GET, "tempcomp");
    this->createCallBack(LHF(_alpacaGetTempCompAvailable), HTTP_GET, "tempcompavailable");
    this->createCallBack(LHF(_alpacaGetTemperature), HTTP_GET, "temperature");

    this->createCallBack(LHF(_alpacaPutTempComp), HTTP_PUT, "tempcomp");
    this->createCallBack(LHF(_alpacaPutHalt), HTTP_PUT, "halt");
    this->createCallBack(LHF(_alpacaPutMove), HTTP_PUT, "move");
}

void AlpacaFocuser::_alpacaGetPage(AsyncWebServerRequest *request, const char *const page)
{
    _service_counter++;
    char path[256] = {0};
    snprintf(path, sizeof(path), "%s.html", page);
    SLOG_PRINTF(SLOG_INFO, "send(LittleFS, %s)\n", path);
    request->send(LittleFS, path);
}

void AlpacaFocuser::_alpacaGetAbsolut(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_ABSOLUT
    _service_counter++;
    bool absolut = false;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        absolut = _getAbsolut();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (bool)absolut);
    DBG_END
}

void AlpacaFocuser::_alpacaGetIsMoving(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_IS_MOVING
    _service_counter++;
    bool is_moving = false;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        is_moving = _getIsMoving();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (bool)is_moving);
    DBG_END
}

void AlpacaFocuser::_alpacaGetMaxIncrement(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_MAX_INCREMENT
    _service_counter++;
    int32_t max_increment = 0.0;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        max_increment = _getMaxIncrement();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (int32_t)max_increment);
    DBG_END
}

void AlpacaFocuser::_alpacaGetMaxStep(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_MAX_STEP
    _service_counter++;
    int32_t max_step = 0.0;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        max_step = _getMaxStep();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (int32_t)max_step);
    DBG_END
}

void AlpacaFocuser::_alpacaGetPosition(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_POSITION
    _service_counter++;
    int32_t position = false;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        position = _getPosition();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (int32_t)position);
    DBG_END
}

void AlpacaFocuser::_alpacaGetStepSize(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_STEP_SIZE
    _service_counter++;
    double step_size = 0.0;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        step_size = _getStepSize();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, step_size);
    DBG_END
}

void AlpacaFocuser::_alpacaGetTempComp(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_TEMP_COMP
    _service_counter++;
    bool temp_comp = false;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        temp_comp = _getTempComp();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (bool)temp_comp);
    DBG_END
}

void AlpacaFocuser::_alpacaGetTempCompAvailable(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_TEMP_COMP_AVAILABLE
    _service_counter++;
    bool temp_comp_available = false;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        temp_comp_available = _getTempCompAvailable();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (bool)temp_comp_available);
    DBG_END
}

void AlpacaFocuser::_alpacaGetTemperature(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_TEMPERATUR
    _service_counter++;
    double temperature = false;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        temperature = _getTemperature();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (double)temperature);
    DBG_END
}

void AlpacaFocuser::_alpacaPutTempComp(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_PUT_TEMP_COMP;
    _service_counter++;
    uint32_t client_idx = 0;
    bool temp_comp = false;
    _alpaca_server->RspStatusClear(_rsp_status);

    try
    {
        if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0)
            throw(&_rsp_status);

        if (_alpaca_server->GetParam(request, "TempComp", temp_comp, Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "TempComp");

        if (!_putTempComp(temp_comp))
            _alpaca_server->ThrowRspStatusParameterInvalidValue(request, _rsp_status, "TempComp", temp_comp);
    }
    catch (AlpacaRspStatus_t *rspStatus)
    { // empty
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};

void AlpacaFocuser::_alpacaPutHalt(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_PUT_HALT;
    _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    // uint32_t id = 0;

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        _putHalt();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};

void AlpacaFocuser::_alpacaPutMove(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_PUT_MOVE;
    _service_counter++;
    uint32_t client_idx = 0;
    _alpaca_server->RspStatusClear(_rsp_status);
    int32_t position = 0;

    try
    {
        if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0)
            throw(&_rsp_status);

        if (_alpaca_server->GetParam(request, "Position", position, Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Position");

        _putMove(position);
    }
    catch (AlpacaRspStatus_t *rspStatus)
    { // empty
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};

#ifdef ALPACA_FOCUSER_PUT_ACTION_IMPLEMENTED
void AlpacaFocuser::AlpacaPutAction(AsyncWebServerRequest *request)
{
    DBG_DEVICE_PUT_ACTION_REQ;
    //_service_counter++;
    uint32_t client_idx = 0;
    _alpaca_server->RspStatusClear(_rsp_status);
    char action[64] = {0};
    char parameters[128] = {0};
    char str_response[1024] = {0};

    try
    {
        if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0 && _clients[client_idx].client_id != ALPACA_CONNECTION_LESS_CLIENT_ID)
            throw(&_rsp_status);

        if (_alpaca_server->GetParam(request, "Action", action, sizeof(action), Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Action");

        if (_alpaca_server->GetParam(request, "Parameters", parameters, sizeof(parameters), Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Action");

        if (_putAction(action, parameters, str_response, sizeof(str_response)) == false)
            _alpaca_server->ThrowRspStatusCommandStringInvalid(request, _rsp_status, parameters);

        _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, str_response, JsonValue_t::kAsPlainStringValue);
    }
    catch (AlpacaRspStatus_t *rspStatus)
    {
        _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    }
    DBG_END
};
#endif

#ifdef ALPACA_FOCUSER_PUT_COMMAND_BOOL_IMPLEMENTED
void AlpacaFocuser::AlpacaPutCommandBool(AsyncWebServerRequest *request)
{
    DBG_DEVICE_PUT_ACTION_REQ;
    _service_counter++;
    uint32_t client_idx = 0;
    _alpaca_server->RspStatusClear(_rsp_status);
    char command[64] = {0};
    char raw[16] = "true";
    bool bool_response = false;

    try
    {
        if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0)
            throw(&_rsp_status);

        if (_alpaca_server->GetParam(request, "Command", command, sizeof(command), Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Command");

        if (_alpaca_server->GetParam(request, "Raw", raw, sizeof(raw), Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Raw");

        if (_putCommandBool(command, raw, bool_response) == false)
            _alpaca_server->ThrowRspStatusCommandStringInvalid(request, _rsp_status, command);

        _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (bool)bool_response);
    }
    catch (AlpacaRspStatus_t *rspStatus)
    {
        _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    }
    DBG_END
};
#endif

#ifdef ALPACA_FOCUSER_PUT_COMMAND_STRING_IMPLEMENTED
void AlpacaFocuser::AlpacaPutCommandString(AsyncWebServerRequest *request)
{
    DBG_DEVICE_PUT_ACTION_REQ;
    _service_counter++;
    uint32_t client_idx = 0;
    _alpaca_server->RspStatusClear(_rsp_status);
    char command_str[256] = {0};
    char raw[16] = "true";
    char str_response[64] = {0};

    try
    {
        if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0)
            throw(&_rsp_status);

        if (_alpaca_server->GetParam(request, "Command", command_str, sizeof(command_str), Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Command");

        if (_alpaca_server->GetParam(request, "Raw", raw, sizeof(raw), Spelling_t::kStrict) == false)
            _alpaca_server->ThrowRspStatusParameterNotFound(request, _rsp_status, "Raw");

        if (_putCommandString(command_str, raw, str_response, sizeof(str_response)) == false)
            _alpaca_server->ThrowRspStatusCommandStringInvalid(request, _rsp_status, command_str);

        _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, str_response);
    }
    catch (AlpacaRspStatus_t *rspStatus)
    {
        _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    }
    DBG_END
};
#endif

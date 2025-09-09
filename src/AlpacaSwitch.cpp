/**************************************************************************************************
  Description:    Common ASCOM Alpaca Switch V2
  Copyright 2024-2025 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#include "AlpacaSwitch.h"

AlpacaSwitch::AlpacaSwitch(uint32_t num_of_switch_devices)
{
    _p_switch_devices = new SwitchDevice_t[num_of_switch_devices];
    _max_switch_devices = num_of_switch_devices;

    strlcpy(_device_type, ALPACA_SWITCH_DEVICE_TYPE, sizeof(_device_type));
    strlcpy(_device_description, ALPACA_SWITCH_DESCRIPTION, sizeof(_device_description));
    strlcpy(_driver_info, ALPACA_SWITCH_DRIVER_INFO, sizeof(_driver_info));
    strlcpy(_device_and_driver_version, esp32_alpaca_device_library_version, sizeof(_device_and_driver_version));
    _device_interface_version = ALPACA_SWITCH_INTERFACE_VERSION;

    // Switch device description - First initialization
    for (uint32_t u = 0; u < _max_switch_devices; u++)
    {
        _p_switch_devices[u].can_write = false;
        snprintf(_p_switch_devices[u].name, sizeof(SwitchDevice_t::name), "SwitchDevice%02d", u);
        snprintf(_p_switch_devices[u].description, sizeof(SwitchDevice_t::description), "Switch Device %02d Description", u);
        _p_switch_devices[u].min_value = 0.0;
        _p_switch_devices[u].max_value = 1.0;
        _p_switch_devices[u].value = _p_switch_devices[u].min_value;
        _p_switch_devices[u].step = 1.0;
        _InitSwitchDevicesInternals(u);
    }
}

void AlpacaSwitch::_InitSwitchDevicesInternals(uint32_t id)
{
    if (id >= 0 && id <= _max_switch_devices)
    {
        _p_switch_devices[id].is_bool = (_p_switch_devices[id].min_value == 0.0 &&
                                         _p_switch_devices[id].max_value == 1.0 &&
                                         _p_switch_devices[id].step == 1.0);
        _p_switch_devices[id].has_been_cancelled = false;
        _p_switch_devices[id].state_change_complete = true;
        _p_switch_devices[id].set_time_stamp_ms = millis();
    }
}

void AlpacaSwitch::Begin()
{
    snprintf(_device_and_driver_version, sizeof(_device_and_driver_version), "%s/%s", _getFirmwareVersion(), esp32_alpaca_device_library_version);

    // Init all switch device states maintained by driver
    for (uint32_t u = 0; u < _max_switch_devices; u++)
    {
        _InitSwitchDevicesInternals(u);
    }
    AlpacaDevice::Begin();
}

void AlpacaSwitch::RegisterCallbacks()
{
    AlpacaDevice::RegisterCallbacks();

    this->createCallBack(LHF(_alpacaGetMaxSwitch), HTTP_GET, "maxswitch");
    this->createCallBack(LHF(_alpacaGetCanWrite), HTTP_GET, "canwrite");
    this->createCallBack(LHF(_alpacaGetSwitch), HTTP_GET, "getswitch");
    this->createCallBack(LHF(_alpacaGetSwitchDescription), HTTP_GET, "getswitchdescription");
    this->createCallBack(LHF(_alpacaGetSwitchName), HTTP_GET, "getswitchname");
    this->createCallBack(LHF(_alpacaGetSwitchValue), HTTP_GET, "getswitchvalue");
    this->createCallBack(LHF(_alpacaGetMinSwitchValue), HTTP_GET, "minswitchvalue");
    this->createCallBack(LHF(_alpacaGetMaxSwitchValue), HTTP_GET, "maxswitchvalue");
    this->createCallBack(LHF(_alpacaGetSwitchStep), HTTP_GET, "switchstep");
    this->createCallBack(LHF(_alpacaGetCanAsync), HTTP_GET, "canasync");
    this->createCallBack(LHF(_alpacaGetStateChangeComplete), HTTP_GET, "statechangecomplete");

    this->createCallBack(LHF(_alpacaPutSetSwitchWrapper), HTTP_PUT, "setswitch");
    this->createCallBack(LHF(_alpacaPutSetSwitchValueWrapper), HTTP_PUT, "setswitchvalue");
    this->createCallBack(LHF(_alpacaPutSetAsyncWrapper), HTTP_PUT, "setasync");
    this->createCallBack(LHF(_alpacaPutSetAsyncValueWrapper), HTTP_PUT, "setasyncvalue");

    this->createCallBack(LHF(_alpacaPutCancleAsync), HTTP_PUT, "cancleasync");
    this->createCallBack(LHF(_alpacaPutSetSwitchName), HTTP_PUT, "setswitchname");

    this->createCallBack(LHF(AlpacaPutAction), HTTP_PUT, "action");
    this->createCallBack(LHF(AlpacaPutCommandBlind), HTTP_PUT, "commandblind");
    this->createCallBack(LHF(AlpacaPutCommandBool), HTTP_PUT, "commandbool");
    this->createCallBack(LHF(AlpacaPutCommandString), HTTP_PUT, "commandstring");
}

/**
 * @brief Handler for maxswitch
 */
void AlpacaSwitch::_alpacaGetMaxSwitch(AsyncWebServerRequest *request)
{
    DBG_SWITCH_GET_MAX_SWITCH
    _service_counter++;
    int32_t max_switch_devices = 0;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        max_switch_devices = _max_switch_devices;
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (int32_t)max_switch_devices);
    DBG_END
}

/**
 * @brief Handler for canwrite
 */
void AlpacaSwitch::_alpacaGetCanWrite(AsyncWebServerRequest *request)
{
    DBG_SWITCH_CAN_WRITE
    _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    bool can_write = false;
    uint32_t id = 0;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            can_write = _p_switch_devices[id].can_write;
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, can_write);
    DBG_END
}

/**
 * @brief Handler for getswitch
 */
void AlpacaSwitch::_alpacaGetSwitch(AsyncWebServerRequest *request)
{
    DBG_SWITCH_GET_SWITCH
    _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    bool bool_value = false;
    uint32_t id = 0;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            if (_p_switch_devices[id].has_been_cancelled == true)
            {
                MYTHROW_RspStatusOperationCancelled(request, _rsp_status, GetSwitchName(id));
            }
            bool_value = _doubleValueToBoolValue(id, _p_switch_devices[id].value);
            _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, bool_value);
            DBG_END;
            return;
        }
    }

mycatch:
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, bool_value);
    DBG_END
}

/**
 * @brief Handler for getswitchdescription
 */
void AlpacaSwitch::_alpacaGetSwitchDescription(AsyncWebServerRequest *request)
{
    DBG_SWITCH_GET_SWITCH_DESCRIPTION;
    _service_counter++;
    char description[kSwitchDescriptionSize] = {0};
    _alpaca_server->RspStatusClear(_rsp_status);
    uint32_t id = 0;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            snprintf(description, sizeof(description), "%s", (_p_switch_devices[id]).description);
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, description, JsonValue_t::kAsJsonStringValue);
    DBG_END
}

/**
 * @brief Handler for getswitchname
 */
void AlpacaSwitch::_alpacaGetSwitchName(AsyncWebServerRequest *request)
{
    DBG_SWITCH_GET_SWITCH_NAME;
    _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    char name[kSwitchNameSize] = {0};
    uint32_t id = 0;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            snprintf(name, sizeof(name), "%s", (_p_switch_devices[id]).name);
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, name, JsonValue_t::kAsJsonStringValue);
    DBG_END
}

/**
 * @brief Handler for getswitchvalue
 */
void AlpacaSwitch::_alpacaGetSwitchValue(AsyncWebServerRequest *request)
{
    DBG_SWITCH_GET_SWITCH_VALUE;
    _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    double double_value = 0.0;
    uint32_t id = 0;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            if (_p_switch_devices[id].has_been_cancelled == true)
            {
                MYTHROW_RspStatusOperationCancelled(request, _rsp_status, GetSwitchName(id));
            }
            double_value = _p_switch_devices[id].value;
            _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, double_value);
            DBG_END;
            return;
        }
    }

mycatch:
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, double_value);
    DBG_END
}

/**
 * @brief Handler for minswitchvalue
 */
void AlpacaSwitch::_alpacaGetMinSwitchValue(AsyncWebServerRequest *request)
{
    DBG_SWITCH_GET_MIN_SWITCH_VALUE;
    _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    double min_value = 0.0;
    uint32_t id = 0;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            min_value = _p_switch_devices[id].min_value;
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, min_value);
    DBG_END
}

/**
 * @brief Handler for maxswitchvalue
 */
void AlpacaSwitch::_alpacaGetMaxSwitchValue(AsyncWebServerRequest *request)
{
    DBG_SWITCH_GET_MAX_SWITCH_VALUE;
    _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    double max_value = 0.0;
    uint32_t id = 0;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            max_value = _p_switch_devices[id].max_value;
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, max_value);
    DBG_END
}

/**
 * @brief Handler for switchstep
 */
void AlpacaSwitch::_alpacaGetSwitchStep(AsyncWebServerRequest *request)
{
    DBG_SWITCH_GET_SWITCH_STEP;
    _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    double step = 0.0;
    uint32_t id = 0;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            step = _p_switch_devices[id].step;
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, step);
    DBG_END
}

/**
 * @brief Handler for canasync
 */
void AlpacaSwitch::_alpacaGetCanAsync(AsyncWebServerRequest *request)
{
    DBG_SWITCH_GET_CAN_ASYNC
    _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    bool can_async = false;
    uint32_t id = 0;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            GetCanAsync(id);
            can_async = _p_switch_devices[id].async_type == SwitchAsyncType_t::kAsyncType;
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, can_async);
    DBG_END
}

/**
 * @brief Handler for statechangecomplete
 */
void AlpacaSwitch::_alpacaGetStateChangeComplete(AsyncWebServerRequest *request)
{
    DBG_SWITCH_GET_CAN_ASYNC
    _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    bool state_change_complete = false;
    uint32_t id = 0;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            state_change_complete = _p_switch_devices[id].state_change_complete;
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, state_change_complete);
    DBG_END
}

/**
 * Common handler for setswitch, setswitchvalue, setasync, setasyncvalue
 */
void AlpacaSwitch::_alpacaPutSetSwitch(AsyncWebServerRequest *request, SwitchValueType_t value_type, SwitchAsyncType_t async_type)
{
    DBG_SWITCH_PUT_SET_SWITCH
    _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    uint32_t id = 0;
    double double_value = 0.0;
    bool bool_value = false;

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kStrict))
        {
            if (async_type == SwitchAsyncType_t::kNoAsyncType ||
                async_type == SwitchAsyncType_t::kAsyncType && (_p_switch_devices[id].async_type == SwitchAsyncType_t::kAsyncType))
            {
                if (_p_switch_devices[id].can_write)
                { 
                    if (value_type == SwitchValueType_t::kDouble)
                    {

                        if (_alpaca_server->GetParam(request, "Value", double_value, Spelling_t::kStrict))
                        {

                            if (value_type == SwitchValueType_t::kDouble)
                            {
                                if (double_value >= _p_switch_devices[id].min_value && double_value <= _p_switch_devices[id].max_value)
                                {
                                    _p_switch_devices[id].state_change_complete = async_type == SwitchAsyncType_t::kNoAsyncType;
                                    if (_writeSwitchValue(id, double_value, async_type))
                                    { // physical set was OK
                                        _p_switch_devices[id].value = double_value;
                                        _p_switch_devices[id].has_been_cancelled = false;
                                        _p_switch_devices[id].set_time_stamp_ms = millis();
                                    }
                                    else
                                    { // exc can't write
                                        _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                                        _rsp_status.http_status = HttpStatus_t::kPassed;
                                        snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - can't write %f to Switch device <%s>",
                                                 request->url().c_str(), _boolValueToDoubleValue(id, bool_value), _p_switch_devices[id].name);
                                    }
                                }

                                else
                                {
                                    _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                                    _rsp_status.http_status = HttpStatus_t::kInvalidRequest;
                                    snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - parameter \'Value\' %f not inside range (%f,..%f)",
                                             request->url().c_str(), double_value, _p_switch_devices[id].min_value, _p_switch_devices[id].max_value);
                                }
                            }
                        }

                        else
                        {
                            _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                            _rsp_status.http_status = HttpStatus_t::kInvalidRequest;
                            snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - parameter \'Value\' not found or invalid",
                                     request->url().c_str());
                        }
                    }
                    else
                    {
                        if (_alpaca_server->GetParam(request, "State", bool_value, Spelling_t::kStrict))
                        {
                            bool bool_value = _boolValueToDoubleValue(id, bool_value);
                            _p_switch_devices[id].state_change_complete = async_type == SwitchAsyncType_t::kNoAsyncType;
                            if (_writeSwitchValue(id,bool_value, async_type))
                            { // physical set was OK
                                _p_switch_devices[id].value = bool_value;
                                _p_switch_devices[id].has_been_cancelled = false;
                                _p_switch_devices[id].set_time_stamp_ms = millis();
                            }
                            else
                            { // exc can't write
                                _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                                _rsp_status.http_status = HttpStatus_t::kPassed;
                                snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - can't write %f to Switch device <%s>",
                                         request->url().c_str(), _boolValueToDoubleValue(id, bool_value), _p_switch_devices[id].name);
                            }
                        }
                        else
                        { // exc invalid
                            _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                            _rsp_status.http_status = HttpStatus_t::kInvalidRequest;
                            snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - parameter \'State\' not found or invalid",
                                     request->url().c_str());
                        }
                    }
                }
                else
                { // exc read only
                    _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                    _rsp_status.http_status = HttpStatus_t::kInvalidRequest;
                    snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - Switch device <%s> is read only",
                             request->url().c_str(), _p_switch_devices[id].name);
                }
            }
            else
            { // exc async not allowed
                _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                _rsp_status.http_status = HttpStatus_t::kPassed;
                snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - Switch device <%s> async not allowed",
                         request->url().c_str(), _p_switch_devices[id].name);
            }
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};

/**
 * @brief Handler for cancleasync
 */
void AlpacaSwitch::_alpacaPutCancleAsync(AsyncWebServerRequest *request)
{
    DBG_SWITCH_GET_CANCLE_ASYNC
    _service_counter++; 
    _alpaca_server->RspStatusClear(_rsp_status);
    uint32_t id = 0;

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            if (GetStateChangeComplete(id) == false) {
                _p_switch_devices[id].has_been_cancelled = true;
            }
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
}

/**
 * @brief Handler for setswitchname
 */
void AlpacaSwitch::_alpacaPutSetSwitchName(AsyncWebServerRequest *request)
{
    DBG_SWITCH_PUT_SET_SWITCH_NAME;
    _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    uint32_t id = 0;
    char name[kSwitchNameSize] = "";

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kStrict))
        {
            if (_alpaca_server->GetParam(request, "Name", name, sizeof(name), Spelling_t::kStrict))
            {
                SetSwitchName(id, name);
            }
            else
            {
                _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                _rsp_status.http_status = HttpStatus_t::kInvalidRequest;
                snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - parameter \'Name\' not found or invalid",
                         request->url().c_str());
            }
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};

/**
 * @brief Handler for action
 */
void AlpacaSwitch::AlpacaPutAction(AsyncWebServerRequest *request)
{
    DBG_DEVICE_PUT_ACTION_REQ;
    //_service_counter++;
    uint32_t client_idx = 0;
    _alpaca_server->RspStatusClear(_rsp_status);
    char action[64] = {0};
    char parameters[128] = {0};
    char str_response[1024] = {0};

    if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0 && _clients[client_idx].client_id != ALPACA_CONNECTION_LESS_CLIENT_ID)
        goto mycatch;

    if (_alpaca_server->GetParam(request, "Action", action, sizeof(action), Spelling_t::kStrict) == false)
        MYTHROW_RspStatusParameterNotFound(request, _rsp_status, "Action");

    if (_alpaca_server->GetParam(request, "Parameters", parameters, sizeof(parameters), Spelling_t::kStrict) == false)
        MYTHROW_RspStatusParameterNotFound(request, _rsp_status, "Action");

    if (_putAction(action, parameters, str_response, sizeof(str_response)) == false)
        MYTHROW_RspStatusCommandStringInvalid(request, _rsp_status, parameters);

    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, str_response, JsonValue_t::kAsPlainStringValue);

    DBG_END;
    return;

mycatch:
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
};

/**
 * @brief Handler for commandblind
 */
void AlpacaSwitch::AlpacaPutCommandBlind(AsyncWebServerRequest *request)
{
    DBG_DEVICE_PUT_ACTION_REQ;
    _service_counter++;
    uint32_t client_idx = 0;
    _alpaca_server->RspStatusClear(_rsp_status);
    char command[64] = {0};
    char raw[16] = "true";
    bool bool_response = false;

    if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0)
        goto mycatch;

    if (_alpaca_server->GetParam(request, "Command", command, sizeof(command), Spelling_t::kStrict) == false)
        MYTHROW_RspStatusParameterNotFound(request, _rsp_status, "Command");

    if (_alpaca_server->GetParam(request, "Raw", raw, sizeof(raw), Spelling_t::kStrict) == false)
        MYTHROW_RspStatusParameterNotFound(request, _rsp_status, "Raw");

    if (_putCommandBlind(command, raw, bool_response) == false)
        MYTHROW_RspStatusCommandStringInvalid(request, _rsp_status, command);

    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (bool)bool_response);

    DBG_END;
    return;

mycatch:
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);

    DBG_END
};

/**
 * @brief Handler for commandbool
 */
void AlpacaSwitch::AlpacaPutCommandBool(AsyncWebServerRequest *request)
{
    DBG_DEVICE_PUT_ACTION_REQ;
    _service_counter++;
    uint32_t client_idx = 0;
    _alpaca_server->RspStatusClear(_rsp_status);
    char command[64] = {0};
    char raw[16] = "true";
    bool bool_response = false;

    if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0)
        goto mycatch;

    if (_alpaca_server->GetParam(request, "Command", command, sizeof(command), Spelling_t::kStrict) == false)
        MYTHROW_RspStatusParameterNotFound(request, _rsp_status, "Command");

    if (_alpaca_server->GetParam(request, "Raw", raw, sizeof(raw), Spelling_t::kStrict) == false)
        MYTHROW_RspStatusParameterNotFound(request, _rsp_status, "Raw");

    if (_putCommandBool(command, raw, bool_response) == false)
        MYTHROW_RspStatusCommandStringInvalid(request, _rsp_status, command);

    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (bool)bool_response);

    DBG_END;
    return;

mycatch:
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);

    DBG_END
};

/**
 * @brief Handler for commandstring
 */
void AlpacaSwitch::AlpacaPutCommandString(AsyncWebServerRequest *request)
{
    DBG_DEVICE_PUT_ACTION_REQ;
    _service_counter++;
    uint32_t client_idx = 0;
    _alpaca_server->RspStatusClear(_rsp_status);
    char command_str[256] = {0};
    char raw[16] = "true";
    char str_response[64] = {0};

    if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0)
        goto mycatch;

    if (_alpaca_server->GetParam(request, "Command", command_str, sizeof(command_str), Spelling_t::kStrict) == false)
        MYTHROW_RspStatusParameterNotFound(request, _rsp_status, "Command");

    if (_alpaca_server->GetParam(request, "Raw", raw, sizeof(raw), Spelling_t::kStrict) == false)
        MYTHROW_RspStatusParameterNotFound(request, _rsp_status, "Raw");

    if (_putCommandString(command_str, raw, str_response, sizeof(str_response)) == false)
        MYTHROW_RspStatusCommandStringInvalid(request, _rsp_status, command_str);

    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, str_response);

    DBG_END;
    return;

mycatch:
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};

bool const AlpacaSwitch::_getDeviceStateList(size_t buf_len, char *buf)
{
    size_t snprintf_result = 0;
    size_t len = 0;
    const size_t max_json_len = 128;    // max len of one json element

    for (unsigned id = 0; id < GetMaxSwitch(); id++)
    {
        if (GetStateChangeComplete(id))
        {
            len = strlen(buf);
            if (buf_len - len > max_json_len) 
            {
                snprintf_result = snprintf(buf + len, buf_len - len - 1, "{\"Name\":\"GetSwitch%d\",\"Value\":%s},", id, GetValue(id) ? "true" : "false");
            }
            else
            {
                snprintf_result = 0;
                break;
            }

            len = strlen(buf);
            if (buf_len - len > max_json_len) 
            {
                snprintf_result = snprintf(buf + len, buf_len - len - 1, "{\"Name\":\"GetSwitchValue%d\",\"Value\":%f},", id, GetSwitchValue(id));
            }
            else
            {
                snprintf_result = 0;
                break;
            }


            // if (buf_len - len > max_json_len) 
            // {
            //     if (GetIsBool(id))
            //         snprintf_result = snprintf(buf + len, buf_len - len - 1, "{\"Name\":\"GetSwitch%d\",\"Value\":%s},", id, GetValue(id) ? "true" : "false");
            //     else
            //         snprintf_result = snprintf(buf + len, buf_len - len - 1, "{\"Name\":\"GetSwitchValue%d\",\"Value\":%f},", id, GetSwitchValue(id));
            // }
            // else
            // {
            //     snprintf_result = 0;
            //     break;
            // }

            len = strlen(buf); 
            if (buf_len - len > max_json_len) 
            {
                snprintf_result = snprintf(buf + len, buf_len - len - 1, "{\"Name\":\"StateChangeComplete%d\",\"Value\":%s},", id, GetStateChangeComplete(id) ? "true" : "false");
            }
            else
            {
                snprintf_result = 0;
                break;
            }
            // 
            // if (buf_len - len > max_json_len) 
            // {
            //     snprintf_result = snprintf(buf + len, buf_len - len - 1, "{\"Name\":\"SwitchName%d\",\"Value\":\"%s\"},", id, GetSwitchName(id));
            // }
            // else
            // {
            //     snprintf_result = 0;
            //     break;
            // } 

        }

    }
    len = strlen(buf);
    *(buf + len - 1) = '\0'; // replace ',' by '\0'

    return (snprintf_result > 0 && snprintf_result <= buf_len);
}

bool AlpacaSwitch::_getAndCheckId(AsyncWebServerRequest *request, uint32_t &id, Spelling_t spelling)
{
    const char k_id[] = "Id";
    if (_alpaca_server->GetParam(request, k_id, id, spelling))
    {
        if (id >= 0 && id < _max_switch_devices)
        {
            return true;
        }
        else
        {
            _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
            _rsp_status.http_status = HttpStatus_t::kInvalidRequest;
            snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - Parameter '%s=%d invalid", request->url().c_str(), k_id, id);
            return false;
        }
    }
    else
    {
        _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
        _rsp_status.http_status = HttpStatus_t::kInvalidRequest;
        snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - Parameter '%s' not found", request->url().c_str(), k_id);
        return false;
    }
}

/*
 * Set switch device value (bool)
 */
const bool AlpacaSwitch::SetSwitch(uint32_t id, bool bool_value)
{
    if (id < _max_switch_devices)
    {
        _p_switch_devices[id].value = bool_value ? _p_switch_devices[id].max_value : _p_switch_devices[id].min_value;
        _p_switch_devices[id].has_been_cancelled = false;
        _p_switch_devices[id].state_change_complete = true;
        _p_switch_devices[id].set_time_stamp_ms = millis();
        return true;
    }
    return false;
};

/*
 * Set switch device value (double) if in range; Consider steps.
 */
const bool AlpacaSwitch::SetSwitchValue(uint32_t id, double double_value)
{
    if (id < _max_switch_devices)
    {
        if (double_value >= _p_switch_devices[id].min_value && double_value <= _p_switch_devices[id].max_value)
        {
            int32_t steps = (double_value - _p_switch_devices[id].min_value) / _p_switch_devices[id].step + 0.5 * _p_switch_devices[id].step;
            _p_switch_devices[id].value = _p_switch_devices[id].min_value + (double)steps * _p_switch_devices[id].step;
            _p_switch_devices[id].value = _p_switch_devices[id].value <= _p_switch_devices[id].max_value ? _p_switch_devices[id].value : _p_switch_devices[id].max_value;
            _p_switch_devices[id].has_been_cancelled = false;
            _p_switch_devices[id].state_change_complete = true;
            _p_switch_devices[id].set_time_stamp_ms = millis();            
            return true;
        }
    }
    return false;
};

const bool AlpacaSwitch::SetSwitchName(uint32_t id, char *name)
{
    if (id < _max_switch_devices)
    {
        strlcpy(_p_switch_devices[id].name, name, kSwitchNameSize);
        return true;
    }
    return false;
};

/*
 * Set switch device value (bool)
 */
const bool AlpacaSwitch::SetStateChangeComplete(uint32_t id, bool state_change_complete)
{
    if (id < _max_switch_devices)
    {
        _p_switch_devices[id].state_change_complete = state_change_complete;
        return true;
    }
    return false;
};
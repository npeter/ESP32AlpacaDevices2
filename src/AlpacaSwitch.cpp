/**************************************************************************************************
  Filename:       AlpacaSwitch.cpp
  Revised:        $Date: 2024-01-28$
  Revision:       $Revision: 01 $
  Description:    Common ASCOM Alpaca Switch V2

  Copyright 2024 peter_n@gmx.de. All rights reserved.
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

    //_max_switch_devices = kSwitchMaxSwitchDevices;
    for (uint32_t i = 0; i < _max_switch_devices; i++)
    {
        _p_switch_devices[i].can_write = false;
        snprintf(_p_switch_devices[i].name, sizeof(SwitchDevice_t::name), "SwitchDevice%02d", i);
        snprintf(_p_switch_devices[i].description, sizeof(SwitchDevice_t::description), "Switch Device %02d Description", i);
        _p_switch_devices[i].min_value = 0.0;
        _p_switch_devices[i].max_value = 1.0;
        _p_switch_devices[i].value = _p_switch_devices[i].min_value;
        _p_switch_devices[i].step = 1.0;
    }
}

void AlpacaSwitch::Begin()
{
    snprintf(_device_and_driver_version, sizeof(_device_and_driver_version), "%s/%s", _getFirmwareVersion(), esp32_alpaca_device_library_version);    
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

    this->createCallBack(LHF(_alpacaPutSetSwitch), HTTP_PUT, "setswitch");
    this->createCallBack(LHF(_alpacaPutSetSwitchName), HTTP_PUT, "setswitchname");
    this->createCallBack(LHF(_alpacaPutSetSwitchValue), HTTP_PUT, "setswitchvalue");
}

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

void AlpacaSwitch::_alpacaGetSwitch(AsyncWebServerRequest *request)
{
    DBG_SWITCH_GET_SWITCH
        _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    bool switch_value = false;
    uint32_t id = 0;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            switch_value = _doubleValueToBoolValue(id, _p_switch_devices[id].value);
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, switch_value);
    DBG_END
}

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
            snprintf(description, sizeof(description),"%s", (_p_switch_devices[id]).description);
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, description, JsonValue_t::kAsJsonStringValue);
    DBG_END
}

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
            snprintf(name, sizeof(name),"%s", (_p_switch_devices[id]).name);            
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, name, JsonValue_t::kAsJsonStringValue);
    DBG_END
}

void AlpacaSwitch::_alpacaGetSwitchValue(AsyncWebServerRequest *request)
{
    DBG_SWITCH_GET_SWITCH_VALUE;
        _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    double value = 0.0;
    uint32_t id = 0;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            value =  _p_switch_devices[id].value;
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, value);
    DBG_END
}

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

void AlpacaSwitch::_alpacaPutSetSwitch(AsyncWebServerRequest *request)
{
    DBG_SWITCH_PUT_SET_SWITCH;
        _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    uint32_t id = 0;
    bool bool_value;

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            if (_alpaca_server->GetParam(request, "State", bool_value, Spelling_t::kIgnoreCase))
            {
                if (_p_switch_devices[id].can_write)
                {
                    _p_switch_devices[id].value = _boolValueToDoubleValue(id, bool_value);
                    if (!_writeSwitchValue(id, _p_switch_devices[id].value))
                    {
                        _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                        _rsp_status.http_status = HttpStatus_t::kPassed;
                        snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - can't write %f to Switch device <%s>",
                                 request->url().c_str(), _boolValueToDoubleValue(id, bool_value), _p_switch_devices[id].name);
                    }
                }
                else
                {
                    _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                    _rsp_status.http_status = HttpStatus_t::kPassed;
                    snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - Switch device <%s> is read only",
                             request->url().c_str(), _p_switch_devices[id].name);
                }
            }
            else
            {
                _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                _rsp_status.http_status = HttpStatus_t::kPassed;
                snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - parameter \"State\" not found or invalid",
                         request->url().c_str());
            }
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};

void AlpacaSwitch::_alpacaPutSetSwitchName(AsyncWebServerRequest *request)
{
    DBG_SWITCH_PUT_SET_SWITCH_NAME;
        _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    uint32_t id = 0;
    char name[kSwitchNameSize] = "";

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            if (_alpaca_server->GetParam(request, "Name", name, sizeof(name), Spelling_t::kIgnoreCase))
            {
                SetSwitchName(id, name);
            }
            else
            {
                _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                _rsp_status.http_status = HttpStatus_t::kPassed;
                snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - parameter \"Name\" not found or invalid",
                         request->url().c_str());
            }
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};

void AlpacaSwitch::_alpacaPutSetSwitchValue(AsyncWebServerRequest *request)
{
    DBG_SWITCH_PUT_SET_SWITCH_VALUE;
        _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    uint32_t id = 0;
    double double_value;

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        if (_getAndCheckId(request, id, Spelling_t::kIgnoreCase))
        {
            if (_alpaca_server->GetParam(request, "Value", double_value, Spelling_t::kIgnoreCase))
            {
                if (_p_switch_devices[id].can_write)
                {

                    if (double_value >= _p_switch_devices[id].min_value && double_value <= _p_switch_devices[id].max_value)
                    {
                        SetSwitchValue(id, double_value);
                        if (!_writeSwitchValue(id, _p_switch_devices[id].value))
                        {
                            _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                            _rsp_status.http_status = HttpStatus_t::kPassed;
                            snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - can't write %f to Switch device <%s>",
                                     request->url().c_str(), double_value, _p_switch_devices[id].name);
                        }
                    }
                    else
                    {
                        _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                        _rsp_status.http_status = HttpStatus_t::kPassed;
                        snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - parameter \"Value\" %f not inside range (%f,..%f)",
                                 request->url().c_str(), double_value, _p_switch_devices[id].min_value, _p_switch_devices[id].max_value);
                    }
                }
                else
                {
                    _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                    _rsp_status.http_status = HttpStatus_t::kPassed;
                    snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - Switch device <%s> is read only",
                             request->url().c_str(), _p_switch_devices[id].name);
                }
            }
            else
            {
                _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
                _rsp_status.http_status = HttpStatus_t::kPassed;
                snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - parameter \"Value\" not found or invalid",
                         request->url().c_str());
            }
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};

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
            _rsp_status.http_status = HttpStatus_t::kPassed;
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
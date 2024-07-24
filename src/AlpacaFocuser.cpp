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

    this->createCallBack(LHF(_alpacaGetAbsolut), HTTP_GET, "absolute", false);
    this->createCallBack(LHF(_alpacaGetIsMoving), HTTP_GET, "is moving", false);
    this->createCallBack(LHF(_alpacaGetMaxIncrement), HTTP_GET, "maxincrement", false);
    this->createCallBack(LHF(_alpacaGetMaxStep), HTTP_GET, "maxstep", false);
    this->createCallBack(LHF(_alpacaGetPosition), HTTP_GET, "position", false);
    this->createCallBack(LHF(_alpacaGetStepSize), HTTP_GET, "stepsize", false);
    this->createCallBack(LHF(_alpacaGetTempComp), HTTP_GET, "tempcomp", false);
    this->createCallBack(LHF(_alpacaGetTempCompAvailable), HTTP_GET, "tempcompavailable", false);
    this->createCallBack(LHF(_alpacaGetTemperature), HTTP_GET, "temperatue", false);

    this->createCallBack(LHF(_alpacaPutTempComp), HTTP_PUT, "tempcomp", false);
    this->createCallBack(LHF(_alpacaPutHalt), HTTP_PUT, "halt", false);
    this->createCallBack(LHF(_alpacaPutMove), HTTP_PUT, "move", false);
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
        absolut = GetAbsolut();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (bool)absolut);
    DBG_END
}

void AlpacaFocuser::_alpacaGetIsMoving(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_ABSOLUT
    _service_counter++;
    bool is_moving = false;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        is_moving = GetIsMoving();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (bool)is_moving);
    DBG_END
}

void AlpacaFocuser::_alpacaGetMaxIncrement(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_ABSOLUT
    _service_counter++;
    int32_t max_increment = 0.0;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        max_increment = GetMaxIncrement();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (int32_t)max_increment);
    DBG_END
}

void AlpacaFocuser::_alpacaGetMaxStep(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_ABSOLUT
    _service_counter++;
    int32_t max_step = 0.0;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        max_step = GetMaxStep();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (int32_t)max_step);
    DBG_END
}

void AlpacaFocuser::_alpacaGetPosition(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_ABSOLUT
    _service_counter++;
    int32_t position = false;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        position = GetPosition();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (int32_t)position);
    DBG_END
}

void AlpacaFocuser::_alpacaGetStepSize(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_ABSOLUT
    _service_counter++;
    int32_t step_size = false;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        step_size = GetStepSize();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (int32_t)step_size);
    DBG_END
}

void AlpacaFocuser::_alpacaGetTempComp(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_ABSOLUT
    _service_counter++;
    bool temp_comp = false;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        temp_comp = GetTempComp();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (bool)temp_comp);
    DBG_END
}

void AlpacaFocuser::_alpacaGetTempCompAvailable(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_ABSOLUT
    _service_counter++;
    bool temp_comp_available = false;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        temp_comp_available = GetTempCompAvailable();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (bool)temp_comp_available);
    DBG_END
}

void AlpacaFocuser::_alpacaGetTemperature(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_GET_ABSOLUT
    _service_counter++;
    double temperature = false;
    _alpaca_server->RspStatusClear(_rsp_status);

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        temperature = GetTemperature();
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, (double)temperature);
    DBG_END
}

void AlpacaFocuser::_alpacaPutTempComp(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_PUT_TEMP_COMP;
    _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    uint32_t id = 0;
    bool temp_comp = false;

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        // TODO check temp_comp_available
        if (_alpaca_server->GetParam(request, "TempComp", temp_comp, Spelling_t::kIgnoreCase))
        {
            _putTempComp(temp_comp);
        }
        else
        {
            _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
            _rsp_status.http_status = HttpStatus_t::kPassed;
            snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - parameter \"Name\" not found or invalid",
                     request->url().c_str());
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};

void AlpacaFocuser::_alpacaPutHalt(AsyncWebServerRequest *request)
{
    DBG_FOCUSER_PUT_TEMP_COMP;
    _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    uint32_t id = 0;

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
    DBG_FOCUSER_PUT_TEMP_COMP;
    _service_counter++;
    _alpaca_server->RspStatusClear(_rsp_status);
    uint32_t id = 0;
    int32_t position = 0;

    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    if (client_idx > 0)
    {
        // TODO check temp_comp_available
        if (_alpaca_server->GetParam(request, "Position", position, Spelling_t::kIgnoreCase))
        {
            _putMove(position);
        }
        else
        {
            _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
            _rsp_status.http_status = HttpStatus_t::kPassed;
            snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - parameter \"Name\" not found or invalid",
                     request->url().c_str());
        }
    }
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
};

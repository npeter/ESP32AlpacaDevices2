/**************************************************************************************************
  Filename:       CoverCalibrator.cpp
  Revised:        $Date: 2024-01-14$
  Revision:       $Revision: 01 $
  Description:    ASCOM CoverCalibrator Device Teplate with Simulation

  Copyright 2024-2025 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#include "CoverCalibrator.h"

const bool COVER_AVAILABLE = true;
const bool CALIBRATOR_AVAILABLE = true;
const int k_open_duration_ms = 3000;
const int k_close_duration_ms = 2000;

const char *const CoverCalibrator::k_device_state_str[7] = {"Init", "Closed", "Opens", "Open", "Closes", "Stopped", "invalid"};

CoverCalibrator::CoverCalibrator() : AlpacaCoverCalibrator()
{
    SetBrightness(0);
    SetMaxBrightness(ALPACA_COVER_CALIBRATOR_MAX_BRIGHTNESS);
    SetCalibratorState(CALIBRATOR_AVAILABLE ? AlpacaCalibratorStatus_t::kOff : AlpacaCalibratorStatus_t::kNotPresent);
    SetCoverState(COVER_AVAILABLE ? AlpacaCoverStatus_t::kClosed : AlpacaCoverStatus_t::kNotPresent);
};

void CoverCalibrator::Begin()
{
    AlpacaCoverCalibrator::Begin();
}

void CoverCalibrator::Loop()
{
    if (COVER_AVAILABLE)
        _coverDeviceLoop();

    if (CALIBRATOR_AVAILABLE)
        _calibratorDeviceLoop();
}

// private Methodes ==========================================================
const char *const CoverCalibrator::getDeviceStateStr(DeviceState_t state)
{
    int32_t i_state = ((int32_t)state >= 0 && (int32_t)state <= ((int32_t)DeviceState_t::kInvalid)) ?: (int32_t)DeviceState_t::kInvalid;
    return ""; // CoverCalibrator::k_device_state_str[i_state];
};

void CoverCalibrator::AlpacaReadJson(JsonObject &root)
{
    DBG_JSON_PRINTFJ(SLOG_INFO, root, "BEGIN (root=<%s>) ...\n", _ser_json_);

    AlpacaCoverCalibrator::AlpacaReadJson(root);
    if (JsonObject obj_config = root["Configuration"])
    {
        SetMaxBrightness(obj_config["MaxBrightness"] | GetMaxBrightness());
        SLOG_PRINTF(SLOG_INFO, "... END  _max_brightness=%d\n", GetMaxBrightness());
    }
    else
    {
        SLOG_PRINTF(SLOG_WARNING, "... END    ... no Calibrator Configuration\n");
    }
}

void CoverCalibrator::AlpacaWriteJson(JsonObject &root)
{
    SLOG_PRINTF(SLOG_INFO, "BEGIN ...\n");
    AlpacaCoverCalibrator::AlpacaWriteJson(root);

    JsonObject obj_config = root["Configuration"].to<JsonObject>();
    obj_config["MaxBrightness"] = GetMaxBrightness();

    // #add # for read only
    JsonObject obj_states = root["#States"].to<JsonObject>();
    obj_states["CalibratorState"] = GetAlpacaCalibratorStatusStr(GetCalibratorState());
    obj_states["CoverState"] = GetAlpacaCoverStatusStr(GetCoverState());

    DBG_JSON_PRINTFJ(SLOG_NOTICE, root, "... END root=<%s>\n", _ser_json_);
}

// Logical CalibratorDevice
const bool CoverCalibrator::_calibratorOff()
{
    SetCalibratorState(AlpacaCalibratorStatus_t::kOff);
    SetBrightness(0);
    return true;
}

const bool CoverCalibrator::_calibratorOn(int32_t brightness)
{
    if (brightness >= 0 && brightness <= GetMaxBrightness())
    {
        AlpacaCalibratorStatus_t
            _setCalibratorState(AlpacaCalibratorStatus_t::kReady);
        SetBrightness(brightness);
        return true;
    }
    else
    {
        SetCalibratorState(AlpacaCalibratorStatus_t::kReady);
        return false;
    }
}

void CoverCalibrator::_calibratorDeviceLoop()
{
    // empty
}

const bool CoverCalibrator::_closeCover()
{
    if (_state == DeviceState_t::kOpen || _state == DeviceState_t::kStopped)
    {
        _startTimeMs = millis();
        SetCoverState(AlpacaCoverStatus_t::kMoving);
        _state = DeviceState_t::kCloses;
        return true;
    }
    return false;
}

const bool CoverCalibrator::_openCover()
{
    if (_state == DeviceState_t::kClosed || _state == DeviceState_t::kStopped)
    {
        _startTimeMs = millis();
        SetCoverState(AlpacaCoverStatus_t::kMoving);
        _state = DeviceState_t::kOpens;
        return true;
    }
    return false;
}

const bool CoverCalibrator::_haltCover()
{
    _cover_halt_event = true;
    return true;
}

#ifdef ALPACA_COVER_CALIBRATOR_PUT_ACTION_IMPLEMENTED
const bool CoverCalibrator::_putAction(const char *const action, const char *const parameters)
{
    bool result = true;

    SLOG_PRINTF(SLOG_NOTICE, "action: <%s>  parameters: <%s>  result: <%s>\n", action, parameters, result ? "true" : "false");
    return result;
}
#endif

#ifdef ALPACA_COVER_CALIBRATOR_PUT_COMMAND_BLIND_IMPLEMENTED
const bool CoverCalibrator::_putCommandBlind(const char *const command, const char *const raw)
{

    bool result = true;

    SLOG_PRINTF(SLOG_NOTICE, "command: <%s>  raw: <%s>  result:<%s>\n", command, raw, result ? "true" : "false");
    return result;
}
#endif

#ifdef ALPACA_COVER_CALIBRATOR_PUT_COMMAND_BOOL_IMPLEMENTED
const bool CoverCalibrator::_putCommandBool(const char *const command, const char *const raw, bool &bool_response)
{

    bool_response = true;
    bool result = true;

    SLOG_PRINTF(SLOG_NOTICE, "command: <%s>  raw: <%s>  bool_response: <%s>  result:<%s>\n", command, raw, bool_response ? "true" : "false", result ? "true" : "false");
    return result;
}
#endif

#ifdef ALPACA_COVER_CALIBRATOR_PUT_COMMAND_STRING_IMPLEMENTED
const bool CoverCalibrator::_putCommandString(const char *const command, const char *const raw, char *string_response, size_t string_response_size)
{

    strncpy(string_response, "Hello astronomer", string_response_size);
    bool result = true;

    SLOG_PRINTF(SLOG_NOTICE, "command: <%s>  raw: <%s>  string_response: <%s>  result:<%s>\n", command, raw, string_response, result ? "true" : "false");
    return result;
}
#endif

void CoverCalibrator::_coverDeviceLoop()
{
    // DBG_COVERCALIBRATOR_SM;
    switch (_state)
    {
    case DeviceState_t::kInit:
        _cover_close_event = false;
        _cover_halt_event = false;
        _cover_open_event = false;
        _startTimeMs = millis();
        SetCoverState(AlpacaCoverStatus_t::kMoving);
        _old_state = _state;
        _state = DeviceState_t::kCloses;
        break;

    case DeviceState_t::kClosed:
        _cover_halt_event = false;
        if (_cover_open_event)
        {
            _cover_open_event = false;
            _startTimeMs = millis();
            SetCoverState(AlpacaCoverStatus_t::kMoving);
            _old_state = _state;
            _state = DeviceState_t::kOpens;
        }
        break;

    case DeviceState_t::kOpens:
        if ((millis() - _startTimeMs) >= k_open_duration_ms)
        {
            SetCoverState(AlpacaCoverStatus_t::kOpen);
            _old_state = _state;
            _state = DeviceState_t::kOpen;
        }
        else if (_cover_halt_event)
        {
            _cover_halt_event = false;
            SetCoverState(AlpacaCoverStatus_t::kUnknown);
            _old_state = _state;
            _state == DeviceState_t::kStopped;
        }
        break;

    case DeviceState_t::kOpen:
        _cover_halt_event = false;
        if (_cover_close_event)
        {
            _startTimeMs = millis();
            SetCoverState(AlpacaCoverStatus_t::kMoving);
            _old_state = _state;
            _state = DeviceState_t::kCloses;
        }
        break;

    case DeviceState_t::kCloses:
        if ((millis() - _startTimeMs) >= k_close_duration_ms)
        {
            SetCoverState(AlpacaCoverStatus_t::kClosed);
            _old_state = _state;
            _state = DeviceState_t::kClosed;
        }
        else if (_cover_halt_event)
        {
            _cover_halt_event = false;
            SetCoverState(AlpacaCoverStatus_t::kUnknown);
            _old_state = _state;
            _state == DeviceState_t::kStopped;
        }
        break;

    case DeviceState_t::kStopped:
        _cover_halt_event = false;
        if (_cover_open_event)
        {
            _startTimeMs = millis();
            SetCoverState(AlpacaCoverStatus_t::kMoving);
            _old_state = _state;
            _state = DeviceState_t::kOpens;
        }
        else if (_cover_close_event)
        {
            _startTimeMs = millis();
            SetCoverState(AlpacaCoverStatus_t::kMoving);
            _old_state = _state;
            _state = DeviceState_t::kCloses;
        }
        _cover_close_event = false;
        _cover_open_event = false;
        break;

    default:
        break;
    }
    // DBG_END
}

/**************************************************************************************************
  Filename:       Focuser.cpp
  Revised:        $Date: 2024-07-24$
  Revision:       $Revision: 01 $
  Description:    ASCOM Alpaca Focuser Device Template

  Copyright 2024-2025 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#include "Focuser.h"

const char *const Focuser::_k_sim_state_str[4] = {"INIT", "STOPPED", "MOVING", "STOPPING"};

Focuser::Focuser() : AlpacaFocuser()
{
}

void Focuser::Begin()
{
    // init Focuser

    AlpacaFocuser::Begin();
}

void Focuser::Loop()
{
    if (millis() - _sim_last_lopp_time_ms >= 1000)
    {
        _sim_last_lopp_time_ms = millis();
        _simMachine();
    }
}

void Focuser::AlpacaReadJson(JsonObject &root)
{
    DBG_JSON_PRINTFJ(SLOG_NOTICE, root, "BEGIN (root=<%s>) ...\n", _ser_json_);

    AlpacaFocuser::AlpacaReadJson(root);
    if (JsonObject obj_config = root["FocuserConfiguration"])
    {
        int32_t max_motor_step = obj_config["MaxMotor"] | _max_motor_step;
        int32_t max_focuser_mm = obj_config["MaxFocuser"] | _max_focuser_mm;

        _max_motor_step = (max_motor_step > k_max_motor_step_min) && (max_motor_step <= k_max_motor_step_max) ? max_motor_step : _max_motor_step;
        _max_focuser_mm = (max_focuser_mm > k_max_focuser_mm_min) && (max_focuser_mm <= k_max_focuser_mm_max) ? max_focuser_mm : _max_focuser_mm;
        _step_size = (double)_max_focuser_mm / (double)_max_motor_step;
        _max_increment = _max_motor_step;

        SLOG_PRINTF(SLOG_INFO, "... END  _max_motor_step=%d _max_focuser_mm=%d _step_size=%8.3f\n", _max_motor_step, _max_focuser_mm, _step_size);
    }
    else
    {
        SLOG_PRINTF(SLOG_WARNING, "... END no configuration\n");
    }
}

void Focuser::AlpacaWriteJson(JsonObject &root)
{
    SLOG_PRINTF(SLOG_NOTICE, "BEGIN ...\n");
    AlpacaFocuser::AlpacaWriteJson(root);

    // Config
    JsonObject obj_config = root["FocuserConfiguration"].to<JsonObject>();
    obj_config["MaxMotor"] = _max_motor_step;
    obj_config["MaxFocuser"] = _max_focuser_mm;

    // #add # for read only
    JsonObject obj_states = root["#States"].to<JsonObject>();
    obj_states["IsMoving"] = _is_moving;
    obj_states["Position"] = _position_steps;
    obj_states["TargetPos"] = _sim_target_position_steps;
    obj_states["Temperature"] = _temperature;
    obj_states["StepSize"] = _step_size;
    obj_states["MaxIncrement"] = _max_increment;
    obj_states["TempCompAvailable"] = _temp_comp_available;
    obj_states["TempComp"] = _temp_comp;

    DBG_JSON_PRINTFJ(SLOG_NOTICE, root, "... END root=<%s>\n", _ser_json_);
}

const bool Focuser::_putMove(int32_t target_position_steps)
{
    bool result = false;

    if (_sim_state == FocuserState_t::kStopped)
    {
        if (target_position_steps >= 0 && target_position_steps <= _max_motor_step)
        {
            _sim_target_position_steps = target_position_steps;
            _is_moving = true;

            result = true;
        }
        else
        {
            // wrong state
        }
    }
    return result;
}

const bool Focuser::_putHalt()
{
    bool result = false;

    if (_sim_state == FocuserState_t::kMoving || _sim_state == FocuserState_t::kStopping)
    {
        _position_steps = _sim_target_position_steps;
        result = true;
    }

    return result;
}

void Focuser::_simMachine()
{
    FocuserState_t old_state = _sim_state;

    _temperature = 20.0 + (double)random(0, 99) / 100.0;

    switch (_sim_state)
    {
    case FocuserState_t::kInit:
        _sim_target_position_steps = _position_steps;
        _is_moving = false;
        _sim_state = FocuserState_t::kStopped;
        break;

    case FocuserState_t::kStopped:
        if (_sim_target_position_steps != _position_steps)
        {
            _sim_state = FocuserState_t::kMoving;
            _is_moving = true;
        }
        else
        {
            _is_moving = false;
        }
        break;

    case FocuserState_t::kMoving:

        if (_sim_target_position_steps != _position_steps)
        {
            _is_moving = true;
            if (_sim_target_position_steps > _position_steps)
            {
                _position_steps = _position_steps + _sim_max_speed_steps;
                if (_position_steps >= _sim_target_position_steps)
                {
                    _position_steps = _sim_target_position_steps;
                    _sim_state = FocuserState_t::kStopping;
                }
            }
            else
            {
                _position_steps = _position_steps - _sim_max_speed_steps;
                if (_position_steps <= _sim_target_position_steps)
                {
                    _position_steps = _sim_target_position_steps;
                    _sim_state = FocuserState_t::kStopping;
                }
            }
        }
        else
        {
            _sim_state = FocuserState_t::kStopping;
        }
        break;

    case FocuserState_t::kStopping:
        _position_steps = _sim_target_position_steps;
        _is_moving = false;
        _sim_state = FocuserState_t::kStopped;
        break;

    defaul:
        break;
    }

    if (old_state != _sim_state)
    {
        SLOG_PRINTF(SLOG_INFO, "state: %s -> %s\n", _simState2Str(old_state), _simState2Str(_sim_state));
    }
}
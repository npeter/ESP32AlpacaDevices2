/**************************************************************************************************
  Filename:       Focuser.h
  Revised:        $Date: 2024-07-24$
  Revision:       $Revision: 01 $
  Description:    ASCOM Alpaca Focuser Device Template

  Copyright 2024-2025 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#pragma once
#include "AlpacaFocuser.h"

enum struct FocuserState_t
{
  kInit = 0,
  kStopped,
  kMoving,
  kStopping
};

class Focuser : public AlpacaFocuser
{
private:
  const bool _k_temp_comp_available = false;
  const int32_t k_max_motor_step_min = 1000;
  const int32_t k_max_motor_step_max = 1000000;
  const int32_t k_max_motor_step_default = k_max_motor_step_min;

  const int32_t k_max_focuser_mm_min = 10;
  const int32_t k_max_focuser_mm_max = 100;
  const int32_t k_max_focuser_mm_default = k_max_focuser_mm_min;

  const int32_t k_max_increment_min = k_max_motor_step_min;
  const int32_t k_max_increment_max = k_max_motor_step_max;
  const int32_t k_max_increment_default = k_max_motor_step_min;

  const double k_step_size_min = 0.1;
  const double k_step_size_max = 100.0;
  const double k_step_size_default = k_step_size_min;

  static const char *const _k_sim_state_str[4];

  int32_t _max_motor_step = k_max_motor_step_default; // Max. motor position / steps
  int32_t _max_focuser_mm = k_max_focuser_mm_default; // Max. focuser position / mm
  int32_t _max_increment = k_max_increment_default;
  double _step_size = _max_focuser_mm / _max_motor_step;

  const bool _absolut = true;
  bool _temp_comp = false;
  const bool _temp_comp_available = false;
  double _temperature = 99.0;

  bool _is_moving = false;
  int32_t _position_steps = 0;

  FocuserState_t _sim_state = FocuserState_t::kInit;
  int32_t _sim_max_speed_steps = 10;
  int32_t _sim_target_position_steps = _position_steps;
  uint32_t _sim_last_lopp_time_ms = 0;

  void AlpacaReadJson(JsonObject &root);
  void AlpacaWriteJson(JsonObject &root);

  const bool _putTempComp(bool temp_comp) { return true; };
  const bool _putHalt();
  const bool _putMove(int32_t target_position_steps);

  // optional Alpaca service: to be implemented if needed
  const bool _putAction(const char *const action, const char *const parameters, char *string_response, size_t string_response_size) { return false; }
  const bool _putCommandBlind(const char *const command, const char *const raw, bool &bool_response) { return false; };
  const bool _putCommandBool(const char *const command, const char *const raw, bool &bool_response) { return false; };
  const bool _putCommandString(const char *const command_str, const char *const raw, char *string_response, size_t string_response_size) { return false; };

  const bool _getAbsolut() { return _absolut; };
  const bool _getIsMoving() { return _is_moving; };
  const int32_t _getMaxIncrement() { return _max_increment; };
  const int32_t _getMaxStep() { return _max_motor_step; };
  const int32_t _getPosition() { return _position_steps; };
  const double _getStepSize() { return _step_size; };
  const bool _getTempComp() { return _temp_comp; };
  const bool _getTempCompAvailable() { return _temp_comp_available; };
  const double _getTemperature() { return _temperature; };

  void _simMachine();
  const char *const _simState2Str(FocuserState_t state) { return _k_sim_state_str[(int)state]; };

public:
  Focuser();
  void Begin();
  void Loop();
};
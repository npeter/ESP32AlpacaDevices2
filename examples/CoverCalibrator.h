/**************************************************************************************************
  Description:    ASCOM CoverCalibrator Device Teplate with Simulation
  Copyright 2024-2025 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#pragma once
#include "AlpacaCoverCalibrator.h"

enum struct DeviceState_t
{
  kInit = 0,
  kClosed,
  kOpens,
  kOpen,
  kCloses,
  kStopped,
  kInvalid
};

class CoverCalibrator : public AlpacaCoverCalibrator
{
private:
  const bool _calibratorOff();
  const bool _calibratorOn(int32_t brightness);
  const bool _closeCover();
  const bool _openCover();
  const bool _haltCover();

  // optional Alpaca service: to be implemented if needed
  const bool _putAction(const char *const action, const char *const parameters, char *string_response, size_t string_response_size) { return false; }
  const bool _putCommandBlind(const char *const command, const char *const raw, bool &bool_response) { return false; };
  const bool _putCommandBool(const char *const command, const char *const raw, bool &bool_response) { return false; };
  const bool _putCommandString(const char *const command_str, const char *const raw, char *string_response, size_t string_response_size) { return false; };

  void AlpacaReadJson(JsonObject &root);
  void AlpacaWriteJson(JsonObject &root);

  // only for simulation needed
  DeviceState_t _state = DeviceState_t::kInit;
  DeviceState_t _old_state = DeviceState_t::kInit;
  uint32_t _startTimeMs = 0;
  bool _cover_close_event = false;
  bool _cover_open_event = false;
  bool _cover_halt_event = false;

  void _calibratorDeviceLoop();
  void _coverDeviceLoop();

  static const char *const k_device_state_str[7];
  const char *const getDeviceStateStr(DeviceState_t state);

public:
  CoverCalibrator();
  void Begin();
  void Loop();
};
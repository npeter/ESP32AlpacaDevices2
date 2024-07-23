/**************************************************************************************************
  Filename:       CoverCalibrator.h
  Revised:        $Date: 2024-01-14$
  Revision:       $Revision: 01 $
  Description:    ASCOM CoverCalibrator Device Teplate with Simulation

  Copyright 2024 peter_n@gmx.de. All rights reserved.
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

#ifdef ALPACA_COVER_CALIBRATOR_PUT_ACTION_IMPLEMENTED
  const bool _putAction(const char *const action, const char *const parameters);
#endif
#ifdef ALPACA_COVER_CALIBRATOR_PUT_COMMAND_BLIND_IMPLEMENTED
  const bool _putCommandBlind(const char *const command, const char *const raw);
#endif
#ifdef ALPACA_COVER_CALIBRATOR_PUT_COMMAND_BOOL_IMPLEMENTED
  const bool _putCommandBool(const char *const command, const char *const raw, bool &bool_response);
#endif
#ifdef ALPACA_COVER_CALIBRATOR_PUT_COMMAND_STRING_IMPLEMENTED
  const bool _putCommandString(const char *const command, const char *const raw, char *string_response, size_t string_response_size);
#endif

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
/**************************************************************************************************
  Filename:       AlpacaCoverCalibrator.h
  Revised:        $Date: 2024-01-14$
  Revision:       $Revision: 01 $
  Description:    Common ASCOM Alpaca CoverCalibrator V1

  Copyright 2024-2025 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#pragma once
#include "AlpacaDevice.h"

// ASCOM  / ALPACA CalobratorStatus Enumeration
enum struct AlpacaCalibratorStatus_t
{
  kNotPresent = 0,
  kOff,
  kNotReady,
  kReady,
  kUnknown,
  kError,
  kInvalid
};

// ASCOM / ALPACA CoverStatus Enumeration
enum struct AlpacaCoverStatus_t
{
  kNotPresent = 0,
  kClosed,
  kMoving,
  kOpen,
  kUnknown,
  kError,
  kInvalid
};

class AlpacaCoverCalibrator : public AlpacaDevice
{
private:
  // CalibratorDevice
  AlpacaCalibratorStatus_t _calibrator_state = AlpacaCalibratorStatus_t::kNotPresent;
  static const char *const kAlpacaCalibratorStatusStr[7];
  int32_t _brightness = 0; // 0,...,_maxBrightness; 0 - off
  int32_t _max_brightness = 0;

  // CoverDevice
  AlpacaCoverStatus_t _cover_state = AlpacaCoverStatus_t::kNotPresent;
  static const char *const k_alpaca_cover_status_str[7];

  virtual const char *const _getFirmwareVersion() { return "-"; };
  void _alpacaGetBrightness(AsyncWebServerRequest *request);
  void _alpacaGetCalibratorState(AsyncWebServerRequest *request);
  void _alpacaGetCoverState(AsyncWebServerRequest *request);
  void _alpacaGetMaxBrightness(AsyncWebServerRequest *request);

  void _alpacaPutCalibratorOff(AsyncWebServerRequest *request);
  void _alpacaPutCalibratorOn(AsyncWebServerRequest *request);
  void _alpacaPutCloseCover(AsyncWebServerRequest *request);
  void _alpacaPutHaltCover(AsyncWebServerRequest *request);
  void _alpacaPutOpenCover(AsyncWebServerRequest *request);

  void AlpacaPutAction(AsyncWebServerRequest *request);
  void AlpacaPutCommandBlind(AsyncWebServerRequest *request);
  void AlpacaPutCommandBool(AsyncWebServerRequest *request);
  void AlpacaPutCommandString(AsyncWebServerRequest *request);

  virtual const bool _putAction(const char *const action, const char *const parameters, char *string_response, size_t string_response_size) = 0;
  virtual const bool _putCommandBlind(const char *const command, const char *const raw, bool &bool_response) = 0;
  virtual const bool _putCommandBool(const char *const command, const char *const raw, bool &bool_response) = 0;
  virtual const bool _putCommandString(const char *const command_str, const char *const raw, char *string_response, size_t string_response_size) = 0;

  virtual const bool _calibratorOff() = 0;
  virtual const bool _calibratorOn(int32_t brightness) = 0;

  virtual const bool _closeCover() = 0;
  virtual const bool _openCover() = 0;
  virtual const bool _haltCover() = 0;

protected:
  AlpacaCoverCalibrator();
  void Begin();
  void RegisterCallbacks();

  const int32_t GetBrightness() { return _brightness; };
  const int32_t GetMaxBrightness() { return _max_brightness; };
  const AlpacaCalibratorStatus_t GetCalibratorState() { return _calibrator_state; };
  const AlpacaCoverStatus_t GetCoverState() { return _cover_state; };

  void SetCoverState(AlpacaCoverStatus_t cover_state) { _cover_state = cover_state; };
  void SetCalibratorState(AlpacaCalibratorStatus_t calibrator_state) { _calibrator_state = calibrator_state; };
  void SetBrightness(int32_t brightness) { _brightness = brightness; };
  void SetMaxBrightness(int32_t max_brightness) { _max_brightness = max_brightness; };

  const char *GetAlpacaCalibratorStatusStr(AlpacaCalibratorStatus_t state) { return kAlpacaCalibratorStatusStr[(uint32_t)state]; };
  const char *const GetAlpacaCoverStatusStr(AlpacaCoverStatus_t state) { return k_alpaca_cover_status_str[(uint32_t)state]; };

public:
};

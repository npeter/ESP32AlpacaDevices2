/**************************************************************************************************
  Filename:       AlpacaObservingConditions.h
  Revised:        $Date: 2024-02-02$
  Revision:       $Revision: 01 $
  Description:    Common ASCOM Alpaca ObservingConditions Interface V1

  Copyright 2024-2025 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#pragma once
#include "AlpacaDevice.h"

const uint32_t kMaxSensorName = 32;
const uint32_t kMaxSensorDescription = 128;

struct OCSensor_t                          // Sensor description
{                                          //
  char sensor_name[kMaxSensorName];        // sensor name as defined by Alpaca
  char description[kMaxSensorDescription]; // sensor description from user
  double value;                            // latest sensor value
  uint32_t update_time_ms;                 // latest sensor update time using system time/ [ms]
  bool is_implemented;                     //
};

enum OCSensorIdx_t // Sensor indices (0,...,max-1) - don't change
{
  kOcCloudCoverSensorIdx = 0,
  kOcDewPointSensorIdx,
  kOcHumiditySensorIdx,
  kOcPressureSensorIdx,
  kOcRainRateSensorIdx,
  kOcSkyBrightnessSensorIdx,
  kOcSkyQualitySensorIdx,
  kOcSkyTemperatureSensorIdx,
  kOcStarFwhmSensorIdx,
  kOcTemperatureSensorIdx,
  kOcWindDirectionSensorIdx,
  kOcWindGuestSensorIdx,
  kOcWindSpeedSensorIdx,
  kOcMaxSensorIdx
};

class AlpacaObservingConditions : public AlpacaDevice
{
private:
  OCSensor_t _sensors[kOcMaxSensorIdx];
  double _average_period = 0.0;

  void _alpacaGetAveragePeriod(AsyncWebServerRequest *request);
  void _alpacaGetCloudCover(AsyncWebServerRequest *request);
  void _alpacaGetDewPoint(AsyncWebServerRequest *request);
  void _alpacaGetHumidity(AsyncWebServerRequest *request);
  void _alpacaGetPressure(AsyncWebServerRequest *request);
  void _alpacaGetRainRate(AsyncWebServerRequest *request);
  void _alpacaGetSkyBrightness(AsyncWebServerRequest *request);
  void _alpacaGetSkyQuality(AsyncWebServerRequest *request);
  void _alpacaGetSkyTemperature(AsyncWebServerRequest *request);
  void _alpacaGetStarFwhm(AsyncWebServerRequest *request);
  void _alpacaGetTemperature(AsyncWebServerRequest *request);
  void _alpacaGetWindDirection(AsyncWebServerRequest *request);
  void _alpacaGetWindGust(AsyncWebServerRequest *request);
  void _alpacaGetWindSpeed(AsyncWebServerRequest *request);
  void _alpacaGetSensordescription(AsyncWebServerRequest *request);
  void _alpacaGetTimeSinceLastUpdate(AsyncWebServerRequest *request);

  void _alpacaPutAveragePeriod(AsyncWebServerRequest *request);
  void _alpacaPutRefresh(AsyncWebServerRequest *request);

  void AlpacaPutAction(AsyncWebServerRequest *request);
  void AlpacaPutCommandBlind(AsyncWebServerRequest *request);
  void AlpacaPutCommandBool(AsyncWebServerRequest *request);
  void AlpacaPutCommandString(AsyncWebServerRequest *request);

  virtual const bool _putAction(const char *const action, const char *const parameters, char *string_response, size_t string_response_size) = 0;
  virtual const bool _putCommandBlind(const char *const command, const char *const raw, bool &bool_response) = 0;
  virtual const bool _putCommandBool(const char *const command, const char *const raw, bool &bool_response) = 0;
  virtual const bool _putCommandString(const char *const command_str, const char *const raw, char *string_response, size_t string_response_size) = 0;

  const bool getDeviceStates(size_t buf_len, char* buf);

  // private helpers
  AlpacaRspStatus_t &_rspStatusSensorNotImplemented(AsyncWebServerRequest *request, AlpacaRspStatus_t &rsp_status, const char *sensor_name);
  const bool _getSensorIdxByName(const char *sensor_name, OCSensorIdx_t &sensor_idx);

  // virtual instanc specific methods
  virtual const char *const _getFirmwareVersion() { return "-"; };
  virtual void _putRefreshRequest() = 0;
  virtual const bool _putAveragePeriodRequest(double average_period) = 0;

protected:
  // Interface for specific implementation
  AlpacaObservingConditions();
  void Begin();
  void RegisterCallbacks();
  // void Loop() { ; } // empty

  const bool SetSensorValueByIdx(OCSensorIdx_t idx, double value, uint32_t update_time_ms);
  const bool SetSensorDescriptionByIdx(OCSensorIdx_t idx, const char *description);
  const bool SetSensorImplementedByIdx(OCSensorIdx_t idx, bool is_implemented);
  void SetAveragePeriod(bool average_period) { _average_period = average_period; };

  const double GetSensorValueByIdx(OCSensorIdx_t idx) { return _sensors[idx < kOcMaxSensorIdx ? idx : kOcCloudCoverSensorIdx].value; };
  const bool GetSensorImplementedByIdx(OCSensorIdx_t idx) { return _sensors[idx < kOcMaxSensorIdx ? idx : kOcCloudCoverSensorIdx].is_implemented; };
  const char *GetSensorNameByIdx(OCSensorIdx_t idx) { return _sensors[idx < kOcMaxSensorIdx ? idx : kOcCloudCoverSensorIdx].sensor_name; };
  const char *GetSensorDescriptionByIdx(OCSensorIdx_t idx) { return _sensors[idx < kOcMaxSensorIdx ? idx : kOcCloudCoverSensorIdx].description; };

public:
};

/**************************************************************************************************
  Filename:       AlpacaObservingConditions.cpp
  Revised:        $Date: 2024-02-02$
  Revision:       $Revision: 01 $
  Description:    Common ASCOM Alpaca ObservingConditions Interface V1

  Copyright 2024-2025 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#include "AlpacaObservingConditions.h"

AlpacaObservingConditions::AlpacaObservingConditions()
{
    strlcpy(_device_type, ALPACA_OBSERVING_CONDITIONS_DEVICE_TYPE, sizeof(_device_type));
    strlcpy(_device_description, ALPACA_OBSERVING_CONDITIONS_DESCRIPTION, sizeof(_device_description));
    strlcpy(_driver_info, ALPACA_OBSERVING_CONDITIONS_DRIVER_INFO, sizeof(_driver_info));
    strlcpy(_device_and_driver_version, esp32_alpaca_device_library_version, sizeof(_device_and_driver_version));
    _device_interface_version = ALPACA_OBSERVING_CONDITIONS_INTERFACE_VERSION;

    for (int i = 0; i < (int)OCSensorIdx_t::kOcMaxSensorIdx; i++)
    {
        _sensors[i].value = 0.0;
        _sensors[i].update_time_ms = 0.0;
        _sensors[i].is_implemented = false;
    }

    // Don't change the sensor names
    strlcpy(_sensors[kOcCloudCoverSensorIdx].sensor_name, "CloudCover", kMaxSensorName);
    strlcpy(_sensors[kOcDewPointSensorIdx].sensor_name, "DewPoint", kMaxSensorName);
    strlcpy(_sensors[kOcHumiditySensorIdx].sensor_name, "Humidity", kMaxSensorName);
    strlcpy(_sensors[kOcPressureSensorIdx].sensor_name, "Pressure", kMaxSensorName);
    strlcpy(_sensors[kOcRainRateSensorIdx].sensor_name, "RainRate", kMaxSensorName);
    strlcpy(_sensors[kOcSkyBrightnessSensorIdx].sensor_name, "SkyBrightness", kMaxSensorName);
    strlcpy(_sensors[kOcSkyQualitySensorIdx].sensor_name, "SkyQuality", kMaxSensorName);
    strlcpy(_sensors[kOcSkyTemperatureSensorIdx].sensor_name, "SkyTemperature", kMaxSensorName);
    strlcpy(_sensors[kOcStarFwhmSensorIdx].sensor_name, "StarFWHM", kMaxSensorName);
    strlcpy(_sensors[kOcTemperatureSensorIdx].sensor_name, "Temperature", kMaxSensorName);
    strlcpy(_sensors[kOcWindDirectionSensorIdx].sensor_name, "WindDirection", kMaxSensorName);
    strlcpy(_sensors[kOcWindGustSensorIdx].sensor_name, "WindGust", kMaxSensorName);
    strlcpy(_sensors[kOcWindSpeedSensorIdx].sensor_name, "WindSpeed", kMaxSensorName);

    // adapt the sensor description
    strlcpy(_sensors[kOcCloudCoverSensorIdx].description, "CloudCover Description", kMaxSensorDescription);
    strlcpy(_sensors[kOcDewPointSensorIdx].description, "DewPoint Description", kMaxSensorDescription);
    strlcpy(_sensors[kOcHumiditySensorIdx].description, "Humidity Description", kMaxSensorDescription);
    strlcpy(_sensors[kOcPressureSensorIdx].description, "Pressure Description", kMaxSensorDescription);
    strlcpy(_sensors[kOcRainRateSensorIdx].description, "RainRate Description", kMaxSensorDescription);
    strlcpy(_sensors[kOcSkyBrightnessSensorIdx].description, "SkyBrightness Description", kMaxSensorDescription);
    strlcpy(_sensors[kOcSkyQualitySensorIdx].description, "SkyQuality  Description", kMaxSensorDescription);
    strlcpy(_sensors[kOcSkyTemperatureSensorIdx].description, "SkyTemperature  Description", kMaxSensorDescription);
    strlcpy(_sensors[kOcStarFwhmSensorIdx].description, "StarFwhm  Description", kMaxSensorDescription);
    strlcpy(_sensors[kOcTemperatureSensorIdx].description, "Temperature  Description", kMaxSensorDescription);
    strlcpy(_sensors[kOcWindDirectionSensorIdx].description, "WindDirection  Description", kMaxSensorDescription);
    strlcpy(_sensors[kOcWindGustSensorIdx].description, "WindGust  Description", kMaxSensorDescription);
    strlcpy(_sensors[kOcWindSpeedSensorIdx].description, "windSpeed  Description", kMaxSensorDescription);
}

void AlpacaObservingConditions::Begin()
{
    snprintf(_device_and_driver_version, sizeof(_device_and_driver_version), "%s/%s", _getFirmwareVersion(), esp32_alpaca_device_library_version);
    AlpacaDevice::Begin();
}

void AlpacaObservingConditions::RegisterCallbacks()
{
    AlpacaDevice::RegisterCallbacks();

    this->createCallBack(LHF(AlpacaPutAction), HTTP_PUT, "action");
    this->createCallBack(LHF(AlpacaPutCommandBlind), HTTP_PUT, "commandblind");
    this->createCallBack(LHF(AlpacaPutCommandBool), HTTP_PUT, "commandbool");
    this->createCallBack(LHF(AlpacaPutCommandString), HTTP_PUT, "commandstring");

    this->createCallBack(LHF(_alpacaGetAveragePeriod), HTTP_GET, "averageperiod");
    this->createCallBack(LHF(_alpacaGetCloudCover), HTTP_GET, "cloudcover");
    this->createCallBack(LHF(_alpacaGetDewPoint), HTTP_GET, "dewpoint");
    this->createCallBack(LHF(_alpacaGetHumidity), HTTP_GET, "humidity");
    this->createCallBack(LHF(_alpacaGetPressure), HTTP_GET, "pressure");
    this->createCallBack(LHF(_alpacaGetRainRate), HTTP_GET, "rainrate");
    this->createCallBack(LHF(_alpacaGetSkyBrightness), HTTP_GET, "skybrightness");
    this->createCallBack(LHF(_alpacaGetSkyQuality), HTTP_GET, "skyquality");
    this->createCallBack(LHF(_alpacaGetSkyTemperature), HTTP_GET, "skytemperature");
    this->createCallBack(LHF(_alpacaGetStarFwhm), HTTP_GET, "starfwhm");
    this->createCallBack(LHF(_alpacaGetTemperature), HTTP_GET, "temperature");
    this->createCallBack(LHF(_alpacaGetWindDirection), HTTP_GET, "winddirection");
    this->createCallBack(LHF(_alpacaGetWindGust), HTTP_GET, "windgust");
    this->createCallBack(LHF(_alpacaGetWindSpeed), HTTP_GET, "windspeed");
    this->createCallBack(LHF(_alpacaGetSensordescription), HTTP_GET, "sensordescription");
    this->createCallBack(LHF(_alpacaGetTimeSinceLastUpdate), HTTP_GET, "timesincelastupdate");
    this->createCallBack(LHF(_alpacaPutAveragePeriod), HTTP_PUT, "averageperiod");
    this->createCallBack(LHF(_alpacaPutRefresh), HTTP_PUT, "refresh");
};

void AlpacaObservingConditions::AlpacaPutAction(AsyncWebServerRequest *request)
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

void AlpacaObservingConditions::AlpacaPutCommandBlind(AsyncWebServerRequest *request)
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

void AlpacaObservingConditions::AlpacaPutCommandBool(AsyncWebServerRequest *request)
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

void AlpacaObservingConditions::AlpacaPutCommandString(AsyncWebServerRequest *request)
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

void AlpacaObservingConditions::_alpacaGetAveragePeriod(AsyncWebServerRequest *request)
{
    DBG_OBSERVING_CONDITIONS_GET_AVERAGE_PERIOD
    _service_counter++;
    uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);
    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, _average_period);
    DBG_END
}

#define METHODE(_M_, _DBGNAME_, _IDX_)                                                                                                                 \
    void AlpacaObservingConditions::_M_(AsyncWebServerRequest *request)                                                                                \
    {                                                                                                                                                  \
        _DBGNAME_;                                                                                                                                     \
        _service_counter++;                                                                                                                            \
        uint32_t client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase);                                              \
        if (_sensors[_IDX_].is_implemented)                                                                                                            \
            _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, _sensors[_IDX_].value);                                                \
        else if (_rsp_status.error_code == AlpacaErrorCode_t::Ok)                                                                                      \
            _alpaca_server->Respond(request, _clients[client_idx], _rspStatusSensorNotImplemented(request, _rsp_status, _sensors[_IDX_].sensor_name)); \
        else                                                                                                                                           \
            _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);                                                                       \
        DBG_END                                                                                                                                        \
    }

METHODE(_alpacaGetCloudCover, DBG_OBSERVING_CONDITIONS_GET_CLOUD_COVER, kOcCloudCoverSensorIdx)
METHODE(_alpacaGetDewPoint, DBG_OBSERVING_CONDITIONS_GET_DEW_POINT, kOcDewPointSensorIdx)
METHODE(_alpacaGetHumidity, DBG_OBSERVING_CONDITIONS_GET_HUMIDITY, kOcHumiditySensorIdx)
METHODE(_alpacaGetPressure, DBG_OBSERVING_CONDITIONS_GET_PRESSURE, kOcPressureSensorIdx)
METHODE(_alpacaGetRainRate, DBG_OBSERVING_CONDITIONS_GET_RAIN_RATE, kOcRainRateSensorIdx)
METHODE(_alpacaGetSkyBrightness, DBG_OBSERVING_CONDITIONS_GET_SKY_BRIGHTNESS, kOcSkyBrightnessSensorIdx)
METHODE(_alpacaGetSkyQuality, DBG_OBSERVING_CONDITIONS_GET_SKY_QUALITY, kOcSkyQualitySensorIdx)
METHODE(_alpacaGetSkyTemperature, DBG_OBSERVING_CONDITIONS_GET_SKY_TEMPERATURE, kOcSkyTemperatureSensorIdx)
METHODE(_alpacaGetStarFwhm, DBG_OBSERVING_CONDITIONS_GET_STAR_FWHM, kOcStarFwhmSensorIdx)
METHODE(_alpacaGetTemperature, DBG_OBSERVING_CONDITIONS_GET_TEMPERATURE, kOcTemperatureSensorIdx)
METHODE(_alpacaGetWindDirection, DBG_OBSERVING_CONDITIONS_GET_WIND_DIRECTION, kOcWindDirectionSensorIdx)
METHODE(_alpacaGetWindGust, DBG_OBSERVING_CONDITIONS_GET_WIND_GUST, kOcWindGustSensorIdx)
METHODE(_alpacaGetWindSpeed, DBG_OBSERVING_CONDITIONS_GET_WIND_SPEED, kOcWindSpeedSensorIdx)
#undef METHODE

void AlpacaObservingConditions::_alpacaGetSensordescription(AsyncWebServerRequest *request)
{
    DBG_OBSERVING_CONDITIONS_GET_SENSOR_DESCRIPTION
    _service_counter++;
    char description[kMaxSensorDescription] = {0};
    char sensor_name[kMaxSensorName] = "";
    uint32_t client_idx = 0;
    OCSensorIdx_t sensor_idx;

    if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase)) == 0)
        goto mycatch;

    if (_alpaca_server->GetParam(request, "SensorName", sensor_name, sizeof(sensor_name), Spelling_t::kIgnoreCase) == false)
        MYTHROW_RspStatusParameterNotFound(request, _rsp_status, "SensorName");

    if (_getSensorIdxByName(sensor_name, sensor_idx) == false)
    {
        _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
        _rsp_status.http_status = HttpStatus_t::kPassed;
        snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - Sensor '%s' invalid", request->url().c_str(), sensor_name);
        goto mycatch;
    }

    snprintf(description, sizeof(description), "%s", _sensors[sensor_idx].description);

mycatch: // empty

    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, description);
    DBG_END
}

void AlpacaObservingConditions::_alpacaGetTimeSinceLastUpdate(AsyncWebServerRequest *request)
{
    DBG_OBSERVING_CONDITIONS_GET_TIME_SINCE_LAST_UPDATE
    _service_counter++;
    double update_time_rel_ms = 0.0;
    char sensor_name[kMaxSensorName] = "";
    uint32_t client_idx = 0;
    OCSensorIdx_t sensor_idx;

    if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kIgnoreCase)) == 0)
        goto mycatch;

    if (_alpaca_server->GetParam(request, "SensorName", sensor_name, sizeof(sensor_name), Spelling_t::kIgnoreCase) == false)
        MYTHROW_RspStatusParameterNotFound(request, _rsp_status, "SensorName");

    if (_getSensorIdxByName(sensor_name, sensor_idx) == false)
    {
        _rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
        _rsp_status.http_status = HttpStatus_t::kPassed;
        snprintf(_rsp_status.error_msg, sizeof(_rsp_status.error_msg), "%s - Sensor '%s' invalid", request->url().c_str(), sensor_name);
        goto mycatch;
    }
    update_time_rel_ms = (double)(millis() - _sensors[sensor_idx].update_time_ms);

mycatch: // empty

    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status, update_time_rel_ms);
    DBG_END
}

bool const AlpacaObservingConditions::_getDeviceStateList(size_t buf_len, char *buf)
{
    size_t snprintf_result = 0;
    size_t len = 0;
    for (int i = 0; i < static_cast<int>(OCSensorIdx_t::kOcMaxSensorIdx); i++)
    {
        OCSensorIdx_t idx = static_cast<OCSensorIdx_t>(i);
        if (GetSensorImplementedByIdx(idx))
        {
            len = strlen(buf);
            if (len < buf_len - 64) // <{"Name":"x","Value":"GetSwitch0},>+'\0'
            {
                snprintf_result =
                    snprintf(buf + len, buf_len - len - 1, "{\"Name\":\"%s\",\"Value\":%f},",
                             GetSensorNameByIdx(idx), GetSensorValueByIdx(idx));
            }
            else
            {
                snprintf_result = 0;
                break;
            }
        }
    }
    len = strlen(buf);
    *(buf + len - 1) = '\0'; // replace ',' by '\0'

    return (snprintf_result > 0 && snprintf_result <= buf_len);
}

void AlpacaObservingConditions::_alpacaPutAveragePeriod(AsyncWebServerRequest *request)
{
    DBG_OBSERVING_CONDITIONS_GET_PUT_AVERAGE_PERIOD
    _service_counter++;
    uint32_t client_idx = 0;
    double average_period = 0.0;
    _alpaca_server->RspStatusClear(_rsp_status);

    if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) == 0)
        goto mycatch;

    if (_alpaca_server->GetParam(request, "AveragePeriod", average_period, Spelling_t::kStrict) == false)
        MYTHROW_RspStatusParameterNotFound(request, _rsp_status, "AvaragePeriod");

    if (_putAveragePeriodRequest(average_period) == false)
        MYTHROW_RspStatusParameterInvalidDoubleValue(request, _rsp_status, "AvaragePeriod", average_period);

mycatch: // empty

    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
}

void AlpacaObservingConditions::_alpacaPutRefresh(AsyncWebServerRequest *request)
{
    DBG_OBSERVING_CONDITIONS_PUT_REFRESH
    _service_counter++;
    uint32_t client_idx = 0;
    _alpaca_server->RspStatusClear(_rsp_status);

    if ((client_idx = checkClientDataAndConnection(request, client_idx, Spelling_t::kStrict)) > 0)
        _putRefreshRequest();

    _alpaca_server->Respond(request, _clients[client_idx], _rsp_status);
    DBG_END
}

AlpacaRspStatus_t &AlpacaObservingConditions::_rspStatusSensorNotImplemented(AsyncWebServerRequest *request, AlpacaRspStatus_t &rsp_status, const char *sensor_name)
{
    rsp_status.error_code = AlpacaErrorCode_t::NotImplemented;
    rsp_status.http_status = HttpStatus_t::kPassed;
    snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - Sensor '%s' not implemented", request->url().c_str(), sensor_name);
    return rsp_status;
}

const bool AlpacaObservingConditions::_getSensorIdxByName(const char *sensor_name, OCSensorIdx_t &sensor_idx)
{
    for (uint32_t i = 0; i < (int)OCSensorIdx_t::kOcMaxSensorIdx; i++)
    {
        if (strcmp(sensor_name, _sensors[i].sensor_name) == 0)
        {
            sensor_idx = (OCSensorIdx_t)i;
            return true;
        }
    }
    return false;
}

const bool AlpacaObservingConditions::SetSensorValueByIdx(OCSensorIdx_t idx, double value, uint32_t update_time_ms)
{
    if (idx < kOcMaxSensorIdx)
    {
        _sensors[idx].value = value;
        _sensors[idx].update_time_ms = update_time_ms;
        return true;
    }
    return false;
}

const bool AlpacaObservingConditions::SetSensorDescriptionByIdx(OCSensorIdx_t idx, const char *description)
{
    if (idx < kOcMaxSensorIdx)
    {
        strlcpy(_sensors[idx].description, description, kMaxSensorDescription);
        return true;
    }
    return false;
}

const bool AlpacaObservingConditions::SetSensorImplementedByIdx(OCSensorIdx_t idx, bool is_implemented)
{
    if (idx < kOcMaxSensorIdx)
    {
        _sensors[idx].is_implemented = is_implemented;
        return true;
    }
    return false;
}

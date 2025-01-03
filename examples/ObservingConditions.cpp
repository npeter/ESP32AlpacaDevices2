/**************************************************************************************************
  Filename:       ObservingConditions.cpp
  Revised:        $Date: 2024-02-02$
  Revision:       $Revision: 01 $
  Description:    ASCOM Observing Conditions Device Template

  Copyright 2024-2025 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#include "ObservingConditions.h"

ObservingConditions::ObservingConditions() : AlpacaObservingConditions()
{
}

void ObservingConditions::Begin()
{
    // adapt the sensor description
    SetSensorDescriptionByIdx(kOcCloudCoverSensorIdx, "CloudCover Description");
    SetSensorDescriptionByIdx(kOcDewPointSensorIdx, "DewPoint Description");
    SetSensorDescriptionByIdx(kOcHumiditySensorIdx, "Humidity Description");
    SetSensorDescriptionByIdx(kOcPressureSensorIdx, "Pressure Description");
    SetSensorDescriptionByIdx(kOcRainRateSensorIdx, "Rain Description");
    SetSensorDescriptionByIdx(kOcSkyBrightnessSensorIdx, "SkyBrightness Description");
    SetSensorDescriptionByIdx(kOcSkyQualitySensorIdx, "SkyQuality  Description");
    SetSensorDescriptionByIdx(kOcSkyTemperatureSensorIdx, "SkyTemperature  Description");
    SetSensorDescriptionByIdx(kOcStarFwhmSensorIdx, "StarFwhm  Description");
    SetSensorDescriptionByIdx(kOcTemperatureSensorIdx, "Temperature  Description");
    SetSensorDescriptionByIdx(kOcWindDirectionSensorIdx, "WindDirection  Description");
    SetSensorDescriptionByIdx(kOcWindGuestSensorIdx, "WindGuest  Description");

    // adapt if not implemented
    SetSensorImplementedByIdx(kOcCloudCoverSensorIdx, true);
    SetSensorImplementedByIdx(kOcDewPointSensorIdx, true);
    SetSensorImplementedByIdx(kOcHumiditySensorIdx, true);
    SetSensorImplementedByIdx(kOcPressureSensorIdx, true);
    SetSensorImplementedByIdx(kOcRainRateSensorIdx, true);
    SetSensorImplementedByIdx(kOcSkyBrightnessSensorIdx, true);
    SetSensorImplementedByIdx(kOcSkyQualitySensorIdx, true);
    SetSensorImplementedByIdx(kOcSkyTemperatureSensorIdx, true);
    SetSensorImplementedByIdx(kOcStarFwhmSensorIdx, true);
    SetSensorImplementedByIdx(kOcTemperatureSensorIdx, true);
    SetSensorImplementedByIdx(kOcWindDirectionSensorIdx, true);
    SetSensorImplementedByIdx(kOcWindGuestSensorIdx, true);
    SetSensorImplementedByIdx(kOcWindSpeedSensorIdx, true);

    // init sensor and sensor data
    uint32_t system_time = millis();
    SetSensorValueByIdx(kOcCloudCoverSensorIdx, 1.0, system_time);
    SetSensorValueByIdx(kOcDewPointSensorIdx, 2.0, system_time);
    SetSensorValueByIdx(kOcHumiditySensorIdx, 3.0, system_time);
    SetSensorValueByIdx(kOcPressureSensorIdx, 3.0, system_time);
    SetSensorValueByIdx(kOcRainRateSensorIdx, 5.0, system_time);
    SetSensorValueByIdx(kOcSkyBrightnessSensorIdx, 6.0, system_time);
    SetSensorValueByIdx(kOcSkyQualitySensorIdx, 7.0, system_time);
    SetSensorValueByIdx(kOcSkyTemperatureSensorIdx, 8.0, system_time);
    SetSensorValueByIdx(kOcStarFwhmSensorIdx, 9.0, system_time);
    SetSensorValueByIdx(kOcTemperatureSensorIdx, 10.0, system_time);
    SetSensorValueByIdx(kOcWindDirectionSensorIdx, 1.0, system_time);
    SetSensorValueByIdx(kOcWindGuestSensorIdx, 2.0, system_time);
    SetSensorValueByIdx(kOcWindSpeedSensorIdx, 3.0, system_time);

    SetAveragePeriod(0.0);
    _update_time_ms = system_time;

    AlpacaObservingConditions::Begin();
}

void ObservingConditions::Loop()
{
    if ((_update_time_ms + _refresh_time_ms) <= millis())
    {
        _refresh();
    }
    //AlpacaObservingConditions::Loop();
}

const bool ObservingConditions::_putAveragePeriodRequest(double average_period)
{
    // perform your specific test
    // https://ascom-standards.org/Help/Developer/html/P_ASCOM_DeviceInterface_IObservingConditions_AveragePeriod.htm
    if (average_period == 0.0) //
    {
        SetAveragePeriod(average_period);
        return true;
    }
    else
    {
        return false;
    }
}

void ObservingConditions::_refresh()
{
    _update_time_ms = millis();
    // refresh sensordata - just for testing
    for (uint32_t i = 0; i < OCSensorIdx_t::kOcMaxSensorIdx; i++)
    {
        SetSensorValueByIdx((OCSensorIdx_t)i, GetSensorValueByIdx((OCSensorIdx_t)i) + 0.01, _update_time_ms);
    }
}

void ObservingConditions::AlpacaReadJson(JsonObject &root)
{
    DBG_JSON_PRINTFJ(SLOG_INFO, root, "BEGIN (root=<%s>) ...\n", _ser_json_);
    AlpacaObservingConditions::AlpacaReadJson(root);

    if (JsonObject obj_config = root["Configuration"])
    {
        int xyz_dummy = 0;
        xyz_dummy = obj_config["xyz"] | xyz_dummy;
        SLOG_PRINTF(SLOG_INFO, "... \"xyz_dummy=%d\"\n", xyz_dummy);
    }
    else
    {
        SLOG_PRINTF(SLOG_WARNING, "... END    ... no Configuration\n");
    }
}

// to be adapted
void ObservingConditions::AlpacaWriteJson(JsonObject &root)
{
    SLOG_PRINTF(SLOG_INFO, "BEGIN ...\n");
    AlpacaObservingConditions::AlpacaWriteJson(root);

    // #add # for read only
    JsonObject obj_states = root["#States"].to<JsonObject>();

    for (int i = kOcCloudCoverSensorIdx; i < kOcMaxSensorIdx; i++)
    {
        if (GetSensorImplementedByIdx((OCSensorIdx_t)i))
            obj_states[GetSensorNameByIdx((OCSensorIdx_t)i)] = GetSensorValueByIdx((OCSensorIdx_t)i);
    }

    DBG_JSON_PRINTFJ(SLOG_INFO, obj_states, "... END \n", _ser_json_);
}
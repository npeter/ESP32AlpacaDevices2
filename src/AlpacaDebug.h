/**************************************************************************************************
  Filename:       AlpacaDebug.h
  Revised:        $Date: 2024-07-01$
  Revision:       $Revision: 02 $

  Description:    Debugging for ESP32 Alpaca server

  Copyright 2024 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#pragma once
#include <Arduino.h>
#include <SLog.h>

// For declaration include "#define VAR_DECLS" in (only) one *.cpp (see AlpacaServer.cpp)
#ifndef ALPACA_VAR_DECLS
#define _ALPACA_DECL_ extern
#define _ALPACA_INIT_(x)
#else
#define _ALPACA_DECL_
#define _ALPACA_INIT_(x) = x
#endif

// definition and declaration of global variables; don't touch
_ALPACA_DECL_ bool gDbg _ALPACA_INIT_(false);

// JSON debugging ...
// comment/uncomment to disable/enable debugging
// #define DBG_JSON_PRINTFJ(...)

#ifndef DBG_JSON_PRINTFJ
#define DBG_JSON_PRINTFJ(lvl, json, ...) \
    {                                    \
        char _ser_json_[1024] = {0};     \
        serializeJson(json, _ser_json_); \
        SLOG_PRINTF(lvl, __VA_ARGS__);   \
    };
#endif

// Debug Alpaca Requests
// comment DBG_REQ to disable
#define DBG_SERVER_GET_MNG_API_VERSION DBG_REQ;
#define DBG_SERVER_GET_MNG_DESCRIPTION DBG_REQ;
#define DBG_SERVER_GET_MNG_CONFIGUREDDEVICES DBG_REQ;

#define DBG_DEVICE_GET_CONNECTED DBG_REQ;
#define DBG_DEVICE_GET_DESCRIPTION DBG_REQ;
#define DBG_DEVICE_GET_DRIVER_INFO DBG_REQ;
#define DBG_DEVICE_GET_DRIVER_VERSION DBG_REQ;
#define DBG_DEVICE_GET_INTERFACE_VERSION DBG_REQ;
#define DBG_DEVICE_GET_NAME DBG_REQ;
#define DBG_DEVICE_GET_SUPPORTED_ACTIONS DBG_REQ;

#define DBG_DEVICE_PUT_ACTION_REQ DBG_REQ;
#define DBG_DEVICE_PUT_COMMAND_BLIND DBG_REQ;
#define DBG_DEVICE_PUT_COMMAND_BOOL DBG_REQ;
#define DBG_DEVICE_PUT_COMMAND_STRING DBG_REQ;
#define DBG_DEVICE_PUT_CONNECTED DBG_REQ;

#define DBG_CC_GET_BRIGHTNESS DBG_REQ;
#define DBG_CC_GET_CALIBRATOR_STATE DBG_REQ;
#define DBG_CC_GET_COVER_STATE DBG_REQ;
#define DBG_CC_GET_MAX_BRIGHTNESS DBG_REQ;

#define DBG_CC_PUT_CALIBRATOR_OFF DBG_REQ;
#define DBG_CC_PUT_CALIBRATOR_ON DBG_REQ;
#define DBG_CC_PUT_CLOSE_COVER DBG_REQ;
#define DBG_CC_PUT_HALT_COVER DBG_REQ;
#define DBG_CC_PUT_OPEN_COVER DBG_REQ;

#define DBG_SWITCH_GET_MAX_SWITCH DBG_REQ;
#define DBG_SWITCH_CAN_WRITE DBG_REQ;
#define DBG_SWITCH_GET_SWITCH DBG_REQ;
#define DBG_SWITCH_GET_SWITCH_DESCRIPTION DBG_REQ;
#define DBG_SWITCH_GET_SWITCH_NAME DBG_REQ;
#define DBG_SWITCH_GET_SWITCH_VALUE DBG_REQ;
#define DBG_SWITCH_GET_MIN_SWITCH_VALUE DBG_REQ;
#define DBG_SWITCH_GET_MAX_SWITCH_VALUE DBG_REQ;
#define DBG_SWITCH_GET_SWITCH_STEP DBG_REQ;

#define DBG_SWITCH_PUT_SET_SWITCH DBG_REQ;
#define DBG_SWITCH_PUT_SET_SWITCH_NAME DBG_REQ;
#define DBG_SWITCH_PUT_SET_SWITCH_VALUE DBG_REQ;

#define DBG_OBSERVING_CONDITIONS_GET_AVERAGE_PERIOD DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_CLOUD_COVER DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_DEW_POINT DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_HUMIDITY DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_PRESSURE DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_RAIN_RATE DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_SKY_BRIGHTNESS DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_SKY_QUALITY DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_SKY_TEMPERATURE DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_STAR_FWHM DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_TEMPERATURE DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_WIND_DIRECTION DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_WIND_GUES DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_WIND_SPEED DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_SENSOR_DESCRIPTION DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_GET_TIME_SINCE_LAST_UPDATE DBG_REQ;

#define DBG_OBSERVING_CONDITIONS_GET_PUT_AVERAGE_PERIOD DBG_REQ;
#define DBG_OBSERVING_CONDITIONS_PUT_REFRESH DBG_REQ;

#define DBG_FOCUSER_GET_ABSOLUT DBG_REQ;
#define DBG_FOCUSER_GET_IS_MOVING DBG_REQ;
#define DBG_FOCUSER_GET_MAX_INCREMENT DBG_REQ;
#define DBG_FOCUSER_GET_MAX_STEP DBG_REQ;
#define DBG_FOCUSER_GET_POSITION DBG_REQ;
#define DBG_FOCUSER_GET_STEP_SIZE DBG_REQ;
#define DBG_FOCUSER_GET_TEMP_COMP DBG_REQ;
#define DBG_FOCUSER_GET_TEMP_COMP_AVAILABLE DBG_REQ;
#define DBG_FOCUSER_GET_ TEMPERATUR DBG_REQ;

#define DBG_FOCUSER_PUT_TEMP_COMP DBG_REQ;
#define DBG_FOCUSER_PUT_HALT DBG_REQ;
#define DBG_FOCUSER_PUT_MOVE DBG_REQ;

// don't touch
// Debug request and response
#ifdef RELEASE
#define DBG_REQ

#define DBG_RESPOND_VALUE

#define DBG_END

#else

#define DBG_REQ                                                                                                                                                                                       \
    {                                                                                                                                                                                                 \
        char s[1024];                                                                                                                                                                                 \
        gDbg = true;                                                                                                                                                                                  \
        snprintf(s, sizeof(s), "Alpaca REQ (%d.%d.%d.%d) %s", request->client()->remoteIP()[0], request->client()->remoteIP()[1], request->client()->remoteIP()[2], request->client()->remoteIP()[3], \
                 request->url().c_str());                                                                                                                                                             \
        for (int i = 0; i < request->args(); i++)                                                                                                                                                     \
        {                                                                                                                                                                                             \
            int len = strlen(s);                                                                                                                                                                      \
            snprintf(&s[len], sizeof(s) - len, " - %s='%s", request->argName(i).c_str(), request->arg(i).c_str());                                                                                    \
        }                                                                                                                                                                                             \
        SLOG_INFO_PRINTF("%s\n", s);                                                                                                                                                                  \
    }

#define DBG_RESPOND_VALUE \
    if (gDbg)             \
        SLOG_INFO_PRINTF("Alpaca RSP %d %s\n", (int32_t)rsp_status.http_status, response);

#define DBG_END gDbg = false;

#endif
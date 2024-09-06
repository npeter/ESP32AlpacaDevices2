/**************************************************************************************************
  Filename:       AlpacaServer.hpp
  Revised:        $Date: 2024-01-14$
  Revision:       $Revision: 01 $

  Description:    ASCOM Alpaca Server

  Copyright 2024 peter_n@gmx.de. All rights reserved.
            based on https://github.com/elenhinan/ESPAscomAlpacaServer
**************************************************************************************************/
#pragma once
#include <Arduino.h>
#include <SPIFFS.h>
#include <esp_system.h>
#include <AsyncUDP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
//#include <OneButton.h>

#include "AlpacaDebug.h"
#include "AlpacaConfig.h"

const char kAlpacaDeviceCommand[] = "/api/v1/%s/%d/%s"; // <device_type>, <device_number>, <command>
const char kAlpacaDeviceSetup[] = "/setup/v1/%s/%d/%s"; // device_type, device_number, command

const char kAlpacaJsonType[] = "application/json";
const uint32_t kAlpacaDiscoveryLength = 64;
const char kAlpacaDiscoveryHeader[] = "alpacadiscovery";

// Lambda Handler Function for calling object function
#define LHF(method) \
    (ArRequestHandlerFunction)[this](AsyncWebServerRequest * request) { this->method(request); }


class AlpacaDevice;

enum struct HttpStatus_t // 
{
    kPassed = 200,         // request correctly formatted and passed to the device handler
    kInvalidRequest = 400, // device could not interprete the request
    kDeviceError = 500     // unexcpected device error
};

enum struct JsonValue_t
{
    kNoValue,
    kAsJsonStringValue,
    kAsPlainStringValue
};

enum struct Spelling_t
{
    kStrict = 0,
    kIgnoreCase = 1,
    kCheckBoth = 2,
    kNoMatch
};

struct AlpacaClient_t
{
    uint32_t client_id;             // connected with ClientID 1,... or 0 - not connected
    uint32_t client_transaction_id; // transactionId
    uint32_t time_ms;               // last client transaction time
    uint32_t max_service_time_ms;           // max time bitween two services
};

enum struct AlpacaErrorCode_t : int32_t
{
    Ok = 0,
    ActionNotImplementedException = (int32_t)0x0000040C, // to indicate that the requested action is not implemented in this driver.
    DriverBase = (int32_t)0x00000500,                    // The starting value for driver-specific error numbers.
    DriverMax = (int32_t)0x00000FFF,                     // The maximum value for driver-specific error numbers.
    InvalidOperationException = (int32_t)0x0000040B,     // to indicate that the requested operation can not be undertaken at this time.
    InvalidValue = (int32_t)0x00000401,                  // for reporting an invalid value.
    InvalidWhileParked = (int32_t)0x00000408,            // used to indicate that the attempted operation is invalid because the mount is currently in a Parked state.
    InvalidWhileSlaved = (int32_t)0x00000409,            // used to indicate that the attempted operation is invalid because the mount is currently in a Slaved state.
    NotConnected = (int32_t)0x00000407,                  // used to indicate that the communications channel is not connected.
    NotImplemented = (int32_t)0x00000400,                // for property or method not implemented.
    NotInCacheException = (int)0x0000040D,               // to indicate that the requested item is not present in the ASCOM cache.
    SettingsProviderError = (int)0x0000040A,             // related to settings.
    UnspecifiedError = (int)0x000004FF,                  // used when nothing else was specified.
    ValueNotSet = (int)0x00000402                        // for reporting that a value has not been set.
};

struct AlpacaRspStatus_t
{
    AlpacaErrorCode_t error_code;
    char error_msg[128];
    HttpStatus_t http_status;
};

class AlpacaServer
{
private:
    // Data for alpaca management description request
    String _mng_server_name = "empty";
    String _mng_manufacture = "DIY by @BigPet";
    String _mng_manufacture_version = "V1.0";
    String _mng_location = "Germany/Bavaria";

    // Logging see SLog
    String _syslog_host = "0.0.0.0";
    uint16_t _log_level = SLOG_DEBUG;
    bool _serial_log = true; // false/true: disable/enable logging after boot

    AsyncWebServer *_server_tcp;
    AsyncUDP _server_udp;
    uint16_t _port_tcp;
    uint16_t _port_udp;
    uint32_t _server_transaction_id = 0;

    char _uid[13]; // from wifi mac
    AlpacaDevice *_device[kAlpacaMaxDevices];
    int _n_devices = 0;

    char _settings_file[128] = "/settings.json";    
    bool _reset_request = false;

    AlpacaRspStatus_t _mng_rsp_status;
    AlpacaClient_t _mng_client_id;

    void _registerCallbacks();
    void _getApiVersions(AsyncWebServerRequest *request);
    void _getDescription(AsyncWebServerRequest *request);
    void _getConfiguredDevices(AsyncWebServerRequest *request);
    int32_t _paramIndex(AsyncWebServerRequest *request, const char *name, Spelling_t spelling);
    void _readJson(JsonObject &root);
    void _writeJson(JsonObject &root);
    void _getJsondata(AsyncWebServerRequest *request);
    void _getLinks(AsyncWebServerRequest *request);

    void _respond(AsyncWebServerRequest *request, AlpacaClient_t &client, AlpacaRspStatus_t &rsp_status, const char *str, JsonValue_t jason_string_value);

public:
    AlpacaServer(const String mng_server_name,
                 const String mng_manufacture,
                 const String mng_manufacture_version,
                 const String mng_location);

    void Begin(uint16_t udp_port = kAlpacaUdpPort, uint16_t tcp_port = kAlpacaTcpPort, bool mount_spiffs = true);
    void Loop();
    void AddDevice(AlpacaDevice *device);
    bool GetParam(AsyncWebServerRequest *request, const char *name, bool &value, Spelling_t spelling);
    bool GetParam(AsyncWebServerRequest *request, const char *name, float &value, Spelling_t spelling);
    bool GetParam(AsyncWebServerRequest *request, const char *name, double &value, Spelling_t spelling);
    bool GetParam(AsyncWebServerRequest *request, const char *name, int32_t &value, Spelling_t spelling);
    bool GetParam(AsyncWebServerRequest *request, const char *name, uint32_t &value, Spelling_t spelling);
    bool GetParam(AsyncWebServerRequest *request, const char *name, char *buffer, int buffer_size, Spelling_t spelling);

    void Respond(AsyncWebServerRequest *request, AlpacaClient_t &client, AlpacaRspStatus_t &rsp_status);
    void Respond(AsyncWebServerRequest *request, AlpacaClient_t &client, AlpacaRspStatus_t &rsp_status, int32_t int_value);
    void Respond(AsyncWebServerRequest *request, AlpacaClient_t &client, AlpacaRspStatus_t &rsp_status, double double_value);
    void Respond(AsyncWebServerRequest *request, AlpacaClient_t &client, AlpacaRspStatus_t &rsp_status, bool bool_value);
    void Respond(AsyncWebServerRequest *request, AlpacaClient_t &client, AlpacaRspStatus_t &rsp_status, const char *str_value, JsonValue_t jason_string_value);

    bool CheckMngClientData(AsyncWebServerRequest *req, Spelling_t spelling);

    bool LoadSettings();
    bool SaveSettings();
    void OnAlpacaDiscovery(AsyncUDPPacket &udpPacket);
    AsyncWebServer *getServerTCP() { return _server_tcp; }
    const char *GetUID() { return _uid; }
    const String GetSyslogHost() { return _syslog_host; };
    const uint16_t GetLogLvl() { return _log_level; };
    const bool GetSerialLog() { return _serial_log; };
    const bool GetResetRequest() { return _reset_request; };
    void SetResetRequest() { _reset_request = true; };

    // only for testing
    void RemoveSettingsFile() { SPIFFS.remove(_settings_file); }

    // Alpaca response status helpers ==============================================================================================
    void RspStatusClear(AlpacaRspStatus_t &rsp_status)
    {
        rsp_status.error_code = AlpacaErrorCode_t::Ok;
        rsp_status.http_status = HttpStatus_t::kPassed;
        strcpy(rsp_status.error_msg, "");
    }

    void ThrowRspStatusClientIDNotFound(AsyncWebServerRequest *req, AlpacaRspStatus_t &rsp_status)
    {
        rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
        rsp_status.http_status = HttpStatus_t::kPassed;
        snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - '%s' not found", req->url().c_str(), "ClientID");
        throw(&rsp_status);
    }

    void ThrowRspStatusClientIDInvalid(AsyncWebServerRequest *req, AlpacaRspStatus_t &rsp_status, int32_t clientID)
    {
        rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
        rsp_status.http_status = HttpStatus_t::kPassed;
        snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - '%s=%d' invalid", req->url().c_str(), "ClientID", clientID);
        throw(&rsp_status);
    }

    void ThrowRspStatusClientTransactionIDNotFound(AsyncWebServerRequest *req, AlpacaRspStatus_t &rsp_status)
    {
        rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
        rsp_status.http_status = HttpStatus_t::kPassed;
        snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - '%s' not found", req->url().c_str(), "ClientTransactionID");
        throw(&rsp_status);
    }

    void ThrowRspStatusClientTransactionIDInvalid(AsyncWebServerRequest *req, AlpacaRspStatus_t &rsp_status, int32_t clientTransactionID)
    {
        rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
        rsp_status.http_status = HttpStatus_t::kInvalidRequest;
        snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - '%s=%d' invalid", req->url().c_str(), "ClientTransactionID", clientTransactionID);
        throw(&rsp_status);
    }

    void ThrowRspStatusParameterNotFound(AsyncWebServerRequest *req, AlpacaRspStatus_t &rsp_status, const char *paraName)
    {
        rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
        rsp_status.http_status = HttpStatus_t::kInvalidRequest;
        snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - Parameter '%s' not found", req->url().c_str(), paraName);
        throw(&rsp_status);
    }

    void ThrowRspStatusParameterInvalidValue(AsyncWebServerRequest *req, AlpacaRspStatus_t &rsp_status, const char *paraName, int32_t paraValue)
    {
        rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
        rsp_status.http_status = HttpStatus_t::kPassed;
        snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - Parameter '%s=%d invalid", req->url().c_str(), paraName, paraValue);
        throw(&rsp_status);
    }

    void ThrowRspStatusParameterInvalidValue(AsyncWebServerRequest *req, AlpacaRspStatus_t &rsp_status, const char *paraName, bool paraValue)
    {
        rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
        rsp_status.http_status = HttpStatus_t::kPassed;
        snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - Parameter '%s=%s invalid", req->url().c_str(), paraName, paraValue?"true":"false");
        throw(&rsp_status);
    }

    void ThrowRspStatusParameterInvalidValue(AsyncWebServerRequest *req, AlpacaRspStatus_t &rsp_status, const char *paraName, double paraValue)
    {
        rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
        rsp_status.http_status = HttpStatus_t::kPassed;
        snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - Parameter '%s=%f invalid", req->url().c_str(), paraName, paraValue);
        throw(&rsp_status);
    }

    void ThrowRspStatusCommandStringInvalid(AsyncWebServerRequest *req, AlpacaRspStatus_t &rsp_status, const char *command_str)
    {
        rsp_status.error_code = AlpacaErrorCode_t::InvalidValue;
        rsp_status.http_status = HttpStatus_t::kPassed;
        snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - Command string %s invalid", req->url().c_str(), command_str);
        throw(&rsp_status);
    }
    void ThrowRspStatusClientAlreadyConnected(AsyncWebServerRequest *req, AlpacaRspStatus_t &rsp_status, int32_t clientID)
    {
        rsp_status.error_code = AlpacaErrorCode_t::InvalidOperationException;
        rsp_status.http_status = HttpStatus_t::kPassed;
        snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - Client with 'ClientID=%d' already connected", req->url().c_str(), clientID);
        throw(&rsp_status);
    }

    void ThrowRspStatusToMannyClients(AsyncWebServerRequest *req, AlpacaRspStatus_t &rsp_status, uint32_t maxNumOfClients)
    {
        rsp_status.error_code = AlpacaErrorCode_t::InvalidOperationException;
        rsp_status.http_status = HttpStatus_t::kPassed;
        snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - Too many (%d) clients connected", req->url().c_str(), maxNumOfClients);
        throw(&rsp_status);
    }

    void ThrowRspStatusClientNotConnected(AsyncWebServerRequest *req, AlpacaRspStatus_t &rsp_status, int32_t clientID)
    {
        rsp_status.error_code = AlpacaErrorCode_t::InvalidOperationException;
        rsp_status.http_status = HttpStatus_t::kPassed;
        snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - Client 'ClientID=%d' not connected", req->url().c_str(), clientID);
        throw(&rsp_status);
    }

    void ThrowRspStatusCommandNotImplemented(AsyncWebServerRequest *req, AlpacaRspStatus_t &rsp_status, const char *command)
    {
        rsp_status.error_code = AlpacaErrorCode_t::NotImplemented;
        rsp_status.http_status = HttpStatus_t::kPassed;
        snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - Command '%s' not implemented", req->url().c_str(), command);
        throw(&rsp_status);
    }

    void ThrowRspStatusActionNotImplemented(AsyncWebServerRequest *req, AlpacaRspStatus_t &rsp_status, const char *action, const char *parameters)
    {
        rsp_status.error_code = AlpacaErrorCode_t::NotImplemented;
        rsp_status.http_status = HttpStatus_t::kPassed;
        snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - Action '%s' with Parameters '%s' not implemented", req->url().c_str(), action, parameters);
        throw(&rsp_status);
    }

    void ThrowRspStatusDeviceNotImplemented(AsyncWebServerRequest *req, AlpacaRspStatus_t &rsp_status, const char *device)
    {
        rsp_status.error_code = AlpacaErrorCode_t::NotImplemented;
        rsp_status.http_status = HttpStatus_t::kPassed;
        snprintf(rsp_status.error_msg, sizeof(rsp_status.error_msg), "%s - Device '%s' not implemented", req->url().c_str(), device);
        throw(&rsp_status);
    }
};

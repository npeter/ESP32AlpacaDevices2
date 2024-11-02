/**************************************************************************************************
  Filename:       AlpacaServer.cpp
  Revised:        $Date: 2024-01-14$
  Revision:       $Revision: 01 $

  Description:    ASCOM Alpaca Server

  Copyright 2024 peter_n@gmx.de. All rights reserved.
            based on https://github.com/elenhinan/ESPAscomAlpacaServer
**************************************************************************************************/
#define ALPACA_VAR_DECLS // don't change
#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include "AlpacaServer.h"
#include "AlpacaDevice.h"
#ifdef ALPACA_ENABLE_OTA_UPDATE
#include "ElegantOTA.h"
#endif

// discovery package
typedef struct
{
private:
    char _header[15];
    char _version;
    char _reserved[48];

public:
    bool valid() { return strncmp(_header, "alpacadiscovery", sizeof("alpacadiscovery") - 1) == 0; }
    char version() { return _version; }
} AlpacaDiscoveryPacket;

static_assert(sizeof(AlpacaDiscoveryPacket) == kAlpacaDiscoveryLength, "Wrong size of struct");

typedef union
{
    AlpacaDiscoveryPacket data;
    char buffer[kAlpacaDiscoveryLength];
} AlpacaDiscoveryBuffer;

AlpacaServer::AlpacaServer(const String mng_server_name,
                           const String mng_manufacture,
                           const String mng_manufacture_version,
                           const String mng_location)
{
    _mng_server_name = mng_server_name;
    _mng_manufacture = mng_manufacture;
    _mng_manufacture_version = mng_manufacture_version;
    _mng_location = mng_location;
}

// initialize alpaca server
void AlpacaServer::Begin(uint16_t udp_port, uint16_t tcp_port, bool mount_littlefs)
{
    // Lib version
    SLOG_PRINTF(SLOG_INFO, "ESP32AlpacaDevice2 Library version=%s\n", esp32_alpaca_device_library_version);

    // Get unique ID from wifi macadr. Default 000000000000
    uint8_t mac_adr[6] = {0};
    esp_wifi_get_mac(WIFI_IF_STA, mac_adr);
    snprintf(_uid, sizeof(_uid), "%02X%02X%02X%02X%02X%02X", mac_adr[0], mac_adr[1], mac_adr[2], mac_adr[3], mac_adr[4], mac_adr[5]);
    SLOG_PRINTF(SLOG_DEBUG, "_uid=%s\n", _uid);

    RspStatusClear(_mng_rsp_status);
    // Setup filesystem
    if (mount_littlefs)
    {
        if (!LittleFS.begin())
        {
            SLOG_ERROR_PRINTF("LittleFS mounting error\n");
        }
    }

    // setup ports
    _port_udp = udp_port;
    _port_tcp = tcp_port;

    SLOG_INFO_PRINTF("Ascom Alpaca discovery UDP port %d\n", _port_udp);

    _server_udp.listen(_port_udp);
    _server_udp.onPacket([this](AsyncUDPPacket &udpPacket)
                         { this->OnAlpacaDiscovery(udpPacket); });

    SLOG_INFO_PRINTF("Ascom Alpaca server TCP port %d\n", _port_tcp)

    _server_tcp = new AsyncWebServer(_port_tcp);
    _server_tcp->begin();

    _server_tcp->onNotFound([this](AsyncWebServerRequest *request)
                            {
                                String url = request->url();
                                request->send(400, "text/plain", "Not found: '" + url + "'");
                                SLOG_WARNING_PRINTF("Url (%s) not found\n", url.c_str()); });

    _registerCallbacks();
#ifdef ALPACA_ENABLE_OTA_UPDATE
    ElegantOTA.begin(_server_tcp);
#endif
}

void AlpacaServer::Loop()
{
    for (int32_t i = 0; i < _n_devices; i++)
    {
        _device[i]->CheckClientConnectionTimeout();
    }
#ifdef ALPACA_ENABLE_OTA_UPDATE
    ElegantOTA.loop();
#endif
}

// add alpaca device to server
void AlpacaServer::AddDevice(AlpacaDevice *device)
{
    if (_n_devices == kAlpacaMaxDevices)
    {
        SLOG_ERROR_PRINTF("max alpaca devices (%d) exceeded\n", kAlpacaMaxDevices);
        return;
    }

    // get device_number for device_type
    int deviceNumber = 0;
    const char *deviceType = device->GetDeviceType();
    // loop through registered devices and count
    for (int i = 0; i < _n_devices; i++)
    {
        if (strcmp(_device[i]->GetDeviceType(), deviceType) == 0)
        {
            deviceNumber++;
        }
    }
    // and set device number
    _device[_n_devices++] = device;
    device->SetAlpacaServer(this);
    device->SetDeviceNumber(deviceNumber);
    device->RegisterCallbacks();
    SLOG_INFO_PRINTF("ADD deviceType=%s deviceNumber=%d\n", deviceType, deviceNumber);
}

// register callbacks for REST API
void AlpacaServer::_registerCallbacks()
{
    // setup rest api
    SLOG_INFO_PRINTF("REGISTER handler for \"/management/apiversions\" to _getApiVersions\n");
    _server_tcp->on("/management/apiversions", HTTP_GET, LHF(_getApiVersions));
    SLOG_INFO_PRINTF("REGISTER handler for \"/management/v1/description\" to _getDescription\n");
    _server_tcp->on("/management/v1/description", HTTP_GET, LHF(_getDescription));
    SLOG_INFO_PRINTF("REGISTER handler for \"/management/v1/configureddevices\" to _getConfiguredDevices\n");
    _server_tcp->on("/management/v1/configureddevices", HTTP_GET, LHF(_getConfiguredDevices));

    // setup webpages
    _server_tcp->serveStatic("/setup", LittleFS, "/www/setup.html");
    _server_tcp->serveStatic(_settings_file, LittleFS, _settings_file);
    _server_tcp->serveStatic("/", LittleFS, "/").setCacheControl("max-age=3600");

    SLOG_INFO_PRINTF("REGISTER handler for \"/jsondata\" to _getJsondata\n");
    _server_tcp->on("/jsondata", HTTP_GET, LHF(_getJsondata));

    SLOG_INFO_PRINTF("REGISTER handler for \"/links\" to _getLinks\n");
    _server_tcp->on("/links", HTTP_GET, LHF(_getLinks));

    // jsonhandler
    AsyncCallbackJsonWebHandler *jsonhandler = new AsyncCallbackJsonWebHandler("/jsondata", [this](AsyncWebServerRequest *request, JsonVariant &json)
                                                                               {
    SLOG_PRINTF(SLOG_INFO, "BEGIN REQ (%s) ...\n", request->url().c_str());
       DBG_REQ
       JsonObject jsonObj = json.as<JsonObject>();
       this->_readJson(jsonObj);
       request->send(200, F("application/json"), "{\"recieved\":\"true\"}"); 
        SLOG_PRINTF(SLOG_INFO, "... END REQ (%s)\n", request->url().c_str());    
        DBG_END });
    _server_tcp->addHandler(jsonhandler);

    // /save_settings  handler
    _server_tcp->on("/save_settings", HTTP_GET, [this](AsyncWebServerRequest *request)
                    {
        SLOG_PRINTF(SLOG_INFO, "BEGIN REQ (%s) ...\n", request->url().c_str());               
        if (this->SaveSettings())
            request->send(200,"application/json","{\"saved\":true}");
        else
            request->send(400,"application/json","{\"saved\":false}"); 
        SLOG_PRINTF(SLOG_INFO, "... END REQ (%s)\n", request->url().c_str());                    
        DBG_END });

    // /reset handler
    _server_tcp->on("/reset", HTTP_GET, [this](AsyncWebServerRequest *request)
                    {
        SLOG_PRINTF(SLOG_INFO,"BEGIN REQ (%s) ... RESET\n", request->url().c_str());
        DBG_REQ;
        _reset_request = true;
        request->send(200,"application/json","{\"activated\":true}");
        DBG_END; });
}

void AlpacaServer::_getApiVersions(AsyncWebServerRequest *request)
{
    DBG_SERVER_GET_MNG_API_VERSION
    RspStatusClear(_mng_rsp_status);
    _mng_client_id.client_id = 0;
    _mng_client_id.client_transaction_id = 0;
    // checkMngClientData(request, Spelling_t::kIgnoreCase);
    Respond(request, _mng_client_id, _mng_rsp_status, ALPACA_INTERFACE_VERSION, JsonValue_t::kAsPlainStringValue);
    DBG_END
}

void AlpacaServer::_getDescription(AsyncWebServerRequest *request)
{
    DBG_SERVER_GET_MNG_DESCRIPTION

    RspStatusClear(_mng_rsp_status);
    _mng_client_id.client_id = 0;
    _mng_client_id.client_transaction_id = 0;
    // checkMngClientData(request, Spelling_t::kIgnoreCase);
    char mng_description[1024] = {0};
    snprintf(mng_description, sizeof(mng_description),
             "{\"ServerName\":\"%s\",\"Manufacturer\":\"%s\",\"ManufacturerVersion\":\"%s\",\"Location\":\"%s\"}",
             _mng_server_name.c_str(), _mng_manufacture.c_str(), _mng_manufacture_version.c_str(), _mng_location.c_str());
    Respond(request, _mng_client_id, _mng_rsp_status, mng_description, JsonValue_t::kAsPlainStringValue);
    DBG_END
}

// Return list of dicts describing connected alpaca devices
void AlpacaServer::_getConfiguredDevices(AsyncWebServerRequest *request)
{
    DBG_SERVER_GET_MNG_CONFIGUREDDEVICES
    char value[kAlpacaMaxDevices * 256 + kAlpacaMaxDevices] = "";
    char deviceinfo[256];

    RspStatusClear(_mng_rsp_status);
    _mng_client_id.client_id = 0;
    _mng_client_id.client_transaction_id = 0;

    // checkMngClientData(request, Spelling_t::kIgnoreCase);

    strcat(value, "[");
    for (int i = 0; i < _n_devices; i++)
    {
        snprintf(
            deviceinfo,
            sizeof(deviceinfo),
            "{\"DeviceName\":\"%s\",\"DeviceType\":\"%s\",\"DeviceNumber\":%i,\"UniqueID\":\"%s\"}",
            _device[i]->GetDeviceName(),
            _device[i]->GetDeviceType(),
            _device[i]->GetDeviceNumber(),
            _device[i]->GetDeviceUID());
        strcat(value, deviceinfo);
        if (i < _n_devices - 1)
            strcat(value, ","); // add comma to all but last device
    }
    strcat(value, "]");
    Respond(request, _mng_client_id, _mng_rsp_status, (const char *)value, JsonValue_t::kAsPlainStringValue);
    DBG_END
}

// return index of parameter 'name' in PUT request, return -1 if not found
int32_t AlpacaServer::_paramIndex(AsyncWebServerRequest *request, const char *name, Spelling_t spelling)
{
    for (int i = 0; i < request->args(); i++)
    {
        String arg_name = request->argName(i);
        if (arg_name != nullptr)
        {
            if ((spelling == Spelling_t::kStrict) ? arg_name.equals(name) : arg_name.equalsIgnoreCase(name))
                return i;
        }
        else
        {
            return -1;
        }
    }
    return -1;
}

// get value of parameter 'name' in PUT request and return true, return false if not found or value invalid
// name - casing mantadory
// value - has to be "true" or "false"; no casing
bool AlpacaServer::GetParam(AsyncWebServerRequest *request, const char *name, bool &value, Spelling_t spelling)
{
    bool result = false;

    int index = _paramIndex(request, name, spelling);
    if (index >= 0)
    {
        if (!(request->arg(index).isEmpty()))
        {
            if (request->arg(index).equalsIgnoreCase("True"))
            {
                value = true;
                result = true;
            }
            else if (request->arg(index).equalsIgnoreCase("False"))
            {
                value = false;
                result = true;
            }
        }
    }
    return result;
}

// get value of parameter 'name' in PUT request and return true, return false if not found
bool AlpacaServer::GetParam(AsyncWebServerRequest *request, const char *name, double &value, Spelling_t spelling)
{
    int32_t index = _paramIndex(request, name, spelling);
    if (index >= 0)
    {
        try
        {
            value = std::stod(request->arg(index).c_str());
            return true;
        }
        catch (std::out_of_range const &ex)
        {
            return false;
        }
        catch (std::invalid_argument const &ex)
        {
            return false;
        }
    }
    return false;
}

// get value of parameter 'name' in PUT request and return true, return false if not found
bool AlpacaServer::GetParam(AsyncWebServerRequest *request, const char *name, float &value, Spelling_t spelling)
{
    int32_t index = _paramIndex(request, name, spelling);
    if (index >= 0)
    {
        try
        {
            value = std::stof(request->arg(index).c_str());
            return true;
        }
        catch (std::out_of_range const &ex)
        {
            return false;
        }
        catch (std::invalid_argument const &ex)
        {
            return false;
        }
    }
    return false;
}

// using namespace std;
//  get value of parameter 'name' in PUT request and return true, return false if not found
bool AlpacaServer::GetParam(AsyncWebServerRequest *request, const char *name, int32_t &value, Spelling_t spelling)
{
    int32_t index = _paramIndex(request, name, spelling);
    if (index >= 0)
    {
        try
        {
            if (!request->arg(index).isEmpty())
            {
                value = std::stoi(request->arg(index).c_str());
                return true;
            }
            else
            {
                return false;
            }
        }
        catch (std::out_of_range const &ex)
        {
            return false;
        }
        catch (std::invalid_argument const &ex)
        {
            return false;
        }
    }
    return false;
}

// get value of parameter 'name' in PUT request and return true, return false if not found or invalid
bool AlpacaServer::GetParam(AsyncWebServerRequest *request, const char *name, uint32_t &value, Spelling_t spelling)
{
    int32_t index = _paramIndex(request, name, spelling);
    if (index >= 0)
    {
        try
        {
            value = std::stoul(request->arg(index).c_str());
            return true;
        }
        catch (std::out_of_range const &ex)
        {
            return false;
        }
        catch (std::invalid_argument const &ex)
        {
            return false;
        }
    }
    return false;
}

// get value of parameter 'name' in request and return true, return false if not found
bool AlpacaServer::GetParam(AsyncWebServerRequest *request, const char *name, char *buffer, int buffer_size, Spelling_t spelling)
{
    int32_t index = _paramIndex(request, name, spelling);
    if (index >= 0)
    {
        request->arg(index).toCharArray(buffer, buffer_size);
        buffer[buffer_size - 1] = '\0';
        return true;
    }
    return false;
}

// Respons without value
void AlpacaServer::Respond(AsyncWebServerRequest *request, AlpacaClient_t &client, AlpacaRspStatus_t &rsp_status)
{
    SLOG_DEBUG_PRINTF("Respond(without value)\n");
    _respond(request, client, rsp_status, nullptr, JsonValue_t::kNoValue);
}
// Response with int32_t value
void AlpacaServer::Respond(AsyncWebServerRequest *request, AlpacaClient_t &client, AlpacaRspStatus_t &rsp_status, int32_t int_value)
{
    SLOG_DEBUG_PRINTF("Respond(with int32_t value)\n");
    char s[33];
    _respond(request, client, rsp_status, itoa((int)int_value, s, 10), JsonValue_t::kAsPlainStringValue);
}
// Response with double value
void AlpacaServer::Respond(AsyncWebServerRequest *request, AlpacaClient_t &client, AlpacaRspStatus_t &rsp_status, double double_value)
{
    SLOG_DEBUG_PRINTF("Respond(with double value)\n");
    char s[64];
    snprintf(s, sizeof(s), "%f", double_value);
    _respond(request, client, rsp_status, s, JsonValue_t::kAsPlainStringValue);
}
// Response with bool value
void AlpacaServer::Respond(AsyncWebServerRequest *request, AlpacaClient_t &client, AlpacaRspStatus_t &rsp_status, bool bool_value)
{
    SLOG_DEBUG_PRINTF("Respond(with bool value)\n");
    _respond(request, client, rsp_status, bool_value ? "true" : "false", JsonValue_t::kAsPlainStringValue);
}
// Response with optional  quoted string value
void AlpacaServer::Respond(AsyncWebServerRequest *request, AlpacaClient_t &client, AlpacaRspStatus_t &rsp_status, const char *str_value, JsonValue_t jason_string_value)
{
    SLOG_DEBUG_PRINTF("Respond(with with optional  quoted string value)\n");
    _respond(request, client, rsp_status, str_value, jason_string_value);
}

// prepare and send json response to alpaca client.
// as_json_str==true will aditional quote the value
void AlpacaServer::_respond(AsyncWebServerRequest *request, AlpacaClient_t &client, AlpacaRspStatus_t &rsp_status, const char *value, JsonValue_t jason_string_value)
{
    char response[2058 + 256];

    _server_transaction_id++;
    if (jason_string_value == JsonValue_t::kNoValue)
    {
        // "{\n\t\"ClientTransactionID\": %i,\n\t\"ServerTransactionID\": %i,\n\t\"ErrorNumber\": %i,\n\t\"ErrorMessage\": \"%s\"\n}"
        snprintf(response, sizeof(response), "{ \"ClientTransactionID\": %i, \"ServerTransactionID\": %i, \"ErrorNumber\": %i, \"ErrorMessage\": \"%s\"}",
                 client.client_transaction_id, _server_transaction_id, rsp_status.error_code, rsp_status.error_msg);
    }
    else if (jason_string_value == JsonValue_t::kAsJsonStringValue)
    {
        // "{\n\t\"Value\": \"%s\",\n\t\"ClientTransactionID\": %i,\n\t\"ServerTransactionID\": %i,\n\t\"ErrorNumber\": %i,\n\t\"ErrorMessage\": \"%s\"\n}"
        snprintf(response, sizeof(response), "{ \"Value\": \"%s\", \"ClientTransactionID\": %i, \"ServerTransactionID\": %i, \"ErrorNumber\": %i, \"ErrorMessage\": \"%s\"}",
                 value, client.client_transaction_id, _server_transaction_id, rsp_status.error_code, rsp_status.error_msg);
    }
    else
    {
        // "{\n\t\"Value\": %s,\n\t\"ClientTransactionID\": %i,\n\t\"ServerTransactionID\": %i,\n\t\"ErrorNumber\": %i,\n\t\"ErrorMessage\": \"%s\"\n}"
        snprintf(response, sizeof(response), "{ \"Value\": %s, \"ClientTransactionID\": %i, \"ServerTransactionID\": %i, \"ErrorNumber\": %i, \"ErrorMessage\": \"%s\"}",
                 value, client.client_transaction_id, _server_transaction_id, rsp_status.error_code, rsp_status.error_msg);
    }
    request->send((int32_t)rsp_status.http_status, kAlpacaJsonType, response);
    DBG_RESPOND_VALUE;
}

// Handler for replying to ascom alpaca discovery UDP packet
void AlpacaServer::OnAlpacaDiscovery(AsyncUDPPacket &udpPacket)
{
    // check for arrived UDP packet at port
    int length = udpPacket.length();
    SLOG_PRINTF(SLOG_INFO, "BEGIN length=%d ...\n", length);

    if (length == 0)
        return;

    SLOG_PRINTF(SLOG_NOTICE, "... Remote ip %03d.%03d.%03d.%03d ...\n", udpPacket.remoteIP()[0], udpPacket.remoteIP()[1], udpPacket.remoteIP()[2], udpPacket.remoteIP()[3]);

    // check size
    if (length < 16)
    {
        SLOG_PRINTF(SLOG_ERROR, "Wrong packet size %d\n", length);
        return;
    }

    // check package content
    AlpacaDiscoveryPacket *alpaca_packet = (AlpacaDiscoveryPacket *)udpPacket.data();
    if (alpaca_packet->valid())
    {
        SLOG_PRINTF(SLOG_INFO, "... Header v.=%c ...\n", alpaca_packet->version());
    }
    else
    {
        SLOG_ERROR_PRINTF("Alpaca Discovery - Header mismatch\n");
        return;
    }

    // reply port to ascom tcp server
    uint8_t resp_buf[32];
    int resp_len = snprintf((char *)resp_buf, sizeof(resp_buf), "{\"AlpacaPort\":%d}", _port_tcp);
    _server_udp.writeTo(resp_buf, resp_len, udpPacket.remoteIP(), udpPacket.remotePort());
    SLOG_PRINTF(SLOG_NOTICE, "... END rsp=%s\n", resp_buf);
}

void AlpacaServer::_getJsondata(AsyncWebServerRequest *request)
{
    SLOG_PRINTF(SLOG_INFO, "BEGIN ...\n");
    DBG_REQ
    JsonDocument doc;
    JsonObject root = doc.to<JsonObject>();
    _writeJson(root);
    String ser_json = "";
    serializeJson(root, ser_json);
    request->send(200, kAlpacaJsonType, ser_json);
    DBG_JSON_PRINTFJ(SLOG_NOTICE, root, "... END ser_json=<%s>\n", _ser_json_);
    DBG_END
}

void AlpacaServer::_getLinks(AsyncWebServerRequest *request)
{
    SLOG_PRINTF(SLOG_INFO, "BEGIN ...\n");
    DBG_REQ
    JsonDocument doc;
    JsonObject root = doc.to<JsonObject>();
    root["Server"] = "/setup";
    for (int i = 0; i < _n_devices; i++)
    {
        root[_device[i]->GetDeviceName()] = _device[i]->GetDeviceURL();
    }

    String ser_json = "";
    serializeJson(root, ser_json);
    request->send(200, kAlpacaJsonType, ser_json);
    DBG_JSON_PRINTFJ(SLOG_INFO, root, "... END ser_json=<%s>\n", _ser_json_);
    DBG_END
}

void AlpacaServer::_readJson(JsonObject &root)
{
    DBG_JSON_PRINTFJ(SLOG_INFO, root, "BEGIN (root=<%s>) ...\n", _ser_json_);

    _mng_server_name = root["Name"] | _mng_server_name;
    _port_tcp = root["TCP_port"] | _port_tcp;
    _port_udp = root["UDP_port"] | _port_udp;
    _syslog_host = root["SYSLOG_host"] | _syslog_host;
    _log_level = root["LOG_level"] | SLOG_DEBUG;
    _serial_log = (root["SERIAL_log"] | 1) == 0 ? false : true;

    SLOG_PRINTF(SLOG_INFO, "... END _mng_server_name=%s _port_tcp=%d _port_udp=%d _syslog_host=%s _log_level=%d _serial_log=%s\n",
                _mng_server_name.c_str(), _port_tcp, _port_udp, _syslog_host.c_str(), _log_level, _serial_log == true ? "true" : "false");
}

void AlpacaServer::_writeJson(JsonObject &root)
{
    SLOG_PRINTF(SLOG_INFO, "BEGIN ...\n");
    // read-only values marked with #
    root["Name"] = _mng_server_name;
    root["UID"] = _uid;
    root["TCP_port"] = _port_tcp;
    root["UDP_port"] = _port_udp;
    root["SYSLOG_host"] = _syslog_host;
    root["LOG_level"] = _log_level;
    root["SERIAL_log"] = _serial_log ? 1 : 0;

    DBG_JSON_PRINTFJ(SLOG_NOTICE, root, "... END root=<%s>\n", _ser_json_);
}

bool AlpacaServer::SaveSettings()
{
    SLOG_PRINTF(SLOG_INFO, "BEGIN ...\n")
    JsonDocument doc;
    JsonObject root = doc.to<JsonObject>();
    DBG_JSON_PRINTFJ(SLOG_INFO, doc, "... doc=<%s> ...\n", _ser_json_);
    _writeJson(root);
    for (int i = 0; i < _n_devices; i++)
    {
        JsonObject json_obj = root[_device[i]->GetDeviceUID()].to<JsonObject>();
        _device[i]->AlpacaWriteJson(json_obj);
    }
    DBG_JSON_PRINTFJ(SLOG_NOTICE, root, "... root=<%s> ...\n", _ser_json_);

    LittleFS.remove(_settings_file);
    File file = LittleFS.open(_settings_file, FILE_WRITE);
    if (!file)
    {
        SLOG_PRINTF(SLOG_WARNING, "... END LittleFS could not create %s\n", _settings_file);
        return false;
    }
    if (serializeJson(doc, file) == 0)
    {
        SLOG_PRINTF(SLOG_WARNING, "... END ArduinoJson failed to write %s\n", _settings_file);
        file.close();
        return false;
    }
    else
    {
        SLOG_PRINTF(SLOG_INFO, "... END ArduinoJson wrote to %s succesfully\n", _settings_file);
    }
    file.close();
    return true;
}

bool AlpacaServer::LoadSettings()
{
    SLOG_PRINTF(SLOG_INFO, "BEGIN ...\n");
    JsonDocument doc;

    File file = LittleFS.open(_settings_file, FILE_READ);
    if (!file)
    {
        SLOG_WARNING_PRINTF("LittleFS: %s could not open\n", _settings_file);
        return false;
    }
    DeserializationError error = deserializeJson(doc, file);
    JsonObject root = doc.as<JsonObject>();
    file.close();
    if (error)
    {
        DBG_JSON_PRINTFJ(SLOG_WARNING, root, "failed to parse settings.json\n");
        return false;
    }

    SLOG_PRINTF(SLOG_INFO, "... LittleFS: %s loaded ...\n", _settings_file);
    _readJson(root);

    for (int i = 0; i < _n_devices; i++)
    {
        JsonObject json_obj = root[_device[i]->GetDeviceUID()];
        DBG_JSON_PRINTFJ(SLOG_INFO, json_obj, "... root[_device[%d]->getDeviceUID()]=<%s> ...\n", i, _ser_json_);

        if (json_obj)
            _device[i]->AlpacaReadJson(json_obj);
    }

    DBG_JSON_PRINTFJ(SLOG_NOTICE, root, "... END root=<%s>\n", _ser_json_);
    return true;
}

/*
 * Check clientID and clientTransactionId
 * fill mng rspStatus and clientIdx
 */
bool AlpacaServer::CheckMngClientData(AsyncWebServerRequest *req, Spelling_t spelling)
{
    RspStatusClear(_mng_rsp_status);

    _mng_client_id.client_id = 0;
    _mng_client_id.client_transaction_id = 0;

    try
    {
        if (GetParam(req, "ClientID", _mng_client_id.client_id, spelling) == false)
            ThrowRspStatusClientIDNotFound(req, _mng_rsp_status);

        if (GetParam(req, "ClientTransactionID", _mng_client_id.client_transaction_id, spelling) == false)
            ThrowRspStatusClientTransactionIDNotFound(req, _mng_rsp_status);

        if (_mng_client_id.client_transaction_id <= 0)
            ThrowRspStatusClientTransactionIDInvalid(req, _mng_rsp_status, _mng_client_id.client_transaction_id);
    }
    catch (AlpacaRspStatus_t *rspStatus)
    {
        return false;
    }
    return true;
}

/**************************************************************************************************
  Filename:       AlpacaSwitch.h
  Revised:        $Date: 2024-01-28$
  Revision:       $Revision: 01 $
  Description:    Common ASCOM Alpaca Switch V2

  Copyright 2024-2025 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#pragma once
#include "AlpacaDevice.h"

const size_t kSwitchNameSize = 32;         // Max. size of switch device name incl. '\0'
const size_t kSwitchDescriptionSize = 128; // Max. size of switch description incl. '\0'

/**
 * @brief Switch device change type: Asynchron/synchron
 */
enum struct SwitchAsyncType_t
{
    kAsyncType,
    kNoAsyncType
};

/**
 * @brief Primary switch device data type
 */
enum struct SwitchValueType_t
{
    kBool,
    kDouble
};

/**
 * @brief Switch device description and states. Maintained by AlpacaSwitch
 *        Firmware  - Fix; Defined by firmware
 *        Init      - Defined by setup page or firmware (see init_by_setup)
 *        Operation - Modified at runtime
 *        Driver    - Impliziet maintaine by the driver
 */
struct SwitchDevice_t
{
    bool init_by_setup;                       // Firmware  - init via setup web page / init with const values ... Begin()
    bool can_write;                           // Init      - switch is read_only / read_write
    char name[kSwitchNameSize];               // Operation - switch name
    char description[kSwitchDescriptionSize]; // Init      - switch description
    double value;                             // Operation - switch value; 0.0 or 1.0 if boolean
    double min_value;                         //Init       - min switch value; 0.0 if boolean
    double max_value;                         //Init       - max switch value; 1.0 if boolean
    double step;                              //Init       - switch steps; 1.0 if boolean
    SwitchAsyncType_t async_type;             //Init       - switch set type is AsyncType / NoAsyncType
    bool is_bool;                             // Driver    - switch type flag for fast access
    bool state_change_complete;               // Driver    - Switch async change state; Always true if NoAsyncType
    bool has_been_cancelled;                    // Driver    - Async switch has been cancled; Always false if NoAsyncType
    uint32_t set_time_stamp_ms;               // Driver    - Timestamp [ms] of set/setasync/compleeted
};

class AlpacaSwitch : public AlpacaDevice
{
private:
    uint32_t _max_switch_devices = 0;
    SwitchDevice_t *_p_switch_devices;

    void _alpacaGetMaxSwitch(AsyncWebServerRequest *request);
    void _alpacaGetCanWrite(AsyncWebServerRequest *request);
    void _alpacaGetSwitch(AsyncWebServerRequest *request);
    void _alpacaGetSwitchDescription(AsyncWebServerRequest *request);
    void _alpacaGetSwitchName(AsyncWebServerRequest *request);
    void _alpacaGetSwitchValue(AsyncWebServerRequest *request);
    void _alpacaGetMinSwitchValue(AsyncWebServerRequest *request);
    void _alpacaGetMaxSwitchValue(AsyncWebServerRequest *request);
    void _alpacaGetSwitchStep(AsyncWebServerRequest *request);
    void _alpacaGetCanAsync(AsyncWebServerRequest *request);
    void _alpacaGetStateChangeComplete(AsyncWebServerRequest *request);

    void _alpacaPutSetSwitch(AsyncWebServerRequest *request, SwitchValueType_t value_type, SwitchAsyncType_t async_type);

    void _alpacaPutSetSwitchWrapper(AsyncWebServerRequest *request) { _alpacaPutSetSwitch(request, SwitchValueType_t::kBool, SwitchAsyncType_t::kNoAsyncType); };
    void _alpacaPutSetAsyncWrapper(AsyncWebServerRequest *request) { _alpacaPutSetSwitch(request, SwitchValueType_t::kBool, SwitchAsyncType_t::kAsyncType); };
    void _alpacaPutSetSwitchValueWrapper(AsyncWebServerRequest *request) { _alpacaPutSetSwitch(request, SwitchValueType_t::kDouble, SwitchAsyncType_t::kNoAsyncType); };
    void _alpacaPutSetAsyncValueWrapper(AsyncWebServerRequest *request) { _alpacaPutSetSwitch(request, SwitchValueType_t::kDouble, SwitchAsyncType_t::kAsyncType); };

    void _alpacaPutCancleAsync(AsyncWebServerRequest *request);
    void _alpacaPutSetSwitchName(AsyncWebServerRequest *request);

    void AlpacaPutAction(AsyncWebServerRequest *request);
    void AlpacaPutCommandBlind(AsyncWebServerRequest *request);
    void AlpacaPutCommandBool(AsyncWebServerRequest *request);
    void AlpacaPutCommandString(AsyncWebServerRequest *request);

    /* devices specific handlers and helpers */
    virtual const bool _putAction(const char *const action, const char *const parameters, char *string_response, size_t string_response_size) = 0;
    virtual const bool _putCommandBlind(const char *const command, const char *const raw, bool &bool_response) = 0;
    virtual const bool _putCommandBool(const char *const command, const char *const raw, bool &bool_response) = 0;
    virtual const bool _putCommandString(const char *const command_str, const char *const raw, char *string_response, size_t string_response_size) = 0;

    virtual const char *const _getFirmwareVersion() { return "-"; };
    /**
     * @brief Write to physical device
     * @return true/false - write was succesful/not succesfull
     */
    virtual const bool _writeSwitchValue(uint32_t id, double value, SwitchAsyncType_t async_type) = 0;

    // private helpers
    bool _getAndCheckId(AsyncWebServerRequest *request, uint32_t &id, Spelling_t spelling);
    const bool _doubleValueToBoolValue(uint32_t id, double double_value) { return (double_value != _p_switch_devices[id].min_value); };
    const double _boolValueToDoubleValue(uint32_t id, bool bool_value) { return (bool_value ? _p_switch_devices[id].max_value : _p_switch_devices[id].min_value); };
    const bool _getDeviceStateList(size_t buf_len, char *buf);
    void  _InitSwitchDevicesInternals(uint32_t id);
protected:
    AlpacaSwitch(uint32_t num_of_switch_devices = 8);
    void Begin();
    void RegisterCallbacks();

    const size_t GetMaxSwitch() { return _max_switch_devices; };
    // geter id has to be correct!
    const bool GetSwitchInitBySetup(uint32_t id) { return _p_switch_devices[id < _max_switch_devices ? id : 0].init_by_setup; };
    const bool GetSwitchCanWrite(uint32_t id) { return _p_switch_devices[id < _max_switch_devices ? id : 0].can_write; };
    const char *GetSwitchName(uint32_t id) { return _p_switch_devices[id < _max_switch_devices ? id : 0].name; };
    const char *GetSwitchDescription(uint32_t id) { return _p_switch_devices[id < _max_switch_devices ? id : 0].description; };
    const bool GetValue(uint32_t id) { return _doubleValueToBoolValue((id < _max_switch_devices ? id : 0), _p_switch_devices[(id < _max_switch_devices ? id : 0)].value); };
    const double GetSwitchValue(uint32_t id) { return _p_switch_devices[id < _max_switch_devices ? id : 0].value; };
    const double GetSwitchMinValue(uint32_t id) { return _p_switch_devices[id < _max_switch_devices ? id : 0].min_value; };
    const double GetSwitchMaxValue(uint32_t id) { return _p_switch_devices[id < _max_switch_devices ? id : 0].max_value; };
    const double GetSwitchStep(uint32_t id) { return _p_switch_devices[id < _max_switch_devices ? id : 0].step; };
    const bool GetCanAsync(uint32_t id) { return _p_switch_devices[id < _max_switch_devices ? id : 0].async_type == SwitchAsyncType_t::kAsyncType; };
    const bool GetStateChangeComplete(uint32_t id) { return _p_switch_devices[id < _max_switch_devices ? id : 0].state_change_complete; };
    const bool GetIsBool(uint32_t id) { return _p_switch_devices[id < _max_switch_devices ? id : 0].is_bool; };
    const uint32_t GetSetTimeStampMs(uint32_t id) { return _p_switch_devices[id < _max_switch_devices ? id : 0].set_time_stamp_ms; };

    // setters ... only for initialization
    void InitSwitchInitBySetup(uint32_t id, bool init_by_setup) { _p_switch_devices[id].init_by_setup = init_by_setup; };
    void InitSwitchCanWrite(uint32_t id, bool can_write) { _p_switch_devices[id].can_write = can_write; };
    void InitSwitchName(uint32_t id, const char *name) { strlcpy(_p_switch_devices[id].name, name, kSwitchNameSize); };
    void InitSwitchDescription(uint32_t id, const char *description) { strlcpy(_p_switch_devices[id].description, description, kSwitchDescriptionSize); };
    void InitSwitchValue(uint32_t id, double double_value) { _p_switch_devices[id].value = double_value; };
    void InitSwitchMinValue(uint32_t id, double min_value) { _p_switch_devices[id].min_value = min_value; };
    void InitSwitchMaxValue(uint32_t id, double max_value) { _p_switch_devices[id].max_value = max_value; };
    void InitSwitchStep(uint32_t id, double step) { _p_switch_devices[id].step = step; };
    void InitSwitchCanAsync(uint32_t id, SwitchAsyncType_t async_type) { _p_switch_devices[id].async_type = async_type; };
    void InitStateChangeComplete(uint32_t id, bool state_change_complete) { _p_switch_devices[id].state_change_complete = state_change_complete; };
    void InitSwitchIsBool(uint32_t id, bool is_bool) { _p_switch_devices[id].is_bool = is_bool; };

    // setter
    const bool SetSwitch(uint32_t id, bool bool_value);
    const bool SetSwitchValue(uint32_t id, double double_value);
    const bool SetSwitchName(uint32_t id, char *name);
    const bool SetStateChangeComplete(uint32_t id, bool state_change_complete);
    void SetTimeStampMs(uint32_t id, uint32_t set_time_stamp_ms) { _p_switch_devices[id].set_time_stamp_ms = set_time_stamp_ms;}

public:
};
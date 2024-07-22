/**************************************************************************************************
  Filename:       AlpacaConfig.h
  Revised:        $Date: 2024-01-25$
  Revision:       $Revision: 02 $

  Description:    Config for Alpac Lib

  Copyright 2024 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#pragma once
#include <Arduino.h>

// Library version see also library.json/version
const char esp32_alpaca_device_library_version[] = "1.0.3";

// ALPACA Server 
#define ALPACA_MAX_CLIENTS  8
#define ALPACA_MAX_DEVICES 4
#define ALPACA_UDP_PORT 32227
#define ALPACA_TCP_PORT 80
#define ALPACA_CLIENT_CONNECTION_TIMEOUT_SEC 120

#define ALPACA_ENABLE_OTA_UPDATE


// ALPACA Management Interface - Description Request
#define ALPACA_INTERFACE_VERSION "[1]"             // /management/apiversions Value: Supported Alpaca API versions
#define ALPACA_MNG_SERVER_NAME "ALPACA-ESP32-DEMO" // only for init; managed by config
#define ALPACA_MNG_MANUFACTURE "DIY by @BigPet"
#define ALPACA_MNG_MANUFACTURE_VERSION "v1.0"
#define ALPACA_MNG_LOCATION "Germany/Bavaria"

// AscomDriver Common Properties:
// - Description: A description of the device, such as manufacturer and model number. Any ASCII characters may be used.
// - DriverInfo: Descriptive and version information about this ASCOM driver
// - DriverVersion: A string containing only the major and minor version of the driver
// - InterfaceVersion: The interface version number that this device supports
// - Name: The short name of the driver, for display purposes
// DeviceType - as defined ASCOM


// =======================================================================================================
// CoverCalibrator - Comon Properties
#define ALPACA_COVER_CALIBRATOR_DESCRIPTION "Alpaca CoverCalibrator Template"                      // init value; managed by config
#define ALPACA_COVER_CALIBRATOR_DRIVER_INFO "ESP32 CoverCalibrator driver by BigPet" // init value; managed by config
#define ALPACA_COVER_CALIBRATOR_DRIVER_VERSION "0.1"                                 //
#define ALPACA_COVER_CALIBRATOR_INTERFACE_VERSION 1                                  // don't change
#define ALPACA_COVER_CALIBRATOR_NAME "not used"                                      // init with <deviceType>-<deviceNumber>; managed by config
#define ALPACA_COVER_CALIBRATOR_DEVICE_TYPE "covercalibrator"                        // don't change

// // CoverCalibrator - Optional Methods
#define ALPACA_COVER_CALIBRATOR_PUT_ACTION_IMPLEMENTED
#define ALPACA_COVER_CALIBRATOR_PUT_COMMAND_BLIND_IMPLEMENTED
#define ALPACA_COVER_CALIBRATOR_PUT_COMMAND_BOOL_IMPLEMENTED
#define ALPACA_COVER_CALIBRATOR_PUT_COMMAND_STRING_IMPLEMENTED

// CoverCalibrator - Specific Properties
#define ALPACA_COVER_CALIBRATOR_MAX_BRIGHTNESS 1023 // init; managed by setup


// =======================================================================================================
// Switch - Comon Properties
#define ALPACA_SWITCH_DESCRIPTION "Alpaca Switch Template"                      // init value; managed by config
#define ALPACA_SWITCH_DRIVER_INFO "ESP32 Switch driver by BigPet" // init value; managed by config
#define ALPACA_SWITCH_DRIVER_VERSION "0.1"                        //
#define ALPACA_SWITCH_INTERFACE_VERSION 1                         // don't change
#define ALPACA_SWITCH_NAME "not used"                             // init with <deviceType>-<deviceNumber>; managed by config
#define ALPACA_SWITCH_DEVICE_TYPE "switch"                        // don't change

// Switch - Specific Properties
// empty


// =======================================================================================================
// ObservingConditions - Comon Properties
#define ALPACA_OBSERVING_CONDITIONS_DESCRIPTION "Alpaca ObservingConditions Template"                      // init value; managed by config
#define ALPACA_OBSERVING_CONDITIONS_DRIVER_INFO "ESP32 ObservingConditions driver by BigPet" // init value; managed by config
#define ALPACA_OBSERVING_CONDITIONS_DRIVER_VERSION "0.1"                                     //
#define ALPACA_OBSERVING_CONDITIONS_INTERFACE_VERSION 1                                      // don't change
#define ALPACA_OBSERVING_CONDITIONS_NAME "not used"                                          // init with <deviceType>-<deviceNumber>; managed by config
#define ALPACA_OBSERVING_CONDITIONS_DEVICE_TYPE "observingconditions"                        // don't change



// include user config 
#include "UserConfig.h"

const uint32_t kAlpacaMaxClients = ALPACA_MAX_CLIENTS; // TODO
const uint32_t kAlpacaMaxDevices = ALPACA_MAX_DEVICES;
const uint32_t kAlpacaUdpPort = ALPACA_UDP_PORT;
const uint32_t kAlpacaTcpPort = ALPACA_TCP_PORT;
const uint32_t kAlpacaClientConnectionTimeoutMs = ALPACA_CLIENT_CONNECTION_TIMEOUT_SEC * 1000;
# ASCOM Alpaca Server for ESP32 based devices

This is an ASCOM Alpaca Server implementation for ESP32. 
The following devices are supported:
- Cover Calibrator V1
- Observing Conditions V1
- Switch  V2
- Focuser V3 

This library is more or less a new implementation. But it's primary based on the great ideas and work of:
- https://github.com/agnunez/AlpacaServerESP32.git 
- https://github.com/elenhinan/ESPAscomAlpacaServer

<br>

## Motivation
My primary motivation was: 
<br>
- To have a state of the art compact and robust platform for the rework of my ESP32 based devices with propritary network protocols and .NET based ASCOM drivers.
- Learn and understand [ASCOM Alpaca API](https://ascom-standards.org/api/#/)
- Work with embedded systems and web technologies
<br><br>

## Features
- Easy to use platform for Alpaca Self Contained Device & Driver
- Templates for CoverCalibrator, Switch, ObservingConditions and Focuser
- Support ASCOM auto discovery
- Optimized for ESP32 (ESP286 not supported!)
- Improved robustness, logging, error handling and debugging
- Device service counter for diagnosis
- Published as library
- Several and different ASCOM devices on one ESP32 possible
- Several client connections per device supported
- Pass ASCOM Conform Universal validation with zero errors and zerro issues (ConformU 3.0.0)
- Addapted for ArduinoJason V7 
- Test with N.I.N.A 3.1 HF2 - with ASCOM Alpaca Discovery (https://nighttime-imaging.eu/)
- Suported ASCOM devices:
    - CoverCalibrator, ObservingConditions, Switch, Focuser
- Example Applications
    - CoverCalibrator.h, CoverCalibrator.cpp
    - ObservingConditions.h, ObservingConditions.cpp
    - Switch.h, Switch.cpp
    - Focuser.h, Focuser.cpp
- Support of optional ASCOM Methods: action, commandblind, commandbool, commandstring
    - See declaration in e.c. Switch.h. Customize methode if needed.
    ```
        // optional Alpaca service: to be implemented if needed
        const bool _putAction(const char *const action, const char *const parameters, char *string_response, size_t string_response_size) { return false; }
        const bool _putCommandBlind(const char *const command, const char *const raw, bool &bool_response) { return false; };
        const bool _putCommandBool(const char *const command, const char *const raw, bool &bool_response) { return false; };
        const bool _putCommandString(const char *const command_str, const char *const raw, char *string_response, size_t string_response_size) { return false; };
    ```
    - Don't forget to add _addAction("myaction") in e.c. Switch.cpp
- Serial and network based logging using SLog: "https://github.com/npeter/SLog"
- Manage persistant device data
    - stored with Setup/button "Save" in LittleFS:/settins.json
    - read (only) at device boot

- Configure persistant data via dynamicaly created webpages (based on the ideas from (https://github.com/elenhinan/ESPAscomAlpacaServer))
<br>
<br>
<img src="AlpacaAscomDriverSetup.png" width="600">
<br>
    - Buttons:
        - Refresh: Refresh web page with current device data
        - Update: Write web page data to device
        - Save: Store device data to LittleFS:/settings.json
        - Reset: Reset device after 10 seconds
    - Tabs:
        - Server: Server settings
        - Dynamic created device specific settings tabs

## Remarks
- For this project "platformio" (https://platformio.org/) is used as development system.
<br><br>
### Example platformio.ini


    [env]
    platform = espressif32
    framework = arduino
    monitor_speed = 115200
    board_build.filesystem = littlefs

    lib_deps = https://github.com/npeter/ESP32AlpacaDevices2

    [env:wemos_d1_mini32]
    board = wemos_d1_mini32
    build_flags=
	    -D ELEGANTOTA_USE_ASYNC_WEBSERVER=1
        -D WEMOS_D1_MINI32

    [env:lolin_s2_mini]
    board = lolin_s2_mini
    monitor_port = COM3
    upload_speed = 921600
    build_flags=
	    -D ELEGANTOTA_USE_ASYNC_WEBSERVER=1
        -D ARDUINO_USB_CDC_ON_BOOT=1
        -D LOLIN_S2_MINI


<br><br>
### ESP32AlpacaDevices library dependencies: (see library.json)
    ...
    "dependencies": [
        {
            "owner": "mathieucarbou",
            "name": "ESPAsyncWebServer",
            "version": "^3.3.11",
            "platforms": [
                "espressif8266",
                "espressif32"
            ]
        },
        {
            "owner": "bblanchon",
            "name": "ArduinoJson",
            "version": "^7.0.2"
        },
        {
            "owner": "ayushsharma82",
            "name": "ElegantOTA",
            "version": "^3.1.6"
        },
        {
            "name": "SLog",
            "version": "https://github.com/npeter/SLog"
        }
    ],
    ...

<br><br>
### Webpage data
- Webpage data has to be stored in the LittleFS - Filesystem of the ESP32
    - platformio/Build Filesystem image

        **VSCode example output:**<br>
        Building FS image from 'data' directory to .pio\build\wemos_d1_mini32\littlefs.bin
        /www/css/bootstrap.min.css.gz
        /www/css/jquery-ui.min.css.gz
        /www/css/theme.css
        /www/js/bootstrap.min.js.gz
        /www/js/jquery-ui.min.js.gz
        /www/js/jquery.min.js.gz
        /www/js/jsonFormer.jquery.js
        /www/setup.html


    - platformio/Upload Filesystem Image<br>

        **VSCode example output:**<br>
        Building in release mode<br>
        Building FS image from 'data' directory to .pio\build\wemos_d1_mini32\littlefs.bin<br>
        /www/css/bootstrap.min.css.gz        
        /www/css/jquery-ui.min.css.gz<br>
        /www/css/theme.css<br>
        /www/js/bootstrap.min.js.gz<br>
        /www/js/jquery-ui.min.js.gz<br>
        /www/js/jquery.min.js.gz<br>
        /www/js/jsonFormer.jquery.js<br>
        /www/setup.html<br>
        Looking for upload port...<br>
        Auto-detected: COM5<br>
        Uploading .pio\build\wemos_d1_mini32\littlefs.bin<br>
        esptool.py v4.7.5<br>
        Serial port COM5<br>
        Connecting.......<br>
        Chip is ESP32-D0WDQ6 (revision v1.0)<br>
        Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None<br>
        Crystal is 40MHz<br>
        MAC: c8:c9:a3:cb:7c:d4<br>
        Uploading stub...<br>
        Running stub...<br>
        Stub running...<br>
        Changing baud rate to 460800<br>
        Changed.<br>
        Configuring flash size...<br>
        Auto-detected Flash size: 4MB<br>
        Flash will be erased from 0x00290000 to 0x003effff...<br>
        Compressed 1441792 bytes to 152944...<br>
        Writing at 0x00290000... (10 %)<br>
        Writing at 0x002e7915... (20 %)<br>
        Writing at 0x002ebc14... (30 %)<br>
        Writing at 0x002f2968... (40 %)<br>
        Writing at 0x002f6f46... (50 %)<br>
        Writing at 0x002faf41... (60 %)<br>
        Writing at 0x002fef3c... (70 %)<br>
        Writing at 0x00302f37... (80 %)<br>
        Writing at 0x00307639... (90 %)<br>
        Writing at 0x0030b5f2... (100 %)<br>
        Wrote 1441792 bytes (152944 compressed) at 0x00290000 in 9.5 seconds (effective 1214.9 kbit/s)...<br>
        Hash of data verified.<br>
        Leaving...<br>
        Hard resetting via RTS pin...<br>
 

<br><br>

## Usage
- Let your class inherit the relevant AscomDevice-derived class (e.c. AscomSwitch, AscomCoverCalibrator, AscomObservingConditions, AscomFocuser)
- Implement all pure virtual functions
- Implement all your device/hardware specific functions
- Adapt AlpacaReadJson/AlpacaWriteJson if needed
- Implement main.cpp/setup() and loop() (see example below)
- See also demo application  .\Examples\*

<br><br>
<img src="Design.png" width="600">
<br>ESP32AlpacaDevices class diagram

<br><br>
<img src="AlpacaDiscoveryMap.png" width="600">
<br>Demo Application: Alpaca Discovery Map generated with **Conform Universal **
<br><br>

## Example main.cpp (see ./Examples/*.** for more details)
The following code block shows the prinziple setup and loop 

    #include <SLog.h>
    #include <AlpacaDebug.h>
    #include <AlpacaServer.h>
    #include <CoverCalibrator.h>
    #include <Switch.h>
    ...

    // Create CoverCalibrator, SwitchDevice and AlpacaServer
    CoverCalibrator coverCalibrator;
    Switch switchDevice;  
    AlpacaServer alpaca_server("your server name", "your manufacture", "your version", "your location");
    ...
    void setup()
    {
        // Setup logging 
        g_Slog.Begin(Serial, 115200);
        SLOG_INFO_PRINTF("BigPet ESP32ALPACADeviceDemo started ...\n");

        // Setup WiFi
        ... 

        // Setup ESP32 Alpaca Server and Devices
        // 1. Init AlpacaServer
        alpaca_server.Begin();

        // 2. Init and add devices
        coverCalibrator.Begin();
        switch.Begin();
        alpaca_server.AddDevice(&coverCalibrator);
        alpaca_server.AddDevice(&switchDevice);

        // 3. Finalize AlpacaServer 
        alpaca_server.RegisterCallbacks();
        alpaca_server.LoadSettings();
        ...

        g_Slog.SetLvlMsk(SLOG_WARNING);
        g_Slog.SetEnableSerial(true);
        ...
    }

    void loop()
    {
        ...
        alpaca_server.Loop();
        coverCalibrator.Loop();
        switchDevice.Loop();
        ...
        delay(50);
    }



## Useful Links
- [ASCOM Alpaca API](https://ascom-standards.org/api/#/)
- [ASCOM Platform Developer Help](https://ascom-standards.org/Help/Developer/html/7d9253c2-fdfd-4c0d-8225-a96bddb49731.htm)
- [ASCOM Documentation Center](https://ascom-standards.org/Documentation/Index.htm#dev)
- [platformio](https://docs.platformio.org/en/latest/what-is-platformio.html)
- [ArduinoJson](https://arduinojson.org/)
<br><br>

    


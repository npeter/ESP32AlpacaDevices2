# Changelog ESP32AlpacaDevices

## 1.0.0 2024-07-03 created

## 1.0.2 2024-07-03 
- 1.0.2.a 2024-07-03 DBG_REQ enabled
- 1.0.2.b 2024-07-16 Dependency AsyncElegantOTA moved to npeter
- 1.0.2.c 2024-07-16 Service counter added; Cleared by first connected client
- 1.0.2.d 2024-07-16 refactoring _supported_action
- 1.0.2.e 2024-07-16 AlpacaGetSupportedActions - json resp kAsPlainStringValue
- 1.0.2.f 2024-07-20 Connection monitoring debugging

## 2.0.0 2024-07-23 
- Refactoring 
- Update examples 
- Test with:
    - "Conform Universal 3.0.0"
    - NINA 3.1 HF1

## 2.1.0 2024-07-26
- Alpaca device type Focuser added
- Demo application extended 
- Test with:
    - **Conform Universal 3.0.0 (Build 29642.2251685)**
    - **N.I.N.A 3.1 HF1**

## 2.1.1 2024-07-26 in work
1. ALPACA optional focuser services implemented
2. SetResetRequestAdded
3. Ajax test 
4. createCallBackUrl() added
5. Ajax test removed 
6. Test and correct optional Alpaca Services with focuser
7. Support focuser optional URL's
8. Support services without connection
9. /www/setup.html moved to /setup.html
10. /www/* moved to /
11. Include fw-version in AlpacaGetDriverVersion service
12. Correct device setup link
13. New Exception: ThrowRspStatusParameterInvalidValue(... bool) added
14. New Exception: ThrowRspStatusParameterInvalidValue(... double) added
15. SPIFFS replaced by LittleFS
16. me-no-dev/ESPAsyncWebServer replaced by mathieucarbou/ESPAsyncWebServer byMathieu Carbou
17. AsyncElegantOTA replaced by ElegantOTA
18. library.json/dependencies reworked
19. esp_read_mac() replaced by esp_wifi_get_mac()
20. Add -fexceptions to build_flags
21. Test with current N.I.N.A version TODO
22. Remove parameter devicemethode in CreateCallback()
23. setup.html moved to /www/setup.html
24. Log libversion
25. Examples/*.* adapted

## 3.0.0 2024-10-28 Alpha
- Based on "Arduino core 3.0.x"  
- SPIFF has been replaced by LittleFS
    - execute "Build Filesystem image" and "Upload Filesystem"
- Changelog see 2.1.1

## 3.1.0 2024-11-06 Release
1. AlpacaSwitch.RegisterCallback() _alpacaPut... HTTP_GET replaced by HTTP_PUT
2. AlpacaServer.Begin initialize mac_adr wit {0}
3. AlpacaServer replace std::stoi, std::stoul, std::stof, std::tod by sscanf to avoid exceptions
4. Exception with Conform Universal ( Alpaca REQ (192.168.178.30) /api/v1/switch/0/connected - ClientID=<> - ClientTransactionID=<67890>) -> try catch removed
5. Replace try and catch in all source files
6. Disable try/catch support: Remove build_flags "fexceptions" in platformio.ini
7. Remove delay() AlpacaDebug.h/DBG_REQ 
8. Log library version

## 3.2.0 2024-11-17 Release 
1. Version 3.1.1 in work
2. Rework serveStatic to avoid unexpected filsystem operations
3. AlpacaSwitch.GetValue() wrong return type changed from const double to const bool
4. AlpacaSwitch._alpacaPutSetSwitchValue() error message improved
5. FOCUSER_PUT_ACTION, FOCUSER_PUT_COMMAND_BOOL, FOCUSER_PUT_COMMAND_STRING implemented
6. Dynamic activation of log settings  
7. Refactoring registration of urls and handlers for server and device 
8. Add request.method to DBG_REQ output
9. AlpacaServer Typo corrected
10. Test with "Conform Universal 4.0.0"
11. Test with N.I.N.A. 3.1 HF2 
12. README.md improved
13. Examples adapted

## 3.3.0 2024-12-18 Release
1. V3.2.1 created
2. Example/Switch.cpp ReadJson/WriteJson spaces in section names removed
3. Example/main.cpp delays adapted
4. AlpacaFocuser issue with ConformUniversal Test Case: "Bad ClientTransactionID casing" solved
5. AlpacaFocuser/AlpacaPutCommandString "throw/catch" replaced by "goto mycatch"
6. Data/favicon.ico added
7. docs/README.md improved

### Open topics 
TODO concept for usr settings





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

## 3.0.1 2024-10-28 in work
1. Exception with Conform Universal ( Alpaca REQ (192.168.178.30) /api/v1/switch/0/connected - ClientID=<> - ClientTransactionID=<67890>)
2. Remove delay() AlpacaDebug.h/DBG_REQ

### Open topics 

2. Version output in log
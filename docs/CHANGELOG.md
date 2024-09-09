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

## 2.1.1 2024-07-26 Adaption for MoFo4
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

## 2.2.0 2024-09-09 Baseline after Adaption for MoFo4 1.0.0

## 2.2.1 2024-09-09 New workin branch created


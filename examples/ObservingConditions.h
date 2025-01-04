/**************************************************************************************************
  Description:    ASCOM Observing Conditions Device Template
  Copyright 2024-2025 peter_n@gmx.de. All rights reserved.
**************************************************************************************************/
#pragma once
#include "AlpacaObservingConditions.h"

class ObservingConditions : public AlpacaObservingConditions
{
public:
  ObservingConditions();
  void Begin();
  void Loop();
  
private:
  uint32_t _update_time_ms = 0;
  uint32_t _refresh_time_ms = 1000;

  // virtual methods
  void _putRefreshRequest() { _update_time_ms = 0; };
  const bool _putAveragePeriodRequest(double average_period);

  // optional Alpaca service: to be implemented if needed
  const bool _putAction(const char *const action, const char *const parameters, char *string_response, size_t string_response_size) { return false; }
  const bool _putCommandBlind(const char *const command, const char *const raw, bool &bool_response) { return false; };
  const bool _putCommandBool(const char *const command, const char *const raw, bool &bool_response) { return false; };
  const bool _putCommandString(const char *const command_str, const char *const raw, char *string_response, size_t string_response_size) { return false; };

  void _refresh();

  void AlpacaReadJson(JsonObject &root);
  void AlpacaWriteJson(JsonObject &root);
};
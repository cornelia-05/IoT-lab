#include <Arduino.h>
#include <stdio.h>
#include "config.h"
#include "relay.h"
#include "system_state.h"
void initRelay() { digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? HIGH : LOW); pinMode(RELAY_PIN, OUTPUT); relayState = false; }
void setRelay(bool state) { relayState = state; digitalWrite(RELAY_PIN, RELAY_ACTIVE_LOW ? (state ? LOW : HIGH) : (state ? HIGH : LOW)); printf("[RELEU] %s\n", state ? "ON" : "OFF"); }
bool getRelayState() { return relayState; }

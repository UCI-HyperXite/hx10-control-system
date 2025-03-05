#pragma once

typedef bool(*FSM_EVENT)(void);
typedef void(*FSM_ACTION)(void);

typedef enum {
    INIT, LOAD, PRECHARGE, START, STOP, FAULT, HALT
} FSM_STATE;


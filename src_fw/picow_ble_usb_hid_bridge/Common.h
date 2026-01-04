// Copyright © 2025 Shiomachi Software. All rights reserved.
#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/flash.h"
#include "pico/cyw43_arch.h"
#include "Type.h"

// [Definitions]
// Maximum size of the HID queue
#define CMN_QUE_DATA_MAX_HID_RPT 32

// Maximum size of the HID report data
#define CMN_HID_RPT_DATA_SIZE 512

// [Enumerations]
// Queue types
typedef enum _E_CMN_QUE_KIND { 
    CMN_QUE_KIND_HID_RPT = 0, // HID Report Queue
    CMN_QUE_KIND_NUM          // Number of queue types
} E_CMN_QUE_KIND;

#pragma pack(1)

// [Structures]
// Queue control structure
typedef struct _ST_QUE {
    ULONG head; // Head index (Read position)
    ULONG tail; // Tail index (Write position)
    ULONG max;  // Maximum capacity of the queue
    PVOID pBuf; // Pointer to the data buffer
} ST_QUE;

// HID Report structure
typedef struct _ST_HID_RPT {
    uint8_t report_id;
    uint8_t report[CMN_HID_RPT_DATA_SIZE];
    uint16_t report_len;
} ST_HID_RPT;

#pragma pack()

// [Function Prototypes]
bool CMN_Enqueue(ULONG iQue, PVOID pData);
bool CMN_Dequeue(ULONG iQue, PVOID pData);
bool CMN_PeekQueue(ULONG iQue, PVOID pData);
void CMN_AdvanceQueue(ULONG iQue);
void CMN_ClearQueue(ULONG iQue);
void CMN_EntrySpinLock(void);
void CMN_ExitSpinLock(void);
void CMN_Init(void);

#endif

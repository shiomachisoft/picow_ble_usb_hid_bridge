// Copyright © 2025 Shiomachi Software. All rights reserved.
#include "Common.h" 

// [File Scope Variables]
static ST_QUE f_astQue[CMN_QUE_KIND_NUM] = {0}; // Array of queue control structures
static ST_HID_RPT f_astQueData_hid[CMN_QUE_DATA_MAX_HID_RPT] = {0}; // Data buffer for the HID queue
static critical_section_t f_stSpinLock = {0}; // Spinlock structure

// Enqueues data into the specified queue
bool CMN_Enqueue(ULONG iQue, PVOID pData) 
{
    bool bRet = false;
    ST_QUE *pstQue = &f_astQue[iQue];
    ST_HID_RPT *pstHidRpt;

    CMN_EntrySpinLock(); // Acquire spinlock

    if ((pstQue->head == (pstQue->tail + 1) % pstQue->max)) { 
        // Queue is full
    }
    else {
        // Perform queuing
        switch (iQue) {
        case CMN_QUE_KIND_HID_RPT: // HID Report Queue
            pstHidRpt = (ST_HID_RPT *)pstQue->pBuf;
            memcpy(&pstHidRpt[pstQue->tail], pData, sizeof(ST_HID_RPT));
            break; 
        default:
            // Should not be reached
            break;              
        }   
        pstQue->tail = (pstQue->tail + 1) % pstQue->max;
    
        bRet = true;
    }
    
    CMN_ExitSpinLock(); // Release spinlock

    return bRet;
}

// Dequeues data from the specified queue
bool CMN_Dequeue(ULONG iQue, PVOID pData)
{
    bool bRet = false;
    ST_QUE *pstQue = &f_astQue[iQue];   
    ST_HID_RPT *pstHidRpt;   
    
    CMN_EntrySpinLock(); // Acquire spinlock

    if (pstQue->head == pstQue->tail) {
        // Queue is empty

        // Do nothing
    }
    else {
        // Perform dequeuing
        switch (iQue) {
        case CMN_QUE_KIND_HID_RPT:  // HID Report Queue
            pstHidRpt = (ST_HID_RPT *)pstQue->pBuf;
            memcpy(pData, &pstHidRpt[pstQue->head], sizeof(ST_HID_RPT));
            break; 
        default:
            // Should not be reached
            break;              
        }   
        pstQue->head = (pstQue->head + 1) % pstQue->max;
        bRet = true;
    }

    CMN_ExitSpinLock(); // Release spinlock

    return bRet;
}

// Peeks at the data from the specified queue without removing it
bool CMN_PeekQueue(ULONG iQue, PVOID pData)
{
    bool bRet = false;
    ST_QUE *pstQue = &f_astQue[iQue];   
    ST_HID_RPT *pstHidRpt;   
    
    CMN_EntrySpinLock(); // Acquire spinlock

    if (pstQue->head == pstQue->tail) {
        // Queue is empty

        // Do nothing
    }
    else {
        // Copy data
        switch (iQue) {
        case CMN_QUE_KIND_HID_RPT:  // HID Report Queue
            pstHidRpt = (ST_HID_RPT *)pstQue->pBuf;
            memcpy(pData, &pstHidRpt[pstQue->head], sizeof(ST_HID_RPT));
            break; 
        default:
            // Should not be reached
            break;              
        }   
        bRet = true;
    }

    CMN_ExitSpinLock(); // Release spinlock

    return bRet;
}

// Advances the queue's read pointer (head)
void CMN_AdvanceQueue(ULONG iQue)
{
    ST_QUE *pstQue = &f_astQue[iQue];

    CMN_EntrySpinLock(); // Acquire spinlock

    if (pstQue->head == pstQue->tail) {
        // Queue is empty

        // Do nothing
    }
    else {
        // Advance the head pointer
        pstQue->head = (pstQue->head + 1) % pstQue->max;
    }

    CMN_ExitSpinLock(); // Release spinlock
}

// Clears all data from the specified queue.
void CMN_ClearQueue(ULONG iQue)
{
    ST_QUE *pstQue = &f_astQue[iQue];

    CMN_EntrySpinLock(); // Acquire spinlock

    // Reset head and tail pointers to empty the queue
    pstQue->head = 0;
    pstQue->tail = 0;

    CMN_ExitSpinLock(); // Release spinlock
}

// Enters a critical section (spinlock).
void CMN_EntrySpinLock(void)
{
    critical_section_enter_blocking(&f_stSpinLock);
}

// Exits the critical section (spinlock)
void CMN_ExitSpinLock(void)
{
    critical_section_exit(&f_stSpinLock);
}

// Initializes the common library
void CMN_Init(void)
{
    // [Initialize variables]
    critical_section_init(&f_stSpinLock);
    f_astQue[CMN_QUE_KIND_HID_RPT].pBuf = (PVOID)f_astQueData_hid;
    f_astQue[CMN_QUE_KIND_HID_RPT].max  = CMN_QUE_DATA_MAX_HID_RPT;
}
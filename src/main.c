/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "bsp/board_api.h"
#include "tusb.h"
// @@add
// =====>
#include "Common.h"
// <=====

//--------------------------------------------------------------------+
// MACROS
//--------------------------------------------------------------------+
// @@add
// =====>
#define USB_REINIT_STABILIZATION_DELAY 100 // ms
#define LED_BLINKING_INTERVAL 200 // ms
// <=====
//--------------------------------------------------------------------+
// GLOBAL VARIABLES
//--------------------------------------------------------------------+
// @@add
// =====>
volatile bool g_usb_reinit_request = false; // Flag to request USB re-initialization when BLE HID connection is established
// <=====

//--------------------------------------------------------------------+
// FUNCTION PROTOTYPES
//--------------------------------------------------------------------+
// @@add
// =====>
void usb_dev_main(void);
void hid_task(void);
void led_blinking_task(void);
bool send_hid_report(void);

extern bool is_ble_app_state_ready(void);
extern void ble_host_main(void);
// <=====

/*------------- MAIN -------------*/
int main(void)
{
    board_init();  

    // init device stack on configured roothub port
    tud_init(BOARD_TUD_RHPORT);

    if (board_init_after_tusb) {
        board_init_after_tusb();
    }      
    
    // @@chg
    // =====>
    stdio_init_all();
    CMN_Init(); 

    // Initialize to lock out CPU Core 0 when btstack writes to flash memory on CPU Core 1
    flash_safe_execute_core_init();

    multicore_launch_core1(ble_host_main);

    usb_dev_main();

    return 0;
    // <=====
}

// @@add
// =====>
//--------------------------------------------------------------------+
// Main loop for the USB device (runs on Core0).
//--------------------------------------------------------------------+
// This function loops indefinitely, handling USB events, LED blinking, and HID tasks.
// It also handles USB re-initialization requests from Core1.
void usb_dev_main(void)
{    
    while (1) 
    {
        // Check for USB re-initialization request from Core1 (BLE host)
        if (g_usb_reinit_request) {
            g_usb_reinit_request = false; 
            if (tud_mounted()) {
                tud_disconnect(); // Disconnect the USB device
                board_delay(USB_REINIT_STABILIZATION_DELAY); // Wait a bit for stabilization
            }
            // Clear any pending HID reports from the queue before reconnecting.
            CMN_ClearQueue(CMN_QUE_KIND_HID_RPT);
            tud_connect();
        }

        tud_task();          // Run TinyUSB device task
        led_blinking_task(); // Run LED blinking task
        hid_task();          // Run HID report sending task
    }
}
// <=====

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
    (void) remote_wakeup_en;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
}

//--------------------------------------------------------------------+
// USB HID
//--------------------------------------------------------------------+

// @@chg
// =====>
// Dequeue and send one HID report from the queue to the USB host.
// return true if a report was successfully sent, false otherwise.
bool send_hid_report(void)
{
    static ST_HID_RPT stHidRpt; // Change local variable to static to use static memory (data area) instead of stack, preventing stack overflow.
    bool bRet = false;

    // Peek at the next report in the queue without removing it yet
    if (CMN_PeekQueue(CMN_QUE_KIND_HID_RPT, &stHidRpt)) {
        // If the host is suspended, wake it up and exit.
        // The report will be sent on a subsequent call after the host resumes.
        if ( tud_suspended()) {
            tud_remote_wakeup();
            return bRet;
        }                 
        // If the HID interface is ready, try to send the report
        if (tud_hid_ready()) {      
            // Try to send the report
            if (tud_hid_report(0, stHidRpt.report, stHidRpt.report_len)) {
                // If sent successfully, remove the report from the queue
                CMN_AdvanceQueue(CMN_QUE_KIND_HID_RPT);
                bRet = true;
            }  
        }
    }
 
    return bRet;
}
// <=====

//--------------------------------------------------------------------+
// HID TASK
//--------------------------------------------------------------------+
void hid_task(void)
{
    // @@chg
    // =====>
    // Dequeue and send one HID report.
    (void)send_hid_report();
    // <=====
}

// Invoked when sent REPORT successfully to host
// Application can use this to send the next report
// Note: For composite reports, report[0] is report ID
void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint16_t len)
{
    (void) instance;
    (void) len;
    // @@chg
    // =====>
    (void) report;
    // <=====
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    // TODO not Implemented
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    (void) instance;
    // @@chg
    // =====>
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) bufsize;
    // <=====
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
    static uint32_t start_ms = 0;
    static bool led_state = false;
    // @@chg
    // =====>
    const uint32_t blink_interval = LED_BLINKING_INTERVAL;

    if (is_ble_app_state_ready()) {
        // Turn on LED when in READY state
        if (!led_state) {
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
            led_state = true;
        }
    } else {
        // Blink every 200ms when not in READY state
        if ( board_millis() - start_ms < blink_interval) return; // not enough time
        start_ms += blink_interval;

        led_state = !led_state;
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);
    }
    // <=====
}
/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "btstack_event.h"
#include "hal_led.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "btstack.h"
// @@add
// =====>
#include "Common.h"
// <=====

// Start the btstack example
int btstack_main(int argc, const char * argv[]);

static btstack_packet_callback_registration_t hci_event_callback_registration;

// @@add
// =====>
// Set once cyw43_arch_init() has returned on Core 1. Core 0 drives the status
// LED through the same wireless chip and must not touch it before then.
volatile bool g_cyw43_initialized = false;
// <=====

static void packet_handler(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size){
    UNUSED(size);
    UNUSED(channel);
    bd_addr_t local_addr;
    if (packet_type != HCI_EVENT_PACKET) return;
    switch(hci_event_packet_get_type(packet)){
        case BTSTACK_EVENT_STATE:
            if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING) return;
            gap_local_bd_addr(local_addr);
            BLE_LOG("BTstack up and running on %s.\n", bd_addr_to_str(local_addr));
            break;
        default:
            break;
    }
}

int ble_bridge_bt_example_init(void) {
    // initialize CYW43 driver architecture (will enable BT if/because CYW43_ENABLE_BLUETOOTH == 1)
    if (cyw43_arch_init()) {
        BLE_LOG("failed to initialise cyw43_arch\n");
        return -1;
    }

    // @@add
    // =====>
    // The wireless chip is up; Core 0 may now drive the status LED.
    g_cyw43_initialized = true;
    // <=====

    // inform about BTstack state
    hci_event_callback_registration.callback = &packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    return 0;
}

void ble_bridge_bt_example_main(void) {

    btstack_main(0, NULL);
}

/*
 * uC Interface - A GUI for Programming & Interfacing with Microcontrollers
 * Copyright (C) 2018  Mitchell Oleson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef GENERAL_COMMS_H
#define GENERAL_COMMS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

static const uint8_t packet_retries = 2;

// Major Keys enum
typedef enum {
    // Error and reset
    MAJOR_KEY_ERROR = 0,
    MAJOR_KEY_RESET,

    // Action confirmations
    MAJOR_KEY_ACK,

    // Update Config Settings
    MAJOR_KEY_CONFIG_UPDATE,

    // GUI Types (Major Keys)
    GUI_TYPE_ERROR,
    GUI_TYPE_WELCOME,
    GUI_TYPE_IO,
    GUI_TYPE_DATA_TRANSMIT,
    GUI_TYPE_PROGRAMMER
} MAJOR_KEYS;

/* Packet #1 (p1) generic key positions */
typedef enum {
    p1_major_key_loc = 0,
    p1_num_p2_bytes_loc,
    p1_crc_loc
} P1_Major_Settings;
static const uint8_t num_p1_bytes = p1_crc_loc + 1;

/* Packet #2 (p2) key positions enum */
typedef enum {
    p2_sub_key_loc = 0,
    p2_data_start_loc
} P2_Sub_Settings;
extern uint8_t num_p2_bytes;

#ifdef __cplusplus
}
#endif

#endif // GENERAL_COMMS_H

/* Copyright 2023 @ Muhan Li
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

#include "os_detection.h"
/*
detected_host_os() => enum {
    OS_UNSURE,
    OS_LINUX,
    OS_WINDOWS,
    OS_MACOS,
    OS_IOS,
} os_variant_t;
*/

#define INDICATOR_RED    0xFF, 0x00, 0x00
#define INDICATOR_YELLOW 0xFF, 0x66, 0x00
#define INDICATOR_GREEN  0x30, 0xD0, 0x10

#define INDICATOR_INDEX_POWER 0
#define INDICATOR_MACRO_POWER 13

#define TAP_MIN_ELAPSED 10

/** @file
 * Copyright (c) 2021-2025, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 * Copyright 2023 NXP
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include <inttypes.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "pal_common.h"

/* Regression test status reporting buffer */
uint8_t test_status_buffer[256]  = {0};

/* This stdc implementation doesn't support tests that involve resets of the
 * test process or the system, so we don't actually need non-volatile memory.
 * Just implement the "nvmem" as an array in memory.
 */

/* Using zero as NVMEM_BASE is a bit arbitrary - we don't actually need callers
 * to specify a base address but the nvmem function signatures have "base" params.
 * Zero is the value used in our pal_config.h file so that's what we should receive.
 */
#define NVMEM_BASE 0

#define NVMEM_SIZE (1024)
static uint8_t g_nvmem[NVMEM_SIZE];

/* The custom test list is a buffer in which all enabled test names are concatenated.
 * The test name template is <TEST_NAME_PREFIX><id><TEST_NAME_SUFFIX>, where <id>
 * is the test identifier.
 */
#define TEST_NAME_PREFIX "test_"
#define TEST_NAME_SUFFIX ";"

char *g_custom_test_list = NULL;

/**
    @brief    - Check that an nvmem access is within the bounds of the nvmem
    @param    - base    : Base address of nvmem (must be zero)
                offset  : Offset into nvmem
                size    : Number of bytes
    @return   - SUCCESS/FAILURE
**/
static int nvmem_check_bounds(addr_t base, uint32_t offset, int size)
{
    if (base != NVMEM_BASE)
        return PAL_STATUS_ERROR;
    if (offset > NVMEM_SIZE)
        return PAL_STATUS_ERROR;
    if (size < 0)
        return PAL_STATUS_ERROR;
    if (offset > (uint32_t)(INT_MAX - size))
        return PAL_STATUS_ERROR;
    if (offset + size > NVMEM_SIZE)
        return PAL_STATUS_ERROR;
    return PAL_STATUS_SUCCESS;
}

/**
    @brief    - Reads from given non-volatile address.
    @param    - offset  : Offset
                buffer  : Pointer to dest address
                size    : Number of bytes
    @return   - SUCCESS/FAILURE
**/
int pal_nvm_read(uint32_t offset, void *buffer, size_t size)
{
    if (nvmem_check_bounds((addr_t) PLATFORM_NVM_BASE, offset, size) != PAL_STATUS_SUCCESS)
    {
        return PAL_STATUS_ERROR;
    }
    memcpy(buffer, g_nvmem + offset, size);
    return PAL_STATUS_SUCCESS;
}

/**
    @brief    - Writes into given non-volatile address.
    @param    - offset  : Offset
                buffer  : Pointer to source address
                size    : Number of bytes
    @return   - SUCCESS/FAILURE
**/
int pal_nvm_write(uint32_t offset, void *buffer, size_t size)
{
    if (nvmem_check_bounds((addr_t) PLATFORM_NVM_BASE, offset, size) != PAL_STATUS_SUCCESS)
    {
        return PAL_STATUS_ERROR;
    }
    memcpy(g_nvmem + offset, buffer, size);
    return PAL_STATUS_SUCCESS;
}

/**
    @brief    - This function initializes the UART

    This implementation doesn't actually use a UART to print test output, we
    just send it to stdout. No init necessary.

    @param    - Void
    @return   - SUCCESS/FAILURE
**/
int pal_uart_init_ns(void)
{
    return PAL_STATUS_SUCCESS;
}

/**
    @brief    - This function parses the input string and writes bytes into UART TX FIFO

    This implementation doesn't actually use a UART to print test output, we
    just send it to stdout.

    @param    - str      : Input String
              - data     : Value for format specifier
    @return   - SUCCESS/FAILURE
**/
int pal_print_ns(const char *str, int32_t data)
{
    if (printf(str, data) < 0)
    {
        return PAL_STATUS_ERROR;
    }
    return PAL_STATUS_SUCCESS;
}

/**
    @brief           - Initializes a hardware watchdog timer

    This implementation doesn't support watchdogs so this is a noop.

    @param           - time_us         : Time in micro seconds
                     - timer_tick_us   : Number of ticks per micro second
    @return          - SUCCESS/FAILURE
**/
int pal_wd_timer_init_ns(uint32_t time_us, uint32_t timer_tick_us)
{
    (void)time_us;
    (void)timer_tick_us;
    return PAL_STATUS_SUCCESS;
}

/**
    @brief           - Enables a hardware watchdog timer

    This implementation doesn't support watchdogs so this is a noop.

    @param           - Void
    @return          - SUCCESS/FAILURE
**/
int pal_watchdog_enable(void)
{
    return PAL_STATUS_SUCCESS;
}

/**
    @brief           - Disables a hardware watchdog timer

    This implementation doesn't support watchdogs so this is a noop.

    @param           - Void
    @return          - SUCCESS/FAILURE
**/
int pal_watchdog_disable(void)
{
    return PAL_STATUS_SUCCESS;
}

/**
     @brief    - Terminates the simulation at the end of all tests completion.

     This implementation just calls exit.

     @param    - void
     @return   - void
**/
void pal_terminate_simulation(void)
{
    ;
}

/**
 *   @brief    - Resets the system.
 *   @param    - void
 *   @return   - SUCCESS/FAILURE
**/
int pal_system_reset(void)
{
    return PAL_STATUS_UNSUPPORTED_FUNC;
}

/**
 *   @brief    - Sets the custom test list buffer
 *   @param    - custom_test_list : Custom test list buffer
     @return   - void
**/
void pal_set_custom_test_list(char *custom_test_list)
{
    g_custom_test_list = custom_test_list;
}

/**
 *   @brief    - Tells if a test is enabled on platform
 *   @param    - test_id : Test ID
 *   @return   - TRUE/FALSE
**/
bool_t pal_is_test_enabled(test_id_t test_id)
{
    char test_id_str[16] = { 0 };

    if (!g_custom_test_list)
        return 1;

    if (sprintf(test_id_str, "%s%d%s", TEST_NAME_PREFIX, test_id, TEST_NAME_SUFFIX) <= 0)
        return 0;

    return strstr(g_custom_test_list, test_id_str)?1:0;
}

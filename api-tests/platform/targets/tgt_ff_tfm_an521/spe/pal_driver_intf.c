 /** @file
  * Copyright (c) 2019-2025, Arm Limited or its affiliates. All rights reserved.
  * SPDX-License-Identifier : Apache-2.0
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

#include "pal_interfaces_s.h"
#include "pal_uart.h"
#include "pal_nvmem.h"
#include "pal_wd_cmsdk.h"

/* Regression test status reporting buffer */
uint8_t test_status_buffer[256]  = {0};

/**
    @brief    - This function initializes the UART
    @param    - uart base addr
    @return   - void
**/
void pal_uart_init(uint32_t uart_base_addr)
{
    pal_uart_cmsdk_init(uart_base_addr);
}

/**
    @brief    - This function sends a character to the UART TX FIFO.
    @param    - c  :  Input character
**/
int pal_print(uint8_t c)
{
  pal_uart_cmsdk_putc(c);
  return PAL_STATUS_SUCCESS;
}

/**
    @brief    - Writes into given non-volatile address.
    @param    - base    : Base address of nvmem
                offset  : Offset
                buffer  : Pointer to source address
                size    : Number of bytes
    @return   - 1/0
**/
int pal_nvmem_write(addr_t base, uint32_t offset, void *buffer, int size)
{
    return nvmem_write(base, offset, buffer, size);
}

/**
    @brief    - Reads from given non-volatile address.
    @param    - base    : Base address of nvmem
                offset  : Offset
                buffer  : Pointer to source address
                size    : Number of bytes
    @return   - 1/0
**/
int pal_nvmem_read(addr_t base, uint32_t offset, void *buffer, int size)
{
    return nvmem_read(base, offset, buffer, size);
}


/**
    @brief           - Initializes a hardware watchdog timer
    @param           - base_addr       : Base address of the watchdog module
                     - time_us         : Time in micro seconds
                     - timer_tick_us   : Number of ticks per micro second
    @return          - SUCCESS/FAILURE
**/
int pal_wd_timer_init(addr_t base_addr, uint32_t time_us, uint32_t timer_tick_us)
{
    return pal_wd_cmsdk_init(base_addr, time_us, timer_tick_us);

}

/**
    @brief           - Enables a hardware watchdog timer
    @param           - base_addr       : Base address of the watchdog module
    @return          - SUCCESS/FAILURE
**/
int pal_wd_timer_enable(addr_t base_addr)
{
    return pal_wd_cmsdk_enable(base_addr);
}

/**
    @brief           - Disables a hardware watchdog timer
    @param           - base_addr       : Base address of the watchdog module
    @return          - SUCCESS/FAILURE
**/
int pal_wd_timer_disable(addr_t base_addr)
{
    return pal_wd_cmsdk_disable(base_addr);
}

/**
    @brief           - Checks whether hardware watchdog timer is enabled
    @param           - base_addr       : Base address of the watchdog module
    @return          - Enabled : 1, Disabled : 0
**/
int pal_wd_timer_is_enabled(addr_t base_addr)
{
    return pal_wd_cmsdk_is_enabled(base_addr);
}

/**
    @brief   - Trigger interrupt for irq signal assigned to driver partition
               before return to caller.
    @param   - void
    @return  - void
**/
void pal_generate_interrupt(void)
{
    pal_uart_cmsdk_generate_irq();
}

/**
    @brief   - Disable interrupt that was generated using pal_generate_interrupt API.
    @param   - void
    @return  - void
**/
void pal_disable_interrupt(void)
{
    pal_uart_cmsdk_disable_irq();
}

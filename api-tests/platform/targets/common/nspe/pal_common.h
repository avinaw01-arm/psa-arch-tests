/** @file
 * Copyright (c) 2020-2023, 2025, Arm Limited or its affiliates. All rights reserved.
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

#ifndef _PAL_COMMON_H_
#define _PAL_COMMON_H_

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>

#include "pal_config.h"
#include "pal_common_val_intf.h"

/* typedef's */
#ifndef TARGET_SPECIFIC_TYPES
typedef uint8_t             bool_t;
#define addr_t              undef_addr_t   //To avoid addr_t name collision
typedef uint32_t            addr_t;
typedef uint32_t            test_id_t;
typedef uint32_t            block_id_t;
typedef char                char8_t;
typedef uint32_t            cfg_id_t;
#endif

#define PAL_STATUS_UNSUPPORTED_FUNC      0xFF

typedef enum {
    PAL_STATUS_SUCCESS = 0x0,
    PAL_STATUS_ERROR   = 0x80
} pal_status_t;

typedef enum {
    NVMEM_READ             = 0x1,
    NVMEM_WRITE            = 0x2,
} nvmem_fn_type_t;

typedef struct {
    nvmem_fn_type_t nvmem_fn_type;
    addr_t base;
    uint32_t offset;
    int size;
} nvmem_param_t;

typedef enum {
    WD_INIT_SEQ         = 0x1,
    WD_ENABLE_SEQ       = 0x2,
    WD_DISABLE_SEQ      = 0x3,
    WD_STATUS_SEQ       = 0x4,
} wd_fn_type_t;

typedef enum {
    WD_LOW_TIMEOUT      = 0x1,
    WD_MEDIUM_TIMEOUT   = 0x2,
    WD_HIGH_TIMEOUT     = 0x3,
    WD_CRYPTO_TIMEOUT   = 0x4,
} wd_timeout_type_t;

typedef struct {
    wd_fn_type_t wd_fn_type;
    addr_t       wd_base_addr;
    uint32_t     wd_time_us;
    uint32_t     wd_timer_tick_us;
} wd_param_t;

typedef enum {
    UART_INIT             = 0x1,
    UART_PRINT            = 0x2,
} uart_fn_type_t;

/*
 * Redefining some of the client.h elements for compilation to go through
 * when PSA IPC APIs are not implemented.
 */
#ifndef IPC

#ifndef PSA_VERSION_NONE
#define PSA_VERSION_NONE            (0u)
#endif

#ifndef PSA_SUCCESS
typedef int32_t psa_status_t;
#define PSA_SUCCESS                 ((psa_status_t)0)
#endif
typedef int32_t psa_handle_t;

#ifndef PSA_NULL_HANDLE
#define PSA_NULL_HANDLE             ((psa_handle_t)0)
#endif

typedef struct psa_invec {
    const void *base;
    size_t len;
} psa_invec;

typedef struct psa_outvec {
    void *base;
    size_t len;
} psa_outvec;

#endif /* IPC */

#endif /* _PAL_COMMON_H_ */


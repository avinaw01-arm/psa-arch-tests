/** @file
 * Copyright (c) 2018-2025, Arm Limited or its affiliates. All rights reserved.
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

#include "pal_interfaces_ns.h"
#include "val_framework.h"
#include "val_client_defs.h"
#include "val_crypto.h"

/**
    @brief    - This API will call the requested crypto function
    @param    - type : function code
                ...  : variable number of arguments
    @return   - Error status
**/
int32_t val_crypto_function(int type, ...)
{
#ifdef CRYPTO
    va_list      valist;
    int32_t      status;

    va_start(valist, type);
    status = pal_crypto_function(type, valist);
    va_end(valist);
    return status;
#else
    (void)type;
    return VAL_STATUS_ERROR;
#endif
}

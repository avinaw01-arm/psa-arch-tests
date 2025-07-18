/** @file
 * Copyright (c) 2020-2025, Arm Limited or its affiliates. All rights reserved.
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
#ifndef _TEST_STORAGE_COMMON_H_
#define _TEST_STORAGE_COMMON_H_

#include "val_storage.h"
#include "val_interfaces.h"

#define val              CONCAT(val, test_entry)
#define psa              CONCAT(psa, test_entry)
#define STORAGE_FUNCTION val->storage_function
#define ITS_TEST_MESSAGE "[Info] Executing ITS tests\n"
#define PS_TEST_MESSAGE  "[Info] Executing PS tests\n"

extern val_api_t           *val;
extern psa_api_t           *psa;

typedef struct {
    val_test_index_t        idx;
    storage_function_type_t api[2];
    psa_status_t            status;
} test_data_t;

extern int32_t storage_buffer_comparison(uint8_t *buffer_to_compare, uint32_t test_buff_size);

#endif /* _TEST_STORAGE_COMMON_H_ */

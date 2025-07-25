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

#ifndef _VAL_H_
#define _VAL_H_

#include "pal_common.h"
#include "val_common_log.h"
#include "val_common_status.h"

#ifndef VAL_NSPE_BUILD
#define STATIC_DECLARE  static
#else
#define STATIC_DECLARE
#endif

#ifndef __WEAK
#define __WEAK __attribute__((weak))
#endif

#ifndef __UNUSED
#define __UNUSED __attribute__((unused))
#endif

#ifndef TRUE
#define TRUE 0
#endif
#ifndef FALSE
#define FALSE 1
#endif

#define _CONCAT(A, B) A##B
#define CONCAT(A, B) _CONCAT(A, B)

#define TEST_NUM_BIT         32
#define TEST_NUM_MASK        0xFFFFFFFF

#define VAL_ERROR(status)       ((status & TEST_STATUS_CODE_MASK) ? 1 : 0)


/* Test Defines */
#define TEST_PUBLISH(test_id, entry) \
   const val_test_info_t __attribute__((section(".acs_test_info"))) \
                      CONCAT(acs_test_info, entry) = {test_id, entry}

#define VAL_MAX_TEST_PER_COMP                200
#define VAL_FF_BASE                            0
#define VAL_CRYPTO_BASE                        1
#define VAL_STORAGE_BASE                       2
#define VAL_INITIAL_ATTESTATION_BASE           3

#define VAL_GET_COMP_NUM(test_id)      \
   ((test_id - (test_id % VAL_MAX_TEST_PER_COMP)) / VAL_MAX_TEST_PER_COMP)
#define VAL_GET_TEST_NUM(test_id)      (test_id % VAL_MAX_TEST_PER_COMP)
#define VAL_CREATE_TEST_ID(comp, num)   ((comp*VAL_MAX_TEST_PER_COMP) + num)

#define TEST_FIELD(num1, num2)           (num2 << 8 | num1)
#define GET_TEST_ISOLATION_LEVEL(num)   (num & 0x3)
#define GET_WD_TIMOUT_TYPE(num)         ((num >> 8) & 0x7)

#define TEST_CHECKPOINT_NUM(n)          n
#define TEST(n)                         n
#define BLOCK(n)                        n

#define BLOCK_NUM_POS                   8
#define ACTION_POS                      16
#define GET_TEST_NUM(n)                 (0xff & n)
#define GET_BLOCK_NUM(n)                ((n >> BLOCK_NUM_POS) & 0xff)

#define GET_ACTION_NUM(n)               ((n >> ACTION_POS) & 0xff)
#define TEST_EXECUTE_FUNC               1
#define TEST_RETURN_RESULT              2
#define INVALID_HANDLE                  0x1234DEAD

#define UART_INIT_SIGN  0xff
#define UART_PRINT_SIGN 0xfe

#define TEST_PANIC()                          \
    do {                                         \
    } while (1)

#define TEST_ASSERT_EQUAL(arg1, arg2, checkpoint)                                   \
    do {                                                                            \
    	if ((arg1) == PAL_STATUS_UNSUPPORTED_FUNC)                                  \
        {                                                                           \
            return RESULT_SKIP(VAL_STATUS_UNSUPPORTED);                             \
        }                                                                           \
        if ((arg1) != arg2)                                                         \
        {                                                                           \
            val->print(ERROR, "\tFailed at Checkpoint: %d\n", checkpoint);    \
            val->print(ERROR, "\tActual: %d\n", arg1);                        \
            val->print(ERROR, "\tExpected: %d\n", arg2);                      \
            return 1;                                                               \
        }                                                                           \
    } while (0)

#define TEST_ASSERT_DUAL(arg1, status1, status2, checkpoint)                        \
    do {                                                                            \
    	if ((arg1) == PAL_STATUS_UNSUPPORTED_FUNC)                                  \
        {                                                                           \
            return RESULT_SKIP(VAL_STATUS_UNSUPPORTED);                             \
        }                                                                           \
        if ((arg1) != status1 && (arg1) != status2)                                 \
        {                                                                           \
            val->print(ERROR, "\tFailed at Checkpoint: %d\n", checkpoint);    \
            val->print(ERROR, "\tActual: %d\n", arg1);                        \
            if ((status1) != (status2))                                             \
            {                                                                       \
                val->print(ERROR, "\tExpected: %d", status1);                 \
                val->print(ERROR, "or %d\n", status2);                        \
            }                                                                       \
            else                                                                    \
            {                                                                       \
                val->print(ERROR, "\tExpected: %d\n", status1);               \
            }                                                                       \
            return 1;                                                               \
        }                                                                           \
    } while (0)

#define TEST_ASSERT_NOT_EQUAL(arg1, arg2, checkpoint)                               \
    do {                                                                            \
    	if ((arg1) == PAL_STATUS_UNSUPPORTED_FUNC)                                  \
        {                                                                           \
            return RESULT_SKIP(VAL_STATUS_UNSUPPORTED);                             \
        }                                                                           \
        if ((arg1) == arg2)                                                         \
        {                                                                           \
            val->print(ERROR, "\tFailed at Checkpoint: %d\n", checkpoint);    \
            val->print(ERROR, "\tValue: %d\n", arg1);                         \
            return 1;                                                               \
        }                                                                           \
    } while (0)

#define TEST_ASSERT_MEMCMP(buf1, buf2, size, checkpoint)                            \
    do {                                                                            \
        if (memcmp(buf1, buf2, size))                                               \
        {                                                                           \
            val->print(ERROR, "\tFailed at Checkpoint: %d : ", checkpoint);   \
            val->print(ERROR, "Unequal data in compared buffers\n", 0);       \
            return 1;                                                               \
        }                                                                           \
    } while (0)

#define TEST_ASSERT_RANGE(arg1, range1, range2, checkpoint)                         \
    do {                                                                            \
    	if ((arg1) == PAL_STATUS_UNSUPPORTED_FUNC)                                  \
        {                                                                           \
            return RESULT_SKIP(VAL_STATUS_UNSUPPORTED);                             \
        }                                                                           \
        if ((arg1) < range1 || (arg1) > range2)                                     \
        {                                                                           \
            val->print(ERROR, "\tFailed at Checkpoint: %d\n", checkpoint);    \
            val->print(ERROR, "\tActual: %d\n", arg1);                        \
            val->print(ERROR, "\tExpected range: %d to ", range1);            \
            val->print(ERROR, "%d", range2);                                  \
            return 1;                                                               \
        }                                                                           \
    } while (0)

/* enums */
typedef enum {
    CALLER_NONSECURE = 0x0,
    CALLER_SECURE    = 0x1,
} caller_security_t;

typedef enum {
    TEST_ISOLATION_L1      = 0x1,
    TEST_ISOLATION_L2      = 0x2,
    TEST_ISOLATION_L3      = 0x3,
} test_isolation_level_t;

typedef enum {
    LEVEL1 = 0x1,
    LEVEL2,
    LEVEL3,
} isolation_level_t;

typedef enum {
    /* VAL uses this boot flag to mark first time boot of the system  */
    BOOT_UNKNOWN                       = 0x1,
    /* VAL/Test uses this boot flag to catch any unwanted system reboot - SIM ERROR Cases*/
    BOOT_NOT_EXPECTED                  = 0x2,
    /* Test performs panic check for non-secure test run and expect reboot */
    BOOT_EXPECTED_NS                   = 0x3,
    /* Test performs panic check for secure test run and expect reboot */
    BOOT_EXPECTED_S                    = 0x4,
    /* Test expects reboot but it didn't happen */
    BOOT_EXPECTED_BUT_FAILED           = 0x5,
    /* Test expects reboot for secure/non-secure test run. If reboot happens,
     * re-enter the same test and execute the next check function
     */
    BOOT_EXPECTED_REENTER_TEST         = 0x6,
    /* Test expect reboot for the test run. If reboot happens,
     * re-enter the same test and continue executing the same check function
     */
    BOOT_EXPECTED_CONT_TEST_EXEC       = 0x7,
    /* Test expects reboot for secure/non-secure on second check of test . If reboot happens,
     * re-enter the same test and execute the next check function
     */
    BOOT_EXPECTED_ON_SECOND_CHECK      = 0x8,
} boot_state_t;

/* enums to report test sub-state */
typedef enum {
  VAL_STATUS_SUCCESS                     = 0x0,
  VAL_STATUS_ERROR                       = 0x11,
  VAL_STATUS_NOT_FOUND                   = 0x12,
  VAL_STATUS_LOAD_ERROR                  = 0x13,
  VAL_STATUS_INSUFFICIENT_SIZE           = 0x14,
  VAL_STATUS_CONNECTION_FAILED           = 0x15,
  VAL_STATUS_CALL_FAILED                 = 0x16,
  VAL_STATUS_READ_FAILED                 = 0x17,
  VAL_STATUS_WRITE_FAILED                = 0x18,
  VAL_STATUS_ISOLATION_LEVEL_NOT_SUPP    = 0x19,
  VAL_STATUS_INIT_FAILED                 = 0x1A,
  VAL_STATUS_SPM_FAILED                  = 0x1B,
  VAL_STATUS_SPM_UNEXPECTED_BEH          = 0x1C,
  VAL_STATUS_FRAMEWORK_VERSION_FAILED    = 0x1D,
  VAL_STATUS_VERSION_API_FAILED          = 0x1E,
  VAL_STATUS_INVALID_HANDLE              = 0x1F,
  VAL_STATUS_INVALID_MSG_TYPE            = 0x20,
  VAL_STATUS_WRONG_IDENTITY              = 0x21,
  VAL_STATUS_MSG_INSIZE_FAILED           = 0x22,
  VAL_STATUS_MSG_OUTSIZE_FAILED          = 0x23,
  VAL_STATUS_SKIP_FAILED                 = 0x24,
  VAL_STATUS_CRYPTO_FAILURE              = 0x25,
  VAL_STATUS_INVALID_SIZE                = 0x26,
  VAL_STATUS_DATA_MISMATCH               = 0x27,
  VAL_STATUS_BOOT_EXPECTED_BUT_FAILED    = 0x28,
  VAL_STATUS_INIT_ALREADY_DONE           = 0x29,
  VAL_STATUS_HEAP_NOT_AVAILABLE          = 0x2A,
  VAL_STATUS_UNSUPPORTED                 = 0x2B,
  VAL_STATUS_DRIVER_FN_FAILED            = 0x2C,
  VAL_STATUS_NO_TESTS                    = 0X2D,
  VAL_STATUS_TEST_FAILED                 = 0x2E,
  VAL_STATUS_MAX_ERROR                   = INT_MAX,
} val_status_t;

/* Driver test function id enums */
typedef enum {
    TEST_PSA_EOI_WITH_NON_INTR_SIGNAL    = 1,
    TEST_PSA_EOI_WITH_MULTIPLE_SIGNALS   = 2,
    TEST_PSA_EOI_WITH_UNASSERTED_SIGNAL  = 3,
    TEST_INTR_SERVICE                    = 4,
    TEST_ISOLATION_PSA_ROT_DATA_RD       = 5,
    TEST_ISOLATION_PSA_ROT_DATA_WR       = 6,
    TEST_ISOLATION_PSA_ROT_STACK_RD      = 7,
    TEST_ISOLATION_PSA_ROT_STACK_WR      = 8,
    TEST_ISOLATION_PSA_ROT_HEAP_RD       = 9,
    TEST_ISOLATION_PSA_ROT_HEAP_WR       = 10,
    TEST_ISOLATION_PSA_ROT_MMIO_RD       = 11,
    TEST_ISOLATION_PSA_ROT_MMIO_WR       = 12,
} driver_test_fn_id_t;

typedef enum {
    NOT_AVAILABLE   = 0x0,
    AVAILABLE       = 0x1,
} is_available_t;

typedef enum {
    TYPE_READ_ONLY      = 0x10,
    TYPE_WRITE_ONLY,
    TYPE_READ_WRITE,
    TYPE_EXECUTE,
    TYPE_RESERVED,
} perm_type_t;

/* typedef's */
typedef struct {
    boot_state_t state;
} boot_t;

typedef struct {
    uint16_t test_num;
    uint8_t block_num;
} test_info_ipc_t;

typedef int32_t (*client_test_t)(caller_security_t caller);
typedef int32_t (*server_test_t)(void);
#endif /* VAL_H */

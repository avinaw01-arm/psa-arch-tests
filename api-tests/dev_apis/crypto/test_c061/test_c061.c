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

#include "val_interfaces.h"
#include "test_c061.h"
#include "test_data.h"

const client_test_t test_c061_crypto_list[] = {
    NULL,
    psa_aead_finish_test,
    NULL,
};

extern  uint32_t g_test_count;

int32_t psa_aead_finish_test(caller_security_t caller __UNUSED)
{
#if ((defined(ARCH_TEST_CCM) || defined(ARCH_TEST_GCM)) && defined(ARCH_TEST_AES_128) && \
(!defined(ARCH_TEST_AEAD_MULTISTAGE_OFF)))
    uint8_t               output[BUFFER_SIZE];
    uint8_t               tag[SIZE_128B];
    int32_t               i, status;
    size_t                length, finish_length, tag_length;
    int                   num_checks = sizeof(check1)/sizeof(check1[0]);
    psa_key_attributes_t  attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_aead_operation_t  operation = PSA_AEAD_OPERATION_INIT;
    psa_key_id_t          key;

    if (num_checks == 0)
    {
        val->print(TEST, "No test available for the selected crypto configuration\n", 0);
        return RESULT_SKIP(VAL_STATUS_NO_TESTS);
    }

    /* Initialize the PSA crypto library*/
    status = val->crypto_function(VAL_CRYPTO_INIT);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));

    for (i = 0; i < num_checks; i++)
    {
        val->print(TEST, "Check %d: ", g_test_count++);
        val->print(TEST, check1[i].test_desc, 0);

        /* Setting up the watchdog timer for each check */
        status = val->wd_reprogram_timer(WD_CRYPTO_TIMEOUT);
        TEST_ASSERT_EQUAL(status, VAL_STATUS_SUCCESS, TEST_CHECKPOINT_NUM(2));

        /* Setup the attributes for the key */
        val->crypto_function(VAL_CRYPTO_SET_KEY_TYPE, &attributes, check1[i].key_type);
        val->crypto_function(VAL_CRYPTO_SET_KEY_USAGE_FLAGS, &attributes, check1[i].usage);
        val->crypto_function(VAL_CRYPTO_SET_KEY_ALGORITHM, &attributes, check1[i].key_alg);

        /* Import the key data into the key slot */
        status = val->crypto_function(VAL_CRYPTO_IMPORT_KEY, &attributes, check1[i].key_data,
                 check1[i].key_length, &key);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(3));

        /* Set the key for a multipart authenticated encryption operation */
        status = val->crypto_function(VAL_CRYPTO_AEAD_ENCRYPT_SETUP, &operation,
                 key, check1[i].alg);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(4));

        /* Declare the lengths of the message and additional data for AEAD */
        status = val->crypto_function(VAL_CRYPTO_AEAD_SET_LENGTHS, &operation, check1[i].ad_length,
                 check1[i].input_length);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(5));

        /* Set the nonce for an authenticated encryption operation */
        status = val->crypto_function(VAL_CRYPTO_AEAD_SET_NONCE, &operation, check1[i].nonce,
                 check1[i].nonce_size);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(6));

        /* Pass additional data to an active AEAD operation */
        status = val->crypto_function(VAL_CRYPTO_AEAD_UPDATE_AD, &operation,
                 check1[i].additional_data, check1[i].ad_length);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(7));

        /* Encrypt or decrypt a message fragment in an active AEAD operation */
        status = val->crypto_function(VAL_CRYPTO_AEAD_UPDATE, &operation,
                 check1[i].plaintext, check1[i].plaintext_length, output,
                 BUFFER_SIZE, &length);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(8));

        /* Finish encrypting a message in an AEAD operation */
        status = val->crypto_function(VAL_CRYPTO_AEAD_FINISH, &operation, output + length,
                 check1[i].output_size, &finish_length, tag, check1[i].tag_size, &tag_length);
        TEST_ASSERT_EQUAL(status, check1[i].expected_status, TEST_CHECKPOINT_NUM(9));

        if (check1[i].expected_status != PSA_SUCCESS)
        {
            /* Finish encrypting a message with an inactive operator should fail */
            status = val->crypto_function(VAL_CRYPTO_AEAD_FINISH, &operation, output,
                 check1[i].output_size, &length, tag, check1[i].tag_size, &tag_length);
            TEST_ASSERT_EQUAL(status, PSA_ERROR_BAD_STATE, TEST_CHECKPOINT_NUM(10));

            /* Abort the AEAD operation */
            status = val->crypto_function(VAL_CRYPTO_AEAD_ABORT, &operation);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(11));

            /* Destroy the key */
            status = val->crypto_function(VAL_CRYPTO_DESTROY_KEY, key);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(12));

            /* Reset the key attributes */
            val->crypto_function(VAL_CRYPTO_RESET_KEY_ATTRIBUTES, &attributes);
            continue;
        }

        /* Compare the output and its length with the expected values */
        length += finish_length;
        TEST_ASSERT_EQUAL(length, check1[i].expected_length, TEST_CHECKPOINT_NUM(13));
        TEST_ASSERT_MEMCMP(output, check1[i].expected_output, length, TEST_CHECKPOINT_NUM(14));
        TEST_ASSERT_EQUAL(tag_length, check1[i].expected_tag_length, TEST_CHECKPOINT_NUM(15));
        TEST_ASSERT_MEMCMP(tag, check1[i].expected_tag, tag_length, TEST_CHECKPOINT_NUM(16));

        /* Abort the AEAD operation */
        status = val->crypto_function(VAL_CRYPTO_AEAD_ABORT, &operation);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(17));

        /* Destroy the key */
        status = val->crypto_function(VAL_CRYPTO_DESTROY_KEY, key);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(18));

        /* Reset the key attributes */
        val->crypto_function(VAL_CRYPTO_RESET_KEY_ATTRIBUTES, &attributes);

        /* Finish encrypting a message with an inactive operator should fail */
        status = val->crypto_function(VAL_CRYPTO_AEAD_FINISH, &operation, output,
                 check1[i].output_size, &length, tag, check1[i].tag_size, &tag_length);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_BAD_STATE, TEST_CHECKPOINT_NUM(19));

    }

    return VAL_STATUS_SUCCESS;
#else
    val->print(TEST, "No test available for the selected crypto configuration\n", 0);
    return RESULT_SKIP(VAL_STATUS_NO_TESTS);
#endif
}

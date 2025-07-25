#/** @file
# * Copyright (c) 2019-2025, Arm Limited or its affiliates. All rights reserved.
# * SPDX-License-Identifier : Apache-2.0
# *
# * Licensed under the Apache License, Version 2.0 (the "License");
# * you may not use this file except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *  http://www.apache.org/licenses/LICENSE-2.0
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
#**/

foreach(test ${PSA_TEST_LIST})
	include(${PSA_SUITE_DIR}/${test}/test.cmake)
	foreach(source_file ${CC_SOURCE})
		list(APPEND SUITE_CC_SOURCE
			${PSA_SUITE_DIR}/${test}/${source_file}
		)
	endforeach()
	foreach(asm_file ${AS_SOURCE})
		list(APPEND SUITE_AS_SOURCE
			${PSA_SUITE_DIR}/${test}/${asm_file}
		)
	endforeach()
	foreach(source_file ${CC_SOURCE_SPE})
		list(APPEND SUITE_CC_SOURCE_SPE
			${PSA_SUITE_DIR}/${test}/${source_file}
		)
	endforeach()
	foreach(asm_file ${AS_SOURCE_SPE})
		list(APPEND SUITE_AS_SOURCE_SPE
			${PSA_SUITE_DIR}/${test}/${asm_file}
		)
	endforeach()
	unset(CC_SOURCE)
	unset(AS_SOURCE)
	unset(CC_SOURCE_SPE)
	unset(AS_SOURCE_SPE)
endforeach()

add_library(${PSA_TARGET_TEST_COMBINE_LIB} STATIC ${SUITE_CC_SOURCE} ${SUITE_AS_SOURCE})
target_compile_definitions(${PSA_TARGET_TEST_COMBINE_LIB} PRIVATE CC_OPTIONS)
target_compile_definitions(${PSA_TARGET_TEST_COMBINE_LIB} PRIVATE AS_OPTIONS)
target_compile_definitions(${PSA_TARGET_TEST_COMBINE_LIB} PRIVATE NONSECURE_TEST_BUILD)

# Test related Include directories
foreach(test ${PSA_TEST_LIST})
	target_include_directories(${PSA_TARGET_TEST_COMBINE_LIB} PRIVATE ${PSA_SUITE_DIR}/${test})
endforeach()

# PSA Include directories
foreach(psa_inc_path ${PSA_INCLUDE_PATHS})
	target_include_directories(${PSA_TARGET_TEST_COMBINE_LIB} PRIVATE ${psa_inc_path})
endforeach()

target_include_directories(${PSA_TARGET_TEST_COMBINE_LIB} PRIVATE
	${CMAKE_CURRENT_BINARY_DIR}
	${PSA_ROOT_DIR}/val/common
	${PSA_ROOT_DIR}/val/nspe
	${PSA_ROOT_DIR}/val/spe
	${PSA_ROOT_DIR}/platform/targets/common/nspe
	${PSA_ROOT_DIR}/platform/targets/common/nspe/crypto
	${PSA_ROOT_DIR}/platform/targets/${TARGET}/nspe
	${COMMON_VAL_PATH}/inc
)

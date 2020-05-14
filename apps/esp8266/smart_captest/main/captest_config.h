/* ***************************************************************************
 *
 * Copyright 2020 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

 //config the capabilities wanted to test here. default use template
#define CAPTEST_SUPPORT_TEMPLATE	0

//#define CAPTEST_SUPPORT_PREFIX_A	1
#define CAPTEST_SUPPORT_PREFIX_D	1
//#define CAPTEST_SUPPORT_PREFIX_F	1
//#define CAPTEST_SUPPORT_PREFIX_L	1
 
#if CAPTEST_SUPPORT_PREFIX_A
 extern void captest_a_initialize(IOT_CTX *ctx);
 extern void send_a_test_capabilities(void);
 
#define custom_captest_init		captest_a_initialize
#define custom_send_capabilities	send_a_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_D
 extern void captest_d_initialize(IOT_CTX *ctx);
 extern void send_d_test_capabilities(void);

#define custom_captest_init		captest_d_initialize
#define custom_send_capabilities	send_d_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_F
 extern void captest_f_initialize(IOT_CTX *ctx);
 extern void send_f_test_capabilities(void);
#define custom_captest_init		captest_f_initialize
#define custom_send_capabilities	send_f_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_L
 extern void captest_l_initialize(IOT_CTX *ctx);
 extern void send_l_test_capabilities(void);

#define custom_captest_init		captest_l_initialize
#define custom_send_capabilities	send_l_test_capabilities

#else

 extern void captest_initialize(IOT_CTX *ctx);
 extern void send_test_capabilities(void);

#define custom_captest_init		captest_initialize
#define custom_send_capabilities	send_test_capabilities

#endif


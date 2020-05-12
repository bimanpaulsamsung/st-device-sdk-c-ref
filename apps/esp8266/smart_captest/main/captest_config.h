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

 //config the capabilities wanted to test here.
#define CAPTEST_SUPPORT_TEMPLATE	1
#define CAPTEST_SUPPORT_PREFIX_L	0
 
#if CAPTEST_SUPPORT_TEMPLATE
 extern void captest_initialize(IOT_CTX *ctx);
 extern void send_test_capabilities(void);
 
#define custom_captest_init		captest_initialize
#define custom_send_capabilities	send_test_capabilities
 
#elif CAPTEST_SUPPORT_PREFIX_L
 extern void captest_l_initialize(IOT_CTX *ctx);
 extern void send_l_test_capabilities(void);
 
#define custom_captest_init		captest_l_initialize
#define custom_send_capabilities	send_l_test_capabilities
#endif


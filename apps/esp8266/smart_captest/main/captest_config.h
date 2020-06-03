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
//#define CAPTEST_SUPPORT_PREFIX_B	1
//#define CAPTEST_SUPPORT_PREFIX_C	1
//#define CAPTEST_SUPPORT_PREFIX_D	1
//#define CAPTEST_SUPPORT_PREFIX_E	1
//#define CAPTEST_SUPPORT_PREFIX_F	1
//#define CAPTEST_SUPPORT_PREFIX_G	1
//#define CAPTEST_SUPPORT_PREFIX_I	1
//#define CAPTEST_SUPPORT_PREFIX_L	1
//#define CAPTEST_SUPPORT_PREFIX_M	1
//#define CAPTEST_SUPPORT_PREFIX_N	1
//#define CAPTEST_SUPPORT_PREFIX_O	1
//#define CAPTEST_SUPPORT_PREFIX_P	1
//#define CAPTEST_SUPPORT_PREFIX_R	1
//#define CAPTEST_SUPPORT_PREFIX_S	1
//#define CAPTEST_SUPPORT_PREFIX_T	1
//#define CAPTEST_SUPPORT_PREFIX_U	1
//#define CAPTEST_SUPPORT_PREFIX_V	1
#define CAPTEST_SUPPORT_PREFIX_W	1
//#define CAPTEST_SUPPORT_PREFIX_Z	1
 
#if CAPTEST_SUPPORT_PREFIX_A
 extern void captest_a_initialize(IOT_CTX *ctx);
 extern void send_a_test_capabilities(void);
 
#define custom_captest_init		captest_a_initialize
#define custom_send_capabilities	send_a_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_B
 extern void captest_b_initialize(IOT_CTX *ctx);
 extern void send_b_test_capabilities(void);

#define custom_captest_init		captest_b_initialize
#define custom_send_capabilities	send_b_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_C
 extern void captest_c_initialize(IOT_CTX *ctx);
 extern void send_c_test_capabilities(void);

#define custom_captest_init		captest_c_initialize
#define custom_send_capabilities	send_c_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_D
 extern void captest_d_initialize(IOT_CTX *ctx);
 extern void send_d_test_capabilities(void);

#define custom_captest_init		captest_d_initialize
#define custom_send_capabilities	send_d_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_E
 extern void captest_e_initialize(IOT_CTX *ctx);
 extern void send_e_test_capabilities(void);

#define custom_captest_init		captest_e_initialize
#define custom_send_capabilities	send_e_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_F
 extern void captest_f_initialize(IOT_CTX *ctx);
 extern void send_f_test_capabilities(void);
#define custom_captest_init		captest_f_initialize
#define custom_send_capabilities	send_f_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_G
 extern void captest_g_initialize(IOT_CTX *ctx);
 extern void send_g_test_capabilities(void);

#define custom_captest_init		captest_g_initialize
#define custom_send_capabilities	send_g_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_I
 extern void captest_i_initialize(IOT_CTX *ctx);
 extern void send_i_test_capabilities(void);

#define custom_captest_init		captest_i_initialize
#define custom_send_capabilities	send_i_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_L
 extern void captest_l_initialize(IOT_CTX *ctx);
 extern void send_l_test_capabilities(void);

#define custom_captest_init		captest_l_initialize
#define custom_send_capabilities	send_l_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_M
 extern void captest_m_initialize(IOT_CTX *ctx);
 extern void send_m_test_capabilities(void);

#define custom_captest_init		captest_m_initialize
#define custom_send_capabilities	send_m_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_N
 extern void captest_n_initialize(IOT_CTX *ctx);
 extern void send_n_test_capabilities(void);

#define custom_captest_init		captest_n_initialize
#define custom_send_capabilities	send_n_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_O
 extern void captest_o_initialize(IOT_CTX *ctx);
 extern void send_o_test_capabilities(void);

#define custom_captest_init		captest_o_initialize
#define custom_send_capabilities	send_o_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_P
 extern void captest_p_initialize(IOT_CTX *ctx);
 extern void send_p_test_capabilities(void);

#define custom_captest_init		captest_p_initialize
#define custom_send_capabilities	send_p_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_R
 extern void captest_r_initialize(IOT_CTX *ctx);
 extern void send_r_test_capabilities(void);

#define custom_captest_init		captest_r_initialize
#define custom_send_capabilities	send_r_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_S
 extern void captest_s_initialize(IOT_CTX *ctx);
 extern void send_s_test_capabilities(void);

#define custom_captest_init		captest_s_initialize
#define custom_send_capabilities	send_s_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_T
 extern void captest_t_initialize(IOT_CTX *ctx);
 extern void send_t_test_capabilities(void);

#define custom_captest_init		captest_t_initialize
#define custom_send_capabilities	send_t_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_U
 extern void captest_u_initialize(IOT_CTX *ctx);
 extern void send_u_test_capabilities(void);

#define custom_captest_init		captest_u_initialize
#define custom_send_capabilities	send_u_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_V
 extern void captest_v_initialize(IOT_CTX *ctx);
 extern void send_v_test_capabilities(void);

#define custom_captest_init		captest_v_initialize
#define custom_send_capabilities	send_v_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_W
 extern void captest_w_initialize(IOT_CTX *ctx);
 extern void send_w_test_capabilities(void);

#define custom_captest_init		captest_w_initialize
#define custom_send_capabilities	send_w_test_capabilities

#elif CAPTEST_SUPPORT_PREFIX_Z
 extern void captest_z_initialize(IOT_CTX *ctx);
 extern void send_z_test_capabilities(void);

#define custom_captest_init		captest_z_initialize
#define custom_send_capabilities	send_z_test_capabilities

#else

 extern void captest_initialize(IOT_CTX *ctx);
 extern void send_test_capabilities(void);

#define custom_captest_init		captest_initialize
#define custom_send_capabilities	send_test_capabilities

#endif


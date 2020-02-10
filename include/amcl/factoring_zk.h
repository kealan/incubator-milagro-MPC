/*
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
*/

/**
 * @file factoring_zk.h
 * @brief ZK proof of knwledge of factoring declarations
 *
 */

#ifndef FACTORING_ZK_H
#define FACTORING_ZK_H

#include "amcl/amcl.h"
#include "amcl/big_1024_58.h"
#include "amcl/ff_2048.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define FS_2048 MODBYTES_1024_58 * FFLEN_2048  /**< 2048 field size in bytes */
#define HFS_2048 MODBYTES_1024_58 * HFLEN_2048 /**< Half 2048 field size in bytes */

#define FACTORING_ZK_B 16 /**< Security parameter, length in bytes */

#define FACTORING_ZK_OK   0  /** < Proof successfully verified */
#define FACTORING_ZK_FAIL 91 /** < Invalid proof */

/*!
 * \brief RSA modulus for ZKP
 */
typedef struct
{
    BIG_1024_58 n[FFLEN_2048]; /**< Integer to prove knowledge of factoring */
    BIG_1024_58 p[HFLEN_2048]; /**< First prime factor of n */
    BIG_1024_58 q[HFLEN_2048]; /**< Second prime factor of n*/
} FACTORING_ZK_modulus;

/** \brief Prove knowledge of the modulus m in ZK
 *
 *  @param  m           Modulus to prove knowledge of factoring for
 *  @param  RNG         Cryptographically secure PRNG
 *  @param  R           Random value used in the proof. If RNG is NULL this is read
 *  @param  E           Fisrt component of the ZK proof
 *  @param  Y           Second component of the ZK proof
 */
void FACTORING_ZK_prove(FACTORING_ZK_modulus *m, csprng *RNG, octet *R, octet *E, octet *Y);

/** \brief Verify ZK proof of knowledge of factoring of N
 *
 *  Verify that (E, Y) is a valid proof of knowldge of factoring of N
 *
 *  @param  N           Public integer, the RSA modulus
 *  @param  E           Fisrt component of the ZK proof
 *  @param  Y           Second component of the ZK proof
 *  @return             1 if the proof is valid, 0 otherwise
 */
int FACTORING_ZK_verify(octet *N, octet *E, octet *Y);

/** \brief Clear modulus
 *
 *  @param m             Modulus for the ZK proof to clean
 *
 */
void FACTORING_ZK_kill_modulus(FACTORING_ZK_modulus *m);

#ifdef __cplusplus
}
#endif

#endif

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

#include "amcl/schnorr.h"

/* Schnorr's proofs smoke test */

int main()
{
    int rc;

    BIG_256_56 x;
    BIG_256_56 q;
    ECP_SECP256K1 G;

    char x_char[SGS_SECP256K1];
    octet X = {0, sizeof(x_char), x_char};

    char v[SFS_SECP256K1+1];
    octet V = {0, sizeof(v), v};

    char r[SGS_SECP256K1];
    octet R = {0, sizeof(r), r};

    char c[SFS_SECP256K1+1];
    octet C = {0, sizeof(c), c};

    char e[SGS_SECP256K1];
    octet E = {0, sizeof(e), e};

    char p[SGS_SECP256K1];
    octet P = {0, sizeof(p), p};

    // Deterministic RNG for testing
    char seed[32] = {0};
    csprng RNG;
    RAND_seed(&RNG, 32, seed);

    BIG_256_56_rcopy(q, CURVE_Order_SECP256K1);
    BIG_256_56_randomnum(x, q, &RNG);

    ECP_SECP256K1_generator(&G);
    ECP_SECP256K1_mul(&G, x);

    BIG_256_56_toBytes(X.val, x);
    X.len = SGS_SECP256K1;

    ECP_SECP256K1_toOctet(&V, &G, 1);

    SCHNORR_commit(&RNG, &R, &C);

    SCHNORR_challenge(&V, &C, &E);

    SCHNORR_prove(&R, &E, &X, &P);

    rc = SCHNORR_verify(&V, &C, &E, &P);
    if (rc)
    {
        printf("FAILURE SCHNORR_verify. RC %d\n", rc);
        exit(EXIT_FAILURE);
    }

    printf("SUCCESS\n");
    exit(EXIT_SUCCESS);
}
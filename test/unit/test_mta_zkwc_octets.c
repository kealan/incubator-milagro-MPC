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

#include <string.h>
#include "test.h"
#include "amcl/mta.h"

/* MTA Receiver ZK Proof with check dump/load to octets unit tests */

#define LINE_LEN 2048

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("usage: ./test_mta_zkwc_octets [path to test vector file]\n");
        exit(EXIT_FAILURE);
    }

    int rc;
    int test_run = 0;

    FILE *fp;
    char line[LINE_LEN] = {0};

    char err_msg[128];

    const char *TESTline = "TEST = ";
    int testNo = 0;

    MTA_ZKWC_commitment c;
    MTA_ZKWC_commitment c_reloaded;
    const char *Uline  = "U = ";
    const char *Zline  = "Z = ";
    const char *Z1line = "Z1 = ";
    const char *Tline  = "T = ";
    const char *Vline  = "V = ";
    const char *Wline  = "W = ";

    MTA_ZKWC_proof proof;
    MTA_ZKWC_proof proof_reloaded;
    const char *Sline  = "S = ";
    const char *S1line = "S1 = ";
    const char *S2line = "S2 = ";
    const char *T1line = "T1 = ";
    const char *T2line = "T2 = ";

    char oct1[FS_2048];
    octet OCT1 = {0, sizeof(oct1), oct1};

    char oct2[2 * FS_2048];
    octet OCT2 = {0, sizeof(oct2), oct2};

    char oct3[2 * FS_2048];
    octet OCT3 = {0, sizeof(oct3), oct3};

    char oct4[2 * FS_2048];
    octet OCT4 = {0, sizeof(oct4), oct4};

    char oct5[2 * FS_2048];
    octet OCT5 = {0, sizeof(oct5), oct5};

    char octECP[EFS_SECP256K1 + 1];
    octet OCTECP = {0, sizeof(octECP), octECP};

    // Make sure proof is properly zeroed before starting test
    FF_2048_zero(proof.s1, FFLEN_2048);

    // Line terminating a test vector
    const char *last_line = Uline;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("ERROR opening test vector file\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, LINE_LEN, fp) != NULL)
    {
        scan_int(&testNo, line, TESTline);

        scan_FF_2048(fp, c.zkc.z,  line, Zline,  FFLEN_2048);
        scan_FF_2048(fp, c.zkc.z1, line, Z1line, FFLEN_2048);
        scan_FF_2048(fp, c.zkc.t,  line, Tline,  FFLEN_2048);
        scan_FF_2048(fp, c.zkc.v,  line, Vline,  2 * FFLEN_2048);
        scan_FF_2048(fp, c.zkc.w,  line, Wline,  FFLEN_2048);

        scan_ECP_SECP256K1(fp, &(c.U), line, Uline);

        scan_FF_2048(fp, proof.s,  line, Sline,  FFLEN_2048);
        scan_FF_2048(fp, proof.s1, line, S1line, HFLEN_2048);
        scan_FF_2048(fp, proof.s2, line, S2line, FFLEN_2048 + HFLEN_2048);
        scan_FF_2048(fp, proof.t1, line, T1line, FFLEN_2048);
        scan_FF_2048(fp, proof.t2, line, T2line, FFLEN_2048 + HFLEN_2048);

        if (!strncmp(line, last_line, strlen(last_line)))
        {
            // Dump and reload commitment
            MTA_ZKWC_commitment_toOctets(&OCTECP, &OCT1, &OCT2, &OCT3, &OCT4, &OCT5, &c);
            rc = MTA_ZKWC_commitment_fromOctets(&c_reloaded, &OCTECP, &OCT1, &OCT2, &OCT3, &OCT4, &OCT5);
            sprintf(err_msg, "FAILURE MTA_ZKWC_commitment_fromOctets. rc = %d.", rc);
            assert_tv(fp, testNo, err_msg, rc == MTA_OK);

            compare_FF_2048(fp, testNo, "c.z",  c.zkc.z,  c_reloaded.zkc.z,  FFLEN_2048);
            compare_FF_2048(fp, testNo, "c.z1", c.zkc.z1, c_reloaded.zkc.z1, FFLEN_2048);
            compare_FF_2048(fp, testNo, "c.t",  c.zkc.t,  c_reloaded.zkc.t,  FFLEN_2048);
            compare_FF_2048(fp, testNo, "c.v",  c.zkc.v,  c_reloaded.zkc.v,  2 * FFLEN_2048);
            compare_FF_2048(fp, testNo, "c.w",  c.zkc.w,  c_reloaded.zkc.w,  FFLEN_2048);

            compare_ECP_SECP256K1(fp, testNo, "c.U", &(c.U), &(c_reloaded.U));

            MTA_ZKWC_proof_toOctets(&OCT1, &OCT2, &OCT3, &OCT4, &OCT5, &proof);
            MTA_ZKWC_proof_fromOctets(&proof_reloaded, &OCT1, &OCT2, &OCT3, &OCT4, &OCT5);

            compare_FF_2048(fp, testNo, "proof.s",  proof.s,  proof_reloaded.s,  FFLEN_2048);
            compare_FF_2048(fp, testNo, "proof.s1", proof.s1, proof_reloaded.s1, FFLEN_2048);
            compare_FF_2048(fp, testNo, "proof.s2", proof.s2, proof_reloaded.s2, FFLEN_2048 + HFLEN_2048);
            compare_FF_2048(fp, testNo, "proof.t1", proof.t1, proof_reloaded.t1, FFLEN_2048);
            compare_FF_2048(fp, testNo, "proof.t2", proof.t2, proof_reloaded.t2, FFLEN_2048 + HFLEN_2048);

            // Mark that at least one test vector was executed
            test_run = 1;
        }
    }

    fclose(fp);

    if (test_run == 0)
    {
        printf("ERROR no test vector was executed\n");
        exit(EXIT_FAILURE);
    }

    // Test invalid U
    OCT_clear(&OCTECP);
    OCTECP.len = OCTECP.max;
    rc = MTA_ZKWC_commitment_fromOctets(&c_reloaded, &OCTECP, &OCT1, &OCT2, &OCT3, &OCT4, &OCT5);
    sprintf(err_msg, "FAILURE MTA_ZKWC_commitment_fromOctets invalid U. rc = %d.", rc);
    assert_tv(fp, testNo, err_msg, rc == MTA_INVALID_ECP);

    printf("SUCCESS");
    exit(EXIT_SUCCESS);
}

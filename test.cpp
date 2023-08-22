#include <base58.h>
#include <crypto/hmac_sha512.h>
#include <openssl/ripemd.h>
#include <openssl/sha.h>
#include <db.h>

#include <iostream>

#include <unistd.h>
#include <string.h>
#include <vector>
#include <cstdio>

void get_hex_seed(size_t bitlen, uint32_t incr, char *data);

int main()
{
        initdb();

        int bitlen = 256;
        uint32_t incr = 1526711362;
        char seed[32];
        char hexseed[64+1];
        std::vector<std::string> split;
        memset(hexseed, 0, sizeof(hexseed));

        uint64_t exactline = 0;

        while (true) {

                ++exactline;
                std::string linereturn;
                return_exact_line(exactline, linereturn);
                if (exists_in_db(linereturn)) {
                    printf("\rfound %s at line %d\n", linereturn.c_str(), exactline);
                }

        }


	return 1;
}


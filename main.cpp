#include <bip32.h>
#include <db.h>
#include <entropy.h>
#include <simplelogger.h>

#include <unistd.h>
#include <string.h>

#include <iostream>
#include <thread>
#include <vector>
#include <cstdio>

bool ecc_started{false};

extern "C" {
    void btc_ecc_start();
    void btc_ecc_stop();
}

// global progress variable
uint32_t progress{0};

void print_progress()
{
	while (true)
	{
		printf("%08x\n", progress);
		sleep(15);
	}
}

void worker_thread(uint32_t thr_id, int thr_total)
{
        int bitlen = 256;
        char entropy[32];
        char hexentropy[64+1];
        uint32_t increment = thr_id;
        memset(hexentropy, 0, sizeof(hexentropy));

        while (true)
        {
            // set global variable
            if (increment > progress) {
                progress = increment;
            }

            // pull entropy
            get_hex_entropy(bitlen, increment, entropy);
            for (int i=0; i<bitlen/8; i++) {
                sprintf(hexentropy+(i*2), "%02hhx", entropy[i]);
            }
            std::string entropy = std::string(hexentropy);

            // entropy->bip39seed->p2kh
            int pathstart = 0;
            int pathfinish = 3;
            std::vector<std::string> p2khlist;
            std::string path = "m/44'/0'/0'/0/";
            entropy_to_p2kh(entropy, bitlen, path, pathstart, pathfinish, p2khlist);

            // check for db match
            for (int i=0; i<p2khlist.size(); i++) {
                if (exists_in_db(p2khlist[i])) {
                    printf("found %s in entropy %s\n", p2khlist[i].c_str(), hexentropy);
                    std::string logentry = std::to_string(increment) + "," + std::string(hexentropy) + "," + path + "," + p2khlist[i] + "\n";
                    filelogger(logentry);
                } 
            }
            increment = increment + thr_total;
        }
}

int main()
{
    if (!ecc_started) {
        btc_ecc_start();
        ecc_started = true;
    }

    int threads = 4;
    initdb();

    std::vector<std::thread> workers;
    for (int i=0; i<threads; i++) {
         workers.push_back(std::thread(&worker_thread, i, threads));
    }

    workers.push_back(std::thread(&print_progress));

    for (int i=0; i<threads+1; i++) {
         workers.at(i).join();
    }

    return 1;
}

#include <bip32.h>
#include <db.h>
#include <deriv.h>
#include <entropy.h>
#include <timer.h>
#include <simplelogger.h>

#include <unistd.h>
#include <string.h>

#include <iostream>
#include <thread>
#include <vector>
#include <cstdio>

extern "C" {
    void btc_ecc_start();
    void btc_ecc_stop();
}

#define DEBUG 0
#define MAX_THREADS 4
bool ecc_started{false};
uint32_t progress[MAX_THREADS];

derivation_type current;

void print_progress()
{
	uint32_t lastprogress[MAX_THREADS];
	for (int i=0; i<MAX_THREADS; i++) {
		lastprogress[i] = 0;
	}

	while (true)
	{
		for (int i=0; i<MAX_THREADS; i++) {
			uint32_t work = (progress[i]-lastprogress[i]) * 12;
			if (work < 0) work = 0;
			printf("%d:%08x (%d/min) ", i, progress[i], work);
			lastprogress[i] = progress[i];
		}
		printf("\n");
		sleep(5);
	}
}

void worker_thread(uint32_t thr_id, int thr_total)
{
        int bitlen = 256;
        char entropy[32];
        char hexentropy[64+1];
        uint32_t increment = (4294967296 / thr_total) * thr_id;
        memset(hexentropy, 0, sizeof(hexentropy));

#if DEBUG
        Timer leg1, leg2, leg3;
#endif

        while (true)
        {
            // set progress count
            progress[thr_id] = increment;

            {
                // pull entropy
#if DEBUG
                leg1.start();
#endif
                get_hex_entropy(bitlen, increment, entropy);
                for (int i=0; i<bitlen/8; i++) {
                    sprintf(hexentropy+(i*2), "%02hhx", entropy[i]);
                }
                std::string entropy = std::string(hexentropy);
#if DEBUG
                leg1.stop();
#endif

                // entropy->bip39seed->p2kh
#if DEBUG
                leg2.start();
#endif
                int pathstart = 0;
                int pathfinish = 19;
                std::vector<std::string> p2khlist;
                std::string path = current == BTC_COMPAT ? "m/49'/0'/0'/0/" : "m/44'/0'/0'/0/";
                entropy_to_p2kh(entropy, bitlen, path, pathstart, pathfinish, p2khlist);
#if DEBUG
                leg2.stop();
#endif

                // check for db match
#if DEBUG
                leg3.start();
#endif
                for (int i=0; i<p2khlist.size(); i++) {
                    address_t match;
                    memcpy(match.bytes, p2khlist[i].c_str(), 12);
                    //printf("%s\n", p2khlist[i].c_str());
                    if (exists_in_db(match)) {
                        printf("found %s in entropy %s\n", p2khlist[i].c_str(), hexentropy);
                        std::string logentry = std::to_string(increment) + "," + std::string(hexentropy) + "," + path + "," + p2khlist[i] + "\n";
                        filelogger(logentry);
                    }
                }
#if DEBUG
                leg3.stop();
#endif

#if DEBUG
                uint64_t seed = leg1.between_nanoseconds();
                uint64_t bip39 = leg2.between_nanoseconds();
                uint64_t search = leg3.between_nanoseconds();

                printf("%llu %llu %llu\n", seed, bip39, search);
#endif
            }

            ++increment;
        }
}

int main()
{
    if (!ecc_started) {
        btc_ecc_start();
        ecc_started = true;
    }

    int threads = MAX_THREADS;
    current = BTC_LEGACY;
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

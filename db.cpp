#include <db.h>
#include <deriv.h>
#include <timer.h>

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <cstdint>

#include <algorithm>
#include <vector>

#define DEBUG 0

#define PARTIAL 12
extern derivation_type current;
std::vector<address_t> addresses[127][127];

bool match_address(const address_t& m1, const address_t& m2)
{
	return m1.bytes[0] == m2.bytes[0] && \
           m1.bytes[1] == m2.bytes[1] && \
           m1.bytes[2] == m2.bytes[2] && \
           m1.bytes[3] == m2.bytes[3] && \
           m1.bytes[4] == m2.bytes[4] && \
           m1.bytes[5] == m2.bytes[5] && \
           m1.bytes[6] == m2.bytes[6] && \
           m1.bytes[7] == m2.bytes[7] && \
           m1.bytes[8] == m2.bytes[8] && \
           m1.bytes[9] == m2.bytes[9] && \
           m1.bytes[10] == m2.bytes[10] && \
           m1.bytes[11] == m2.bytes[11];
}

bool exists_in_db(const address_t& address)
{
#if DEBUG
    Timer leg1;
#endif

#if DEBUG
    leg1.start();
#endif

    unsigned int a = (unsigned int)address.bytes[8];
    unsigned int c = (unsigned int)address.bytes[9];

    for(std::vector<address_t>::const_iterator it=addresses[a][c].begin(); it!=addresses[a][c].end(); it++) {
        if (match_address(*it, address)) return true;
    }

#if DEBUG
    leg1.stop();
#endif

#if DEBUG
    uint64_t search = leg1.between_nanoseconds();
    printf("%llu\n", search);
#endif

    return false;
}

void initdb()
{
    std::ifstream file("Bitcoin_addresses_August_13_2023.txt");

    uint64_t entries{0};
    std::string matchtype = current == BTC_COMPAT ? std::string("3") : std::string("1");

    std::string line;
    while (std::getline(file, line)) {
        if (line.substr(0,1) == matchtype) {

            address_t entry;
            memcpy(entry.bytes, line.c_str(), 12);

            ++entries;
            unsigned int a = (unsigned int)entry.bytes[8];
            unsigned int c = (unsigned int)entry.bytes[9];
            addresses[a][c].push_back(entry);
        }
    };

    printf("%d addresses loaded..\n", entries);
}

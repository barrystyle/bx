#include <timer.h>

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <cstdint>

#include <algorithm>
#include <vector>

#define DEBUG 0

#define PARTIAL 12
std::vector<std::string> addresses[127][127];

bool exists_in_db(std::string& address)
{
#if DEBUG
    Timer leg1;
#endif

#if DEBUG
    leg1.start();
#endif

    unsigned int a = (unsigned int)address.substr(8,1).c_str()[0];
    unsigned int c = (unsigned int)address.substr(9,1).c_str()[0];

    if (std::find(addresses[a][c].begin(), addresses[a][c].end(), address.substr(0, PARTIAL)) != addresses[a][c].end()) {
        return true;
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

void return_exact_line(uint64_t linenum, std::string& address)
{
    std::ifstream file("Bitcoin_addresses_August_13_2023.txt");

    uint64_t entries{0};

    std::string line;
    while (std::getline(file, line)) {
         ++entries;
         if (entries == linenum) {
             address = line.substr(0, PARTIAL);
             return;
         }
    }
}

void initdb()
{
    std::ifstream file("Bitcoin_addresses_August_13_2023.txt");

    uint64_t entries{0};

    std::string line;
    while (std::getline(file, line)) {
       if (line.substr(0, 3) != std::string("bc1")) {
         ++entries;
         unsigned int a = (unsigned int)line.substr(8,1).c_str()[0];
         unsigned int c = (unsigned int)line.substr(9,1).c_str()[0];
         addresses[a][c].push_back(line.substr(0, PARTIAL));
       }
    };

    printf("%d addresses loaded..\n", entries);
}

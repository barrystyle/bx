#include <stdio.h>
#include <string.h>
#include <fstream>
#include <cstdint>

#include <algorithm>
#include <vector>

#define PARTIAL 24
std::vector<std::string> addresses[127][127];

bool exists_in_db(std::string& address)
{
    std::string b = address.substr(6,1);
    unsigned int a = int(b.c_str()[0]);
    std::string d = address.substr(7,1);
    unsigned int c = int(d.c_str()[0]);

    if (std::find(addresses[a][c].begin(), addresses[a][c].end(), address.substr(0, PARTIAL)) != addresses[a][c].end()) {
        return true;
    }

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
         ++entries;
         std::string a = line.substr(6,1);
         unsigned int b = int(a.c_str()[0]);
         std::string c = line.substr(7,1);
         unsigned int d = int(c.c_str()[0]);
         addresses[b][d].push_back(line.substr(0, PARTIAL));
    };

    printf("%d addresses loaded..\n", entries);
}

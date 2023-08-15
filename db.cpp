#include <stdio.h>
#include <string.h>
#include <fstream>
#include <cstdint>

#include <algorithm>
#include <vector>

std::vector<std::string> addresses[127][127];

bool exists_in_db(std::string& address)
{
    // third byte
    std::string b = address.substr(3,1);
    unsigned int a = int(b.c_str()[0]);

    std::string d = address.substr(4,1);
    unsigned int c = int(d.c_str()[0]);

    if (std::find(addresses[a][c].begin(), addresses[a][c].end(), address) != addresses[a][c].end()) {
        return true;
    }

    return false;
}

void initdb()
{
    std::ifstream file("Bitcoin_addresses_August_13_2023.txt");

    uint64_t entries{0};

    std::string line;
    while (std::getline(file, line)) {
         if (line.substr(0,3) != "bc1") {
             ++entries;
             std::string a = line.substr(3,1);
             unsigned int b = int(a.c_str()[0]);
             std::string c = line.substr(4,1);
             unsigned int d = int(c.c_str()[0]);
             addresses[b][d].push_back(line);
         }
    };

    printf("%d addresses loaded..\n", entries);
}

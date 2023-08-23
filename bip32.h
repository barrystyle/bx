#ifndef BIP32_H
#define BIP32_H

#include <string.h>
#include <string>
#include <vector>

void get_bip32_btckey(std::string& seed, std::string& path, int start, int last, std::vector<std::string>& p2khlist);
void entropy_to_p2kh(std::string& entropy, int bitlen, std::string& path, int start, int last, std::vector<std::string>& p2khlist);

#endif // BIP32_H

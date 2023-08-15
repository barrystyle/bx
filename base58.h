#ifndef BASE58_H
#define BASE58_H

#include <string>
#include <vector>

std::string EncodeBase58(const unsigned char* pbegin, const unsigned char* pend);
std::string EncodeBase58(const std::vector<unsigned char>& vch);
std::string EncodeBase58Check(const std::vector<unsigned char>& vchIn, bool priv);

#endif // BASE58_H

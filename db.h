#ifndef DB_H
#define DB_H

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <cstdint>

bool exists_in_db(std::string& address);
void return_exact_line(uint64_t linenum, std::string& address);
void initdb();

#endif // DB_H

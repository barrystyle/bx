#ifndef DB_H
#define DB_H

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <cstdint>

// storage def/methods
typedef struct {
    char bytes[12];
} address_t;

bool exists_in_db(const address_t& address);
void initdb();

#endif // DB_H

#include <string.h>

unsigned char binvalue(const char v)
{
    if (v >= '0' && v <= '9')
        return v - '0';

    if (v >= 'a' && v <= 'f')
        return v - 'a' + 10;

    return 0;
}

void binlify(unsigned char* bin, const char* hex)
{
    int len = strlen(hex);
    for (int i = 0; i < len / 2; i++)
        bin[i] = binvalue(hex[i * 2]) << 4 | binvalue(hex[i * 2 + 1]);
}

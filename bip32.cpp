#include <string.h>
#include <string>
#include <btc/bip32.h>
#include <btc/utils.h>

bool ecc_started{false};

extern "C" {
    void btc_ecc_start();
    void btc_ecc_stop();
}

void print_keychain(uint8_t* key, uint8_t* chain)
{
    for (int i = 0; i < 32; i++) {
        printf("%02hhx", *(chain + i));
    }
    printf("\n");
    for (int i = 0; i < 32; i++) {
        printf("%02hhx", *(key + i));
    }
    printf("\n");
}

void get_bip32_btckey(std::string& entropy, std::string& path, std::string& p2kh)
{
    if (!ecc_started) {
        btc_ecc_start();
        ecc_started = true;
    }

    btc_hdnode node, node2;

    char str[512];
    memset(str, 0, sizeof(str));

    uint8_t private_key[32];
    uint8_t chain_code[32];

    /* init m */
    btc_hdnode_from_seed(utils_hex_to_uint8(entropy.c_str()), entropy.size() / 2, &node);
    memcpy(private_key, node.private_key, 32);
    memcpy(chain_code, node.chain_code, 32);

    if (path == "m" || path == "m/") {
        btc_hdnode_get_p2pkh_address(&node, &btc_chainparams_main, str, sizeof(str));
        p2kh = std::string(str);
        return;
    }

    /* derive for path */
    btc_hd_generate_key(&node2, path.c_str(), private_key, chain_code, false);
    btc_hdnode_get_p2pkh_address(&node2, &btc_chainparams_main, str, sizeof(str));

    p2kh = std::string(str);
}


#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "data_slice.hpp"

#include <boost/thread.hpp>

constexpr uint8_t byte_bits = 8;
typedef std::vector<uint8_t> data_chunk;
constexpr uint8_t max_uint8 = std::numeric_limits<uint8_t>::max();

std::mt19937& get_twister(uint32_t increment)
{
    // Boost.thread will clean up the thread statics using this function.
    auto const deleter = [](std::mt19937* twister) {
        delete twister;
    };

    // Maintain thread static state space.
    static boost::thread_specific_ptr<std::mt19937> twister(deleter);

    // This is thread safe because the instance is thread static.
    if (twister.get() == nullptr) {
        // Seed with high resolution clock.
        twister.reset(new std::mt19937(static_cast<uint32_t>(increment)));
    }

    return *twister;
}

void pseudo_random_fill(data_chunk& chunk, uint32_t increment)
{
    // uniform_int_distribution is undefined for sizes < 16 bits.
    std::uniform_int_distribution<uint16_t> distribution(0, max_uint8);

    for (auto& byte: chunk) {
        byte = static_cast<uint8_t>(distribution(get_twister(increment)));
    }
}

data_chunk new_entropy(size_t bit_length, uint32_t increment)
{
    size_t fill_entropy_size = bit_length / byte_bits;
    data_chunk entropy(fill_entropy_size);
    pseudo_random_fill(entropy, increment);
    return entropy;
}

void get_hex_entropy(size_t bitlen, uint32_t increment, char *data)
{
    const auto entropy = new_entropy(bitlen, increment);
    memcpy(data, entropy.data(), bitlen / 8);
}

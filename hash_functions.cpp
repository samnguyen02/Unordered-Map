#include "hash_functions.h"

size_t polynomial_rolling_hash::operator() (std::string const & str) const {
    const int HASH_B = 19;
    const long HASH_M = 3298534883309ul;

    size_t hash = 0;
    long p = 1;

    for (char c: str) {
        hash += c*p;
        p = (p* HASH_B) % HASH_M;
    }
    return hash;
}

size_t fnv1a_hash::operator() (std::string const & str) const {
    size_t hash = 0xCBF29CE484222325;
    for (int c: str) {
        hash = hash ^ c;
        hash = hash * 0x00000100000001B3;
    }
    return hash;
}

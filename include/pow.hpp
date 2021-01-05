#ifndef CRYPTO_PROOF_OF_WORK_HPP
#define CRYPTO_PROOF_OF_WORK_HPP

#include <limits.h>
#include <gmpxx.h>

#include "blockchain.hpp"

#include "utils.hpp"

#define HEXBITS 16
#define MAX_NONCE 9223372036854775807

static const int64_t targetBits = 10;

class ProofOfWork {
public:
    ProofOfWork(Block& pow_block);

    std::string prepareData(int nonce);
    int         run(std::string& hash);
    bool        validate();
private:
    Block *block;
    mpz_class target;

    std::string intToHex(int64_t val);
};

#endif
#ifndef CRYPTO_BLOCK_HPP
#define CRYPTO_BLOCK_HPP

#include <chrono>
#include <ctime>
#include <string>
#include <vector>

#include "cryptopp/cryptlib.h"
#include "cryptopp/sha.h"

#include "block.pb.h"

#include "utils.hpp"
#include "transaction.hpp"

using namespace CryptoPP;

class Block {
public:
    // Old constructor TODO: Remove
    Block (std::string data, std::string prevBlockHash);
    // Normal Block Constructor
    Block (std::vector<Transaction> transactions, std::string prevBlockHash);
    // Gensis Block
    Block (Transaction coinbase);
    Block (CryptoProtobuf::Block* d_block);
    void loadBlock(CryptoProtobuf::Block* s_block);

    // Getters
    std::string hashTransactions();
    std::string getPrevBlockHash();
    std::string getHash();

    // Instance Variables
    time_t      timestamp;
    std::vector<Transaction> transactions;
    std::string prevBlockHash;
    std::string hash;
    int     nonce;

private:
    void createBlock(std::vector<Transaction> transactions, std::string prevBlockHash);
};

namespace BlockSerialize {
    std::string serialize(Block* b);
    Block deserialize(std::string& serialBlockString);
}

typedef Block GenesisBlock;

#endif
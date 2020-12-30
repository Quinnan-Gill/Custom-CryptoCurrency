#ifndef CRYPTO_BLOCKCHAIN_HPP
#define CRYPTO_BLOCKCHAIN_HPP

#include <string>
#include <iostream>

#include "cryptodb.hpp"
#include "block.hpp"

class BlockChain {
public:
    // NewBlockchain
    BlockChain ();
    ~BlockChain();

    void addBlock(std::string& data);
    Block newGenesisBlock();

    // Instance Variables
    std::string tip; // The hash of the last block
    CryptoDB* db;
};

#endif
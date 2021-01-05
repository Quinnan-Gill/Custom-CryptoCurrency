#ifndef BLOCKCHAIN_ITERATOR_HPP
#define BLOCKCHAIN_ITERATOR_HPP

#include <string>

#include "cryptodb.hpp"
#include "block.hpp"
#include "blockchain.hpp"

class BlockChain;

class BlockchainIterator {
public:
    BlockchainIterator(BlockChain* bc);

    Block nextBlock();
private:
    std::string currentHash;
    CryptoDB* db;
};

#endif
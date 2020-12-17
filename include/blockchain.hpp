#ifndef CRYPTO_BLOCKCHAIN_HPP
#define CRYPTO_BLOCKCHAIN_HPP

#include <chrono>
#include <ctime>
#include <string>
#include <vector>

#include "cryptopp/cryptlib.h"
#include "cryptopp/sha.h"

using namespace CryptoPP;

class Block {
public:
    Block (std::string data, std::string prevBlockHash);

    // Block() {}
    void        setHash();
    std::string getPrevBlockHash();
    std::string getData();
    std::string getHash();

    // Instance Variables
    time_t      timestamp;
    std::string data;
    std::string prevBlockHash;
    std::string hash;
    int     nonce;
};

class BlockChain {
public:
    // NewBlockchain
    BlockChain ();

    void addBlock(std::string data);
    Block newGenesisBlock();

    // Instance Variables
    std::vector<Block> blocks;
};

std::string getHex(const std::string& input);

#endif
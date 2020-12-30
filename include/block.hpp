#ifndef CRYPTO_BLOCK_HPP
#define CRYPTO_BLOCK_HPP

#include <chrono>
#include <ctime>
#include <string>

#include "cryptopp/cryptlib.h"
#include "cryptopp/sha.h"

#include "block.pb.h"

using namespace CryptoPP;

class Block {
public:
    Block (std::string data, std::string prevBlockHash);
    Block (
        time_t timestamp,
        std::string data,
        std::string prevBlockHash,
        std::string hash,
        int nonce
    );

    // Block() {}
    void        setHash();
    std::string getPrevBlockHash();
    std::string getData();
    std::string getHash();

    // Serialization/Deserialization
    std::string serialize();
    std::string deserialize();

    // Instance Variables
    time_t      timestamp;
    std::string data;
    std::string prevBlockHash;
    std::string hash;
    int     nonce;
};

// Utility functions
std::string getHex(const std::string& input);
std::string serialize(Block* b);
Block deserialize(std::string& serialBlockString);

#endif
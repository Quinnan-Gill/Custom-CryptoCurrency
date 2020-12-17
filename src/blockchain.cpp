#include "blockchain.hpp"
#include "pow.hpp"

/*
 * The Block Code
 */
Block::Block(std::string data, std::string prevBlockHash) {
    this->timestamp     = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()
    );
    this->data          = data;
    this->prevBlockHash = prevBlockHash;
    
    // Determine the Hash
    ProofOfWork pow(*this);
    this->nonce = pow.run(this->hash);
}

void Block::setHash() {
    std::string timestamp_str = std::ctime(&timestamp);
    std::string headers;
    headers.append(prevBlockHash);
    headers.append(data);
    headers.append(timestamp_str);

    SHA256 sha256Hash;
    sha256Hash.Update((const byte*)headers.data(), headers.size());
    hash.resize(sha256Hash.DigestSize());
    sha256Hash.Final((byte*)&hash[0]);
}

std::string Block::getPrevBlockHash() {
    return getHex(prevBlockHash);
}

std::string Block::getData() {
    return getHex(data);
}

std::string Block::getHash() {
    return getHex(hash);
}

std::string getHex(const std::string& input) {
    // const char hex_digits[] = "0123456789ABCDEF";
    const char hex_digits[] = "0123456789abcdef";

    std::string output;
    output.reserve(output.length() * 2);
    for(unsigned char c : input) {

        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }

    return output;
}

/*
 * Blockchain
 */
BlockChain::BlockChain() {
    blocks.push_back(newGenesisBlock());
}

void BlockChain::addBlock(std::string data) {
    Block prevBlock = blocks.back();
    Block newBlock {data, prevBlock.hash};

    blocks.push_back(newBlock);
}

Block BlockChain::newGenesisBlock() {
    return Block {"Genesis Block", ""};
}
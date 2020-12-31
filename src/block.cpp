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

Block::Block(
    time_t timestamp,
    std::string data,
    std::string prevBlockHash,
    std::string hash,
    int nonce
) {
    this->timestamp     = timestamp;
    this->data          = data;
    this->prevBlockHash = prevBlockHash;
    this->hash          = hash;
    this->nonce         = nonce;
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

std::string BlockSerialize::serialize(Block* b) {
    CryptoProtobuf::Block s_block;
    std::string serialBlockString;

    s_block.set_timestamp(b->timestamp);
    s_block.set_data(b->data);
    s_block.set_prevblockhash(b->prevBlockHash);
    s_block.set_hash(b->hash);
    s_block.set_nonce(b->nonce);

    if (!s_block.SerializeToString(&serialBlockString)) {
        throw "Unable to Serialize Block";
    }

    return serialBlockString;
}

Block BlockSerialize::deserialize(std::string& serialBlockString) {
    CryptoProtobuf::Block d_block;
    if(!d_block.ParseFromString(serialBlockString)) {
        throw "Unable to Deserialize Block";
    }

    Block b{
        d_block.timestamp(),
        d_block.data(),
        d_block.prevblockhash(),
        d_block.hash(),
        d_block.nonce()
    };

    return b;
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
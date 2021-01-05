#include "pow.hpp"

ProofOfWork::ProofOfWork(Block& pow_block) {
    block = &pow_block;
    unsigned int shiftL = (unsigned int)(256 - targetBits);
    target = (mpz_class(1) << shiftL); 
}

std::string ProofOfWork::prepareData(int nonce) {
    std::string data;
    data.append(block->getPrevBlockHash());
    data.append(block->hashTransactions());
    data.append(intToHex(block->timestamp));
    data.append(intToHex(targetBits));
    data.append(intToHex((int64_t)nonce));

    return data;
}

int ProofOfWork::run(std::string& final_hash) {
    mpz_class hashInt;
    int nonce = 0;
    std::string hash;

    printf("Mining a new block\n");
    while (nonce < INT_MAX) {
        std::string data = prepareData(nonce);
        
        SHA256 sha256Hash;
        sha256Hash.Update((const byte*)data.data(), data.size());
        hash.resize(sha256Hash.DigestSize());
        sha256Hash.Final((byte*)&hash[0]);

        printf("\r%s", getHex(hash).c_str());
        hashInt.set_str(getHex(hash).c_str(), 16);

        if (cmp(hashInt, target) == -1) {
            break;
        } else {
            nonce++;
        }
    }
    printf("\n\n");

    block->hash = hash;

    return nonce;
}

bool ProofOfWork::validate() {
    mpz_class hashInt;
    std::string data;
    std::string hash;

    data = prepareData(block->nonce);

    SHA256 sha256Hash;
    sha256Hash.Update((const byte*)data.data(), data.size());
    hash.resize(sha256Hash.DigestSize());
    sha256Hash.Final((byte*)&hash[0]);

    hashInt.set_str(getHex(hash).c_str(), 16);
    
    std::string hashIntStr = hashInt.get_str(16);
    std::string targetStr = target.get_str(16);
    
    
    bool isValid = cmp(hashInt, target) == -1;
    
    return isValid;
}

std::string ProofOfWork::intToHex(int64_t val) {
    char hex_string[HEXBITS];
    std::memset(hex_string, 0, HEXBITS);
    sprintf(hex_string, "%lx", val);
    // return (std::string)&hex_string;
    std::string ret(hex_string, HEXBITS);
    return ret;
}
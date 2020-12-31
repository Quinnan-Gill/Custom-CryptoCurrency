#include "blockchain.hpp"

/*
 * Blockchain
 */
BlockChain::BlockChain() {
    // Create Database
    db = new CryptoDB();

    // Normal Execution
    leveldb::DB* blockBucket = db->getBucket(BLOCKS);

    leveldb::Status s = blockBucket->Get(
        leveldb::ReadOptions(),
        "l",
        &tip
    );

    if (s.IsNotFound()) {
        Block genesis = newGenesisBlock();
        s = blockBucket->Put(
            leveldb::WriteOptions(),
            genesis.hash,
            BlockSerialize::serialize(&genesis)
        );
        checkStatus(s);

        s = blockBucket->Put(
            leveldb::WriteOptions(),
            "l",
            genesis.hash
        );
        checkStatus(s);

        tip = genesis.hash;
    }
}

BlockChain::~BlockChain() {
    delete db;
}

void BlockChain::addBlock(std::string& data) {
    // Block prevBlock = blocks.back();
    // Block newBlock {data, prevBlock.hash};

    // blocks.push_back(newBlock);
    std::string lastHash;

    leveldb::DB* blockBucket = db->getBucket(BLOCKS);
    leveldb::Status s = blockBucket->Get(
        leveldb::ReadOptions(),
        "l",
        &lastHash
    );
    checkStatus(s);

    Block newBlock {data, lastHash};

    s = blockBucket->Put(
        leveldb::WriteOptions(),
        newBlock.hash,
        BlockSerialize::serialize(&newBlock)
    );
    checkStatus(s);

    s = blockBucket->Put(
        leveldb::WriteOptions(),
        "l",
        newBlock.hash
    );
    checkStatus(s);

    tip = newBlock.hash;
}

Block BlockChain::newGenesisBlock() {
    printf("No existing blockchain found. Creating a new one...\n");
    return Block {"Genesis Block", ""};
}
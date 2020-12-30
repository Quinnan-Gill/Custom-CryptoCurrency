#include "blockchain_iterator.hpp"

BlockchainIterator::BlockchainIterator(BlockChain* bc) {
    currentHash = bc->tip;
    db = bc->db;
}

Block BlockchainIterator::nextBlock() {
    std::string encodedBlock;

    leveldb::DB* blockBucket = db->getBucket(BLOCKS);

    leveldb::Status s = blockBucket->Get(
        leveldb::ReadOptions(),
        currentHash,
        &encodedBlock
    );
    
    Block currentBlock = deserialize(encodedBlock);

    currentHash = currentBlock.prevBlockHash;

    return currentBlock;
}
#include "pow.hpp"

/*
 * The Block Code
 */
Block::Block(std::vector<Transaction> transactions, std::string prevBlockHash) {    
    createBlock(transactions, prevBlockHash);
}

Block::Block(Transaction coinbase) {
    std::vector<Transaction> coinbaseVec;
    coinbaseVec.push_back(coinbase);
    // Block(coinbaseVec, "");
    createBlock(coinbaseVec, "");
}

void Block::createBlock(std::vector<Transaction> transactions, std::string prevBlockHash) {
    this->timestamp     = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now()
    );
    this->transactions  = transactions;
    this->prevBlockHash = prevBlockHash;
    
    // Determine the Hash
    ProofOfWork pow(*this);
    this->nonce = pow.run(this->hash);
}

Block::Block(CryptoProtobuf::Block* d_block) {
    this->timestamp = d_block->timestamp();
    for(int i=0; i < d_block->transactions_size(); i++) {
        CryptoProtobuf::Transaction d_tx = d_block->transactions(i);
        Transaction tx{&d_tx};
        transactions.push_back(tx);
    }
    this->prevBlockHash = d_block->prevblockhash();
    this->hash          = d_block->hash();
    this->nonce         = d_block->nonce();
}

void Block::loadBlock(CryptoProtobuf::Block* s_block) {
    s_block->set_timestamp(this->timestamp);
    for(Transaction tx : this->transactions) {
        tx.loadTransaction(s_block->add_transactions());
    }
    s_block->set_prevblockhash(this->prevBlockHash);
    s_block->set_hash(this->hash);
    s_block->set_nonce(this->nonce);
}

std::string Block::hashTransactions() {
    std::string txHashes;
    std::string txHash;

    for(Transaction tx : this->transactions) {
        txHashes.append(tx.id);
    }

    SHA256 sha256Hash;
    sha256Hash.Update((const byte*)txHashes.data(), txHashes.size());
    txHash.resize(sha256Hash.DigestSize());
    sha256Hash.Final((byte*)&txHash[0]);

    return getHex(txHash);    
}

std::string Block::getPrevBlockHash() {
    return getHex(prevBlockHash);
}

std::string Block::getHash() {
    return getHex(hash);
}

std::string BlockSerialize::serialize(Block* b) {
    CryptoProtobuf::Block s_block;
    std::string serialBlockString;

    // s_block.set_timestamp(b->timestamp);
    // s_block.set_data(b->data);
    // s_block.set_prevblockhash(b->prevBlockHash);
    // s_block.set_hash(b->hash);
    // s_block.set_nonce(b->nonce);
    b->loadBlock(&s_block);

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

    Block b{&d_block};

    return b;
}

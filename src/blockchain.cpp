#include "blockchain.hpp"

/*
 * Blockchain
 */
// Existing Blockchain
BlockChain::BlockChain() {
    // Create Database
    bool create_db = false;
    bool error_db  = false;
    db = new CryptoDB(create_db, error_db);

    // Normal Execution
    leveldb::DB* blockBucket = db->getBucket(BLOCKS);

    leveldb::Status s = blockBucket->Get(
        leveldb::ReadOptions(),
        "l",
        &tip
    );
    checkStatus(s);
}

// Create Blockchain
BlockChain::BlockChain(std::string address) {
    // Make sure it did not exist
    bool create_db = true;
    bool error_db  = true;
    db = new CryptoDB(create_db, error_db);

    leveldb::DB* blockBucket = db->getBucket(BLOCKS);

    CoinbaseTX cbtx {address, GENESIS_COINBASE_DATA};
    GenesisBlock genesis{cbtx};

    leveldb::Status s = blockBucket->Put(
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

BlockChain::~BlockChain() {
    delete db;
}

void BlockChain::mineBlock(std::vector<Transaction> transactions) {
    std::string lastHash;

    leveldb::DB* blockBucket = db->getBucket(BLOCKS);
    leveldb::Status s = blockBucket->Get(
        leveldb::ReadOptions(),
        "l",
        &lastHash
    );
    checkStatus(s);

    Block newBlock {transactions, lastHash};

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

std::vector<Transaction> BlockChain::findUnspentTransactions(std::string address) {
    std::vector<Transaction> unspentTXs;
    std::map<std::string, std::vector<int>> spentTXOs;
    BlockchainIterator bci{this};

    while (true) {
        Block block = bci.nextBlock();

        for(Transaction tx : block.transactions) {
            std::string txID = tx.id;

            for(int i=0; i < tx.vout.size(); i++) {
                if (spentTXOs.count(txID) != 0) {
                    if(loopOutputs(i, &(spentTXOs.at(txID))))
                        continue;
                }

                if (tx.vout.at(i).canBeUnlockedWith(address)) {
                    unspentTXs.push_back(tx);
                }
            }

            if (tx.isCoinbase() == false) {
                for (TXInput in : tx.vin) {
                    if (in.canUnlockOutputWith(address)) {
                        std::string inTxID = in.txid;
                        spentTXOs[inTxID].push_back(in.vout);
                    }
                }
            }
        }

        if (block.prevBlockHash.size() == 0) {
            break;
        }
    }

    return unspentTXs;
}

bool BlockChain::loopOutputs(int outIdx, std::vector<int>* spentTXOs) {
    for(int i=0; i < spentTXOs->size(); i++) {
        if (spentTXOs->at(i) == outIdx) {
            return true;
        }
    }
    return false;
}

std::vector<TXOutput> BlockChain::findUTXO(std::string address) {
    std::vector<TXOutput> UTXOS;
    std::vector<Transaction> unspentTransactions = findUnspentTransactions(address);

    for (Transaction tx : unspentTransactions) {
        for (TXOutput out : tx.vout) {
            if (out.canBeUnlockedWith(address)) {
                UTXOS.push_back(out);
            }
        }
    }

    return UTXOS;
}

int BlockChain::findSpendableOutputs(
    std::string address,
    int amount,
    std::map<std::string, std::vector<int>>* unspentOutputs
) {
    std::vector<Transaction> unspentTXs = findUnspentTransactions(address);
    int accumulated = 0;

    for (Transaction tx : unspentTXs) {
        std::string txID = tx.id;

        for (int i=0; i < tx.vout.size(); i++) {
            if (tx.vout.at(i).canBeUnlockedWith(address) && accumulated < amount) {
                accumulated += tx.vout.at(i).value;
                if (unspentOutputs->count(txID) == 0) {
                    std::vector<int> val;
                    val.push_back(i);
                    unspentOutputs->insert({txID, val});
                } else {
                    unspentOutputs->at(txID).push_back(i);
                }
                
                if (accumulated >= amount) {
                    return accumulated;
                }
            }
        }
    }

    return accumulated;
}

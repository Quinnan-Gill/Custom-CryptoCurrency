#ifndef CRYPTO_BLOCKCHAIN_HPP
#define CRYPTO_BLOCKCHAIN_HPP

#include <map>
#include <string>
#include <vector>
#include <iostream>

#include "cryptodb.hpp"
#include "block.hpp"
#include "transaction.hpp"
#include "blockchain_iterator.hpp"

#define GENESIS_COINBASE_DATA "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks"

class BlockChain {
public:
    // NewBlockchain
    // New BlockChain
    BlockChain ();
    BlockChain (std::string address);
    ~BlockChain();

    void mineBlock(std::vector<Transaction> transactions);
    std::vector<Transaction> findUnspentTransactions(std::string address);
    std::vector<TXOutput> findUTXO(std::string address);
    int findSpendableOutputs(
        std::string address,
        int amount,
        std::map<std::string, std::vector<int>>* unspentOutputs
    );
    Block newGenesisBlock();

    // Instance Variables
    std::string tip; // The hash of the last block
    CryptoDB* db;
private:
    bool loopOutputs(int outIdx, std::vector<int>* spentTXOs);
};

typedef BlockChain CreateBlockChain;

#endif
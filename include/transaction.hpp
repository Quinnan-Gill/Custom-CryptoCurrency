#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include <vector>

#include "cryptopp/cryptlib.h"
#include "cryptopp/sha.h"

#include "block.pb.h"

using namespace CryptoPP;

// Reward for mining
const int subsidy = 10;

/*
 * TXInput
 */
class TXInput {
public:
    TXInput(std::string txid, int vout, std::string scriptSig);
    TXInput(CryptoProtobuf::TXInput* d_txin);
    void loadTXInput(CryptoProtobuf::TXInput* s_txin);

    std::string txid;
    int         vout;
    std::string scriptSig;
};

namespace TXInputSerialize {
    std::string serialize(TXInput* txin);
    TXInput     deserialize(std::string& serialTXInput);
}

/*
 * TXOuput
 */
class TXOutput {
public:
    TXOutput(int value, std::string scriptPubKey);
    TXOutput(CryptoProtobuf::TXOutput* d_txout);
    void loadTXOutput(CryptoProtobuf::TXOutput* s_txout);

    int         value;
    std::string scriptPubKey;
};

namespace TXOutputSerialize {
    std::string serialize(TXOutput* txout);
    TXOutput    deserialize(std::string& serialTXOutput);
}

/*
 * Transaction
 */
class Transaction {
public:
    Transaction(std::string to, std::string data);
    Transaction(CryptoProtobuf::Transaction* d_trans);

    std::string           id;
    std::vector<TXInput>  vin;
    std::vector<TXOutput> vout;
private:
    void setID();
};

namespace TransactionSerialize {
    std::string serialize(Transaction* transaction);
    Transaction deserialize(std::string& serialTransaction);
}

#endif
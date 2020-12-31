#include "transaction.hpp"

/*
 * TXInput
 */
TXInput::TXInput(std::string txid, int vout, std::string scriptSig) {
    this->txid      = txid;
    this->vout      = vout;
    this->scriptSig = scriptSig;
}

TXInput::TXInput(CryptoProtobuf::TXInput* d_txin) {
    this->txid      = d_txin->txid();
    this->vout      = d_txin->vout();
    this->scriptSig = d_txin->scriptsig();
}

void TXInput::loadTXInput(CryptoProtobuf::TXInput* s_txin) {
    s_txin->set_txid(this->txid);
    s_txin->set_vout(this->vout);
    s_txin->set_scriptsig(this->scriptSig);
}

std::string TXInputSerialize::serialize(TXInput* txin) {
    CryptoProtobuf::TXInput s_txin;
    std::string serialTXInput;

    txin->loadTXInput(&s_txin);

    if (!s_txin.SerializeToString(&serialTXInput)) {
        throw "Unable to Serialize TXInput";
    }

    return serialTXInput;
}

TXInput TXInputSerialize::deserialize(std::string& serialTXInput) {
    CryptoProtobuf::TXInput d_txin;
    if(!d_txin.ParseFromString(serialTXInput)) {
        throw "Unable to Deserialize TXInput";
    }

    TXInput txin {&d_txin};

    return txin;
}


/*
 * TXOuput
 */
TXOutput::TXOutput(int value, std::string scriptPubKey) {
    this->value        = value;
    this->scriptPubKey = scriptPubKey;
}

TXOutput::TXOutput(CryptoProtobuf::TXOutput* d_txout) {
    this->value = d_txout->value();
    this->scriptPubKey = d_txout->scriptpubkey();
}

void TXOutput::loadTXOutput(CryptoProtobuf::TXOutput* s_txout) {
    s_txout->set_value(this->value);
    s_txout->set_scriptpubkey(this->scriptPubKey);
}

std::string TXOutputSerialize::serialize(TXOutput* txout) {
    CryptoProtobuf::TXOutput s_txout;
    std::string serialTXOutput;

    txout->loadTXOutput(&s_txout);

    if (!s_txout.SerializeToString(&serialTXOutput)) {
        throw "Unable to Serialize TXOutput";
    }

    return serialTXOutput;
}

TXOutput TXOutputSerialize::deserialize(std::string& serialTXOutput) {
    CryptoProtobuf::TXOutput d_txout;
    if(!d_txout.ParseFromString(serialTXOutput)) {
        throw "Unable to Deserialize TXInput";
    }

    TXOutput txout {&d_txout};

    return txout;
}

/*
 * Transaction
 */
Transaction::Transaction(std::string to, std::string data) {
    if (data.size() == 0) {
        data.reserve(10 + to.size());
        data.resize(std::max(
            0,
            sprintf((char*)data.c_str(), "Reward to %s", to.c_str())
        ));
    }

    TXInput  txin  {"", -1, data};
    TXOutput txout {subsidy, to};
    
    vin.push_back(txin);
    vout.push_back(txout);

    setID();
}

Transaction::Transaction(CryptoProtobuf::Transaction* d_trans) {
    this->id   = d_trans->id();
    for(int i=0; i < d_trans->vin_size(); i++) {
        CryptoProtobuf::TXInput d_txin = d_trans->vin(i);
        TXInput txin{&d_txin};
        vin.push_back(txin);
    }
    for(int i=0; i < d_trans->vout_size(); i++) {
        CryptoProtobuf::TXOutput d_txout = d_trans->vout(i);
        TXOutput txout{&d_txout};
        vout.push_back(txout);
    }
}

void Transaction::setID() {
    std::string hash;
    std::string encodeTrans = TransactionSerialize::serialize(this);
    
    SHA256 sha256Hash;
    sha256Hash.Update((const byte*)encodeTrans.data(), encodeTrans.size());
    hash.resize(sha256Hash.DigestSize());
    sha256Hash.Final((byte*)&hash[0]);

    this->id = hash;
}

std::string TransactionSerialize::serialize(Transaction* transaction) {
    CryptoProtobuf::Transaction s_transaction;
    std::string serialTransaction;

    s_transaction.set_id(transaction->id);
    for(TXInput txin : transaction->vin) {
        txin.loadTXInput(s_transaction.add_vin());
    }
    for(TXOutput txout : transaction->vout) {
        txout.loadTXOutput(s_transaction.add_vout());
    }

    if(!s_transaction.SerializeToString(&serialTransaction)) {
        throw "Unable to Serialize Transaction";
    }

    return serialTransaction;
}

Transaction TransactionSerialize::deserialize(std::string& serialTransaction) {
    CryptoProtobuf::Transaction d_transaction;
    if(!d_transaction.ParseFromString(serialTransaction)) {
        throw "Unable to Deserialize Transaction";
    }

    Transaction trans {&d_transaction};

    return trans;
}
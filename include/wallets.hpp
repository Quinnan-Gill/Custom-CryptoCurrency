#ifndef WALLETS_HPP
#define WALLETS_HPP

#include <string>
#include <map>
#include <iostream>
#include <sys/stat.h>
#include <fstream>

#include "wallet.hpp"

class Wallets {
public:
    Wallets();
    // Serialization functions
    Wallets(CryptoProtobuf::Wallets* d_wallets);
    void loadWallets(CryptoProtobuf::Wallets* s_wallets);

    // Wallet getters and setters
    std::string createWallet();
    std::vector<std::string> getAddresses();
    Wallet getWallet(std::string address);

    // Wallets load/save methods
    // std::string loadFromFile();
    void loadFromFile();
    void saveToFile();
private:
    std::map<std::string, Wallet> wallets;
private:
    void parseWallet(CryptoProtobuf::Wallets* d_wallets);
};

namespace WalletsSerialize {
    std::string serialize(Wallets* wallet);
    Wallets     deserialize(std::string& serialWallets);
}

#endif
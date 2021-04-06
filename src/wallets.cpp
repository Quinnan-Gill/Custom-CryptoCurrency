#include "wallets.hpp"

Wallets::Wallets() {
    loadFromFile();
}

Wallets::Wallets(CryptoProtobuf::Wallets* d_wallets) {
    parseWallet(d_wallets);
}

void Wallets::loadWallets(CryptoProtobuf::Wallets* s_wallets) {
    for(auto const& [address, wallet] : wallets) {
        CryptoProtobuf::Wallets::WalletMap* kv = s_wallets->add_map();
        kv->set_address(address);

        // CryptoProtobuf::Wallet* s_wallet = (CryptoProtobuf::Wallet*)&(kv->wallet());
        // ((Wallet) wallet).loadWallet(s_wallet);
        CryptoProtobuf::Wallet s_wallet;
        ((Wallet) wallet).loadWallet(&s_wallet);

        *(kv->mutable_wallet()) = s_wallet;
    }
}

void Wallets::parseWallet(CryptoProtobuf::Wallets* d_wallets) {
    for(int i=0; i < d_wallets->map_size(); i++) {
        const CryptoProtobuf::Wallets::WalletMap& kv = d_wallets->map(i);
        CryptoProtobuf::Wallet d_wallet = kv.wallet();
        Wallet wallet{&d_wallet}; 
        this->wallets[kv.address()] = wallet;
    }
}

std::string Wallets::createWallet() {
    Wallet newWallet;
    std::string address = newWallet.getAddress();

    wallets[address] = newWallet;

    return address;
}

Wallet Wallets::getWallet(std::string address) {
    return wallets[address];
}

std::vector<std::string> Wallets::getAddresses() {
    std::vector<std::string> addresses;
    for(auto const& [address, wallet] : wallets) {
        addresses.push_back(address);
    }

    return addresses;
}

void Wallets::loadFromFile() {
    std::fstream fileContent(WALLET_FILE, std::ios::in | std::ios::binary);
    if (!fileContent.good()) {
        // return "Could not access file\n";
        fprintf(stderr, "Could not access file\n");
        return;
    }

    CryptoProtobuf::Wallets d_wallets;
    if (!d_wallets.ParseFromIstream(&fileContent)) {
        fprintf(stderr, "Failed to parse wallet\n");
        exit(1);
    }

    parseWallet(&d_wallets);

    fileContent.close();
}

void Wallets::saveToFile() {
    std::fstream outputFile(WALLET_FILE, std::ios::out | std::ios::trunc | std::ios::binary);
    CryptoProtobuf::Wallets s_wallets;

    loadWallets(&s_wallets);

    if (!s_wallets.SerializeToOstream(&outputFile)) {
        fprintf(stderr, "Failed to output file\n");
        exit(1);
    }

    outputFile.close();
}

#ifndef WALLET_HPP
#define WALLET_HPP

#include <string>
#include <iostream>

#include "cryptopp/eccrypto.h"
#include "cryptopp/osrng.h"
#include "cryptopp/oids.h"
#include "cryptopp/filters.h"
#include "cryptopp/hex.h"

#include "wallets.pb.h"

#include "utils.hpp"

#define VERSION "\x00"
#define WALLET_FILE "wallet.dat"
#define ADDRESS_CHECKSUM_LEN 4

using namespace CryptoPP;
typedef StringSinkTemplate<std::string> StringSink;

class Wallet {
public:
    Wallet();
    // Serialize Helper Functions
    Wallet(CryptoProtobuf::Wallet* d_wallet);
    void loadWallet(CryptoProtobuf::Wallet* s_wallet);

    std::string getAddress();

    // Instance Variables
    ECDSA<ECP, SHA256>::PrivateKey privateKey;
    ECDSA<ECP, SHA256>::PublicKey publicKey;
private:
    void newKeyPair();
};

namespace WalletSerialize {
    std::string serialize(Wallet* wallet);
    Wallet     deserialize(std::string& serialWallet);
}

#endif
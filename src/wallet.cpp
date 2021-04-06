#include "wallet.hpp"

Wallet::Wallet() {
    newKeyPair();
}

Wallet::Wallet(CryptoProtobuf::Wallet* d_wallet) {
    std::string serializedPrivKey = d_wallet->privatekey();
    std::string serializedPubKey  = d_wallet->publickey();

    privateKey.BERDecodePrivateKey(
        StringSource(serializedPrivKey, true, NULL).Ref(),
        true, serializedPrivKey.size()
    );

    publicKey.BERDecode(
        StringSource(serializedPubKey, true, NULL).Ref()
    );
}

void Wallet::loadWallet(CryptoProtobuf::Wallet* s_wallet) {
    std::string encodedPrivateKey;
    std::string encodedPublicKey;

    StringSink privStringSink(encodedPrivateKey);
    StringSink pubStringSink(encodedPublicKey);

    privateKey.DEREncodePrivateKey(privStringSink.Ref());
    publicKey.DEREncode(pubStringSink.Ref());

    s_wallet->set_privatekey(encodedPrivateKey);
    s_wallet->set_publickey(encodedPublicKey);
}

void Wallet::newKeyPair() {
    AutoSeededRandomPool prng;

    privateKey.Initialize(prng, ASN1::secp256r1());
    privateKey.MakePublicKey(publicKey);
}

std::string Wallet::getAddress() {
    std::string encodedPublicKey;
    publicKey.Save(StringSink(encodedPublicKey).Ref());

    std::string pubKeyHash = hashPubKey(encodedPublicKey);

    std::string versionedPayload (VERSION);
    versionedPayload.append(pubKeyHash);
    std::string checksumVal = checksum(versionedPayload);

    std::string fullPayload = versionedPayload;
    fullPayload.append(checksumVal);
    std::string address = base58Encode(fullPayload);

    return address;
}

/*
 * Serialization functions
 */
std::string WalletSerialize::serialize(Wallet* wallet) {
    CryptoProtobuf::Wallet s_wallet;
    std::string serialWallet;

    wallet->loadWallet(&s_wallet);

    if(!s_wallet.SerializeToString(&serialWallet)) {
        throw "Unable to Serialize Wallet";
    }

    return serialWallet;
}

Wallet WalletSerialize::deserialize(std::string& serialWallet) {
    CryptoProtobuf::Wallet d_wallet;
    if(!d_wallet.ParseFromString(serialWallet)) {
        throw "Unable to Deserialize Wallet";
    }

    Wallet wallet {&d_wallet};

    return wallet;
}
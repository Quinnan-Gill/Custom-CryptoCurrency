#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <openssl/ripemd.h>

#include "cryptopp/sha.h"
#include "cryptopp/integer.h"
#include "cryptopp/modarith.h"

#define BASE_COUNT 58

using namespace CryptoPP;

static const char alphabet[BASE_COUNT] = {
    '1','2','3','4','5','6','7','8','9','A','B','C','D','E','F',
    'G','H','J','K','L','M','N','P','Q','R','S','T','U','V','W',
    'X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','m',
    'n','o','p','q','r','s','t','u','v','w','x','y','z'
};


// Utility functions
std::string getHex(const std::string& input);
std::string hashPubKey(std::string& encodedPublicKey);
std::string checksum(std::string& payload);

std::string base58Encode(std::string& input);
std::string base58Decode(std::string& input);
int getCharIndex(char val);
std::string reverseBytes(std::string& rev);

#endif
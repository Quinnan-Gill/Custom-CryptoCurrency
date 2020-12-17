#include <stdio.h>
#include <string>

#include "cryptopp/cryptlib.h"
#include "cryptopp/sha.h"

#define HEXBITS 16

using namespace CryptoPP;

std::string getHex(const std::string& input) {
    // const char hex_digits[] = "0123456789ABCDEF";
    const char hex_digits[] = "0123456789abcdef";

    std::string output;
    output.reserve(output.length() * 2);
    for(unsigned char c : input) {

        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }

    return output;
}

void test1() {
    std::string data = "Block5fdac2ea";
    std::string hash;

    SHA256 sha256Hash;
    sha256Hash.Update((const byte*)data.data(), data.size());
    hash.resize(sha256Hash.DigestSize());
    sha256Hash.Final((byte*)&hash[0]);

    printf("------Before set_str: %s\n", getHex(hash).c_str());
}

void test2() {
    std::string data = "Block5fdac2ea";
    std::string hash;

    SHA256 sha256Hash;
    sha256Hash.Update((const byte*)data.data(), data.size());
    hash.resize(sha256Hash.DigestSize());
    sha256Hash.Final((byte*)&hash[0]);

    printf("------Before set_str: %s\n", getHex(hash).c_str());
}

void test3() {
    int64_t val = 123214235342523;

    char hex_string[HEXBITS];
    sprintf(hex_string, "%lx", val);
    // return (std::string)&hex_string;
    std::string ret(hex_string, HEXBITS);
    printf("Result: %s\n", ret.c_str());
}

int main() {
    test1();
    test2();
    test3();
    return 0;
}
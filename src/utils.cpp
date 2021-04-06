#include "utils.hpp"

/*
 * Utility functions
 */
/*
 * Converts input into hex values
 */
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

std::string hashPubKey(std::string& encodedPublicKey) {
    std::string hash;
    SHA256 sha256Hash;
    sha256Hash.Update((const byte*)encodedPublicKey.data(), encodedPublicKey.size());
    hash.resize(sha256Hash.DigestSize());
    sha256Hash.Final((byte*)&hash[0]);

    RIPEMD160_CTX ripemd;
    unsigned char md[32];
    unsigned char* pmd = RIPEMD160((const unsigned char*) hash.c_str(), hash.size(), md);
    std::string publicRIPEMD160 ((char *) pmd);

    return publicRIPEMD160;
}

std::string checksum(std::string& payload) {
    std::string hash;

    SHA256 sha256Hash1;
    sha256Hash1.Update((const byte*)payload.data(), payload.size());
    hash.resize(sha256Hash1.DigestSize());
    sha256Hash1.Final((byte *)&hash[0]);

    SHA256 sha256Hash2;
    sha256Hash2.Update((const byte*)hash.data(), hash.size());
    sha256Hash2.Final((byte *)&hash[0]);

    return hash;
}

std::string base58Encode(std::string& input) {
    Integer x((const byte*)input.c_str(), input.size());
    Integer base(BASE_COUNT);
    Integer zero, mod;
    std::string result;

    while (x.Compare(zero) != 0) {
        mod = x % base;
        x = x / base;
        std::string encodeByte{alphabet[mod.ConvertToLong()]};
        result.append(encodeByte);
    }

    result = reverseBytes(result);
    // Add for '\x00' null byte
    result = alphabet[0] + result;

    return result;
}

std::string base58Decode(std::string& input) {
    Integer result;
    Integer fiftyEight(BASE_COUNT);

    for(std::string::iterator it=input.begin(); it!=input.end(); ++it) {
        int charIndex = getCharIndex(*it);

        if (charIndex != 0) {
            Integer charIntegerIndex(charIndex);
            result *= fiftyEight;
            result += charIntegerIndex;
        }
    }

    std::string str_result;
    size_t resultByteCount = result.ByteCount();
    for(size_t i=0; i < resultByteCount; i++) {
        std::string resultByte{(char)result.GetByte(i)};
        str_result.append(resultByte);
    }

    return reverseBytes(str_result);
}

int getCharIndex(char val) {
    for(int i=0; i < BASE_COUNT; i++) {
        if (val == alphabet[i]) {
            return i;
        }
    }

    return -1;
}

std::string reverseBytes(std::string& rev) {
    int j = rev.size()-1;
    for(int i=0; i < j; i++) {
        char temp = rev[i];
        rev[i] = rev[j];
        rev[j] = temp;
        j--;
    }

    return rev;
}
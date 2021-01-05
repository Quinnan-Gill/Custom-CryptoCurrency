#include "utils.hpp"

/*
 * Utility functions
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
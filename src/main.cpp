#include <iostream>
#include <cstring>
#include <chrono>
#include <ctime>

#include "cryptopp/cryptlib.h"
#include "cryptopp/sha.h"
#include "cryptopp/hex.h"
#include "cryptopp/files.h"

#include "block.hpp"
#include "blockchain.hpp"
#include "pow.hpp"
#include "cli.hpp"

int main(int argc, char *argv[]) {
    // Initialize Protobuf
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    CLI cli {};
    cli.run(argc, argv);

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}

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

    BlockChain bc;

    // bc.addBlock("Send 1 BTC to Ivan");
    // bc.addBlock("Send 2 BTC to Ivan");

    // for(Block block : bc.blocks) {
    //     printf("Prev. hash: %s\n", block.getPrevBlockHash().c_str());
    //     printf("Data: %s\n", block.data.c_str());
    //     printf("Hash: %s\n", block.getHash().c_str());
    //     printf("Nonce: %d\n", block.nonce);

    //     ProofOfWork pow{block};
    //     printf("PoW: %s\n", pow.validate() ? "true" : "false");
    //     printf("\n");
    // }

    CLI cli {&bc};
    cli.run(argc, argv);

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}


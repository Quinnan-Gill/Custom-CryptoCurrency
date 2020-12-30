#include "cli.hpp"

CLI::CLI(BlockChain* bc) {
    this->bc = bc;
}

void CLI::run(int argc, char *argv[]) {
    if (argc <= 1) {
        usage(argv);
        exit(1);
    }

    if(strcmp("addBlock", argv[1]) == 0 && argc == 3) {
        std::string data{argv[2]};
        addBlock(data);
    } else if(strcmp("printchain", argv[1]) == 0) {
        printChain();
    } else {
        usage(argv);
    }
}

void CLI::addBlock(std::string& data) {
    bc->addBlock(data);
    printf("Success!\n\n");
}

void CLI::printChain() {
    BlockchainIterator bci {bc};
    
    while (true) {
        Block block = bci.nextBlock();

        printf("Prev. hash: %s\n", block.getPrevBlockHash().c_str());
        printf("Data: %s\n", block.data.c_str());
        printf("Hash: %s\n", block.getHash().c_str());
        printf("Nonce: %d\n", block.nonce);

        ProofOfWork pow{block};
        printf("PoW: %s\n", pow.validate() ? "true" : "false");
        printf("\n");

        if (block.prevBlockHash.size() == 0) {
            break;
        }
    }
}

void CLI::usage(char *argv[]) {
    printf("Usage: %s [CMD] [DATA]\n", argv[0]);
    printf("\t- addBlock   This adds the following data to the block chain\n");
    printf("\t- printchain Display history of the block chain\n");
}
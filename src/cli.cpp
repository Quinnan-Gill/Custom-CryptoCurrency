#include "cli.hpp"

CLI::CLI() {}

void CLI::run(int argc, char *argv[]) {
    if (argc <= 1) {
        usage(argv);
        exit(1);
    }

    if(strcmp("getbalance", argv[1]) == 0 && argc == 4) {
        std::string address;
        parseCreateBlockchain(argv, &address);
        getBalance(address);
    } else if(strcmp("createblockchain", argv[1]) == 0 && argc == 4) {
        std::string address;
        parseCreateBlockchain(argv, &address);
        createBlockchain(address);
    } else if(strcmp("printchain", argv[1]) == 0) {
        printChain();
    } else if(strcmp("send", argv[1]) == 0 && argc == SEND_ARGC) {
        std::string from;
        std::string to;
        int amount = parseSend(argv, &from, &to);
        send(from, to, amount);
    } else {
        usage(argv);
    }
}

void CLI::usage(char *argv[]) {
    printf("Usage:\n");
    printf("  getbalance -address ADDRESS - Get balance of ADDRESS\n");
    printf("  createblockchain -address ADDRESS - Create a blockchain and send genesis block reward to ADDRESS\n");
    printf("  printchain - Print all the blocks of the blockchain\n");
    printf("  send -from FROM -to TO -amount AMOUNT - Send AMOUNT of coins from FROM address to TO\n");
}

/*
 * Create Blockchain
 */
void CLI::parseCreateBlockchain(char *argv[], std::string* address) {
    if(strcmp("-address", argv[2]) == 0) {
        address->append(argv[3]);
    } else {
        usage(argv);
        exit(1);
    }
}

void CLI::createBlockchain(std::string address) {
    CreateBlockChain bc {address};
    printf("Done!\n");
}

/*
 * Get Balance
 */
void CLI::parseGetBalance(char *argv[], std::string* address) {
    if(strcmp("-address", argv[2]) == 0) {
        address->append(argv[3]);
    } else {
        usage(argv);
    }
}

void CLI::getBalance(std::string address) {
    BlockChain bc;

    int balance = 0;
    std::vector<TXOutput> utxos = bc.findUTXO(address);

    for(TXOutput out : utxos) {
        balance += out.value;
    }

    printf("Balance of '%s': %d\n", address.c_str(), balance);
}

/*
 * Print chain
 */
void CLI::printChain() {
    BlockChain bc;

    BlockchainIterator bci {&bc};
    
    while (true) {
        Block block = bci.nextBlock();

        printf("Prev. hash: %s\n", block.getPrevBlockHash().c_str());
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

/*
 * Send
 */
int CLI::parseSend(char *argv[], std::string* from, std::string* to) {
    int amount;
    uint8_t count = 0;
    for(int i=2; i < SEND_ARGC-1; i+=2) {
        if(strcmp("-from", argv[i]) == 0) {
            from->append(argv[i+1]);
            count |= 1;
        } else if(strcmp("-to", argv[i]) == 0) {
            to->append(argv[i+1]);
            count |= 2;
        } else if(strcmp("-amount", argv[i]) == 0) {
            amount = atoi(argv[i+1]);
            count |= 4;
        }
    }


    if (count != 0x7) {
        usage(argv);
        exit(1);
    }

    return amount;
}

void CLI::send(std::string from, std::string to, int amount) {
    BlockChain bc{};

    Transaction tx{from, to, amount, &bc};
    std::vector<Transaction> txVec;
    txVec.push_back(tx);

    bc.mineBlock(txVec);
    printf("Success!\n");
}
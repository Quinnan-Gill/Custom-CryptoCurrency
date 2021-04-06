#ifndef CLI_HPP
#define CLI_HPP

#include "blockchain.hpp"
#include "blockchain_iterator.hpp"
#include "pow.hpp"
#include "wallets.hpp"

#define SEND_ARGC 8

class CLI {
public:
    CLI();   
    void run(int argc, char *argv[]); 

private:
    void usage(char *argv[]);

    void parseCreateBlockchain(char *argv[], std::string* address);
    void createBlockchain(std::string address);

    void createWallet();

    void parseGetBalance(char *argv[], std::string* address);
    void getBalance(std::string address);

    void printChain();

    int parseSend(char *argv[], std::string* from, std::string* to);
    void send(std::string from, std::string to, int amount);
};

#endif
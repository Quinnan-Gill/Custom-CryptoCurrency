#ifndef CLI_HPP
#define CLI_HPP

#include "blockchain.hpp"
#include "blockchain_iterator.hpp"
#include "pow.hpp"

class CLI {
public:
    CLI(BlockChain* bc);   
    void run(int argc, char *argv[]); 

private:
    BlockChain* bc;

    void usage(char *argv[]);
    void addBlock(std::string& data);
    void printChain();
};

#endif
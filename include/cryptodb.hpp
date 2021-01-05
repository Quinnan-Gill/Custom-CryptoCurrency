#ifndef CRYPTO_DB_HPP
#define CRYPTO_DB_HPP

#include <iostream>
#include <string>
#include <filesystem>

#include "leveldb/db.h"

#define DB_DIR            "./cryptodb/"
#define BLOCK_BUCKET      "block_bucket"
#define CHAINSTATE_BUCKET "chainstate_bucket"

enum Bucket {
    BLOCKS,
    CHAINSTATE
};

class CryptoDB {
public:
    CryptoDB(bool create, bool error);

    ~CryptoDB();

    leveldb::DB* getBucket(Bucket bucketOption);
private:
    leveldb::DB* blockBucket = nullptr;
    // leveldb::DB* chainstateBucket = nullptr;
    std::string dbPath;

    void createDirectory(const char* path);
};

void checkStatus(leveldb::Status s);

#endif
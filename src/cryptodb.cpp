#include "cryptodb.hpp"

CryptoDB::CryptoDB() {
    leveldb::Options options;
    options.create_if_missing = true;

    dbPath = DB_DIR;
    createDirectory(dbPath.c_str());

    leveldb::Status status = leveldb::DB::Open(
        options,
        dbPath.append(BLOCK_BUCKET).c_str(),
        &blockBucket
    );
    if (!status.ok()) {
        std::cerr << status.ToString() << std::endl;
        exit(1);
    }

    status = leveldb::DB::Open(
        options,
        dbPath.append(CHAINSTATE_BUCKET).c_str(),
        &chainstateBucket
    );
    if (!status.ok()) {
        std::cerr << status.ToString() << std::endl;
        exit(1);
    }
}

CryptoDB::~CryptoDB() {
    delete blockBucket;
    delete chainstateBucket;
}

leveldb::DB* CryptoDB::getBucket(Bucket bucketOption) {
    switch(bucketOption) {
    case BLOCKS:
        return blockBucket;
    case CHAINSTATE:
        return chainstateBucket;
    default:
        return nullptr;
    }
}

void CryptoDB::createDirectory(const char* path) {
    std::filesystem::create_directories(path);
}

void checkStatus(leveldb::Status s) {
    if (!s.ok()) {
        std::cerr << s.ToString() << std::endl;
        exit(1);
    }
}
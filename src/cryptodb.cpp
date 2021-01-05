#include "cryptodb.hpp"

CryptoDB::CryptoDB(bool create, bool error) {
    leveldb::Options options;
    options.create_if_missing = create;
    options.error_if_exists = error;

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
    // if (!status.ok() && error) {
    //     std::cerr << "Block already exists." << std::endl;
    //     exit(1);
    // } else if (!status.ok() && !create) {
    //     std::cerr << status.ToString() << std::endl;
    //     std::cerr << "No existing blockchain found. Create one first." << std::endl;
    //     exit(1);
    // }
}

CryptoDB::~CryptoDB() {
    delete blockBucket;
    // delete chainstateBucket;
}

leveldb::DB* CryptoDB::getBucket(Bucket bucketOption) {
    switch(bucketOption) {
    case BLOCKS:
        return blockBucket;
    // case CHAINSTATE:
    //     return chainstateBucket;
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
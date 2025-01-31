#pragma once
#ifndef SHA512_H
#define SHA512_H
#include <openssl/sha.h>
#include "hash.h"

class sha512 : public Hash{
    /*
    this class is a hash function that implements the abstract class Hash
    it uses the openssl library to perform a sha512
    */
public:
    int getHashSize() const noexcept{return SHA512_DIGEST_LENGTH;}  //returns the length of the sha512 (64 byte)
    Bytes hash(const Bytes bytes) const;            //performs the sha512 on a Bytes object
    Bytes hash(const std::string str) const;        //performs the sha512 on a string
};

#endif //SHA512_H
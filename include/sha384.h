#include <openssl/sha.h>
#include "hash.h"

class sha384 : Hash{
public:
    int getHashSize() const noexcept{return SHA384_DIGEST_LENGTH;}
    Bytes hash(Bytes bytes);
    Bytes hash(std::string str);
};
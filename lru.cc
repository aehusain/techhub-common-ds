#include "lru.h"
#include <unordered_map>

const int
bucket_fn(const int &value)
{
    // let the std library deal with hashing funciton
    return value;
}

int
main(int argc, char *argv[])
{
    LRU<int, int, std::unordered_map> lru(bucket_fn, 10);
    
    int i = 0;
    for (; i < 5; ++i) {
        lru(i);
    }

    lru.display_keys();
    lru(i);
    lru(++i);

    for (; i < 15; ++i) {
        lru(i);
    }

    std::cout << std::endl;

    lru.display_keys();
    return 0;
}


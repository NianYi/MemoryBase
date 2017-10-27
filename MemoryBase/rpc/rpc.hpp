#ifndef RPC_HPP
#define RPC_HPP
#include <cstring>

#include "../include/utility.hpp"

namespace MemoryBase{
class Rpc : private NoCopy{
public:
    static uint32_t Hash(const char *str){
        uint32_t val = 0;
        uint32_t len = strlen(str);
        for(int i=0; i<len; ++i){
            val += uint32_t(str[i]) * (len - i);
        }
        return val;
    }
private:
}
}



#endif

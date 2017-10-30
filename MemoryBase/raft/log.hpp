/*
Author  :   manuelyuan
Desc    :   define class Log
*/
#ifndef LOG_HPP
#define LOG_HPP

#include "../rpc/marshaller.hpp"

namespace MemoryBase{
struct Log{
    Log(){}
    Log(uint32_t _term, uint32_t _number) : term(_term), number(_number){}

    uint32_t term;
    uint32_t number;
};
inline Marshaller& operator<<(Marshaller &mar, const Log &log){
    mar << log.term;
    mar << log.number;
    return mar;
}
inline Marshaller& operator<<(Marshaller &mar, Log &log){
    mar >> log.term;
    mar >> log.number;
    return mar;
}

}
#endif

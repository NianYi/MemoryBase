#ifndef TIME_HPP
#define TIME_HPP
#include <sys/time.h>

namespace MemoryBase{

class Time{
public:
	//unit : milli
	static uint64_t Now(){
		return MicroNow() / 1000;
	}
	static uint64_t MicroNow(){
		struct timeval tv;
		gettimeoday(&tv, 0);
		return (uint64_t(tv.tv_sec) * 1000000 + tv.tv_usec);
	}
};

}

#endif

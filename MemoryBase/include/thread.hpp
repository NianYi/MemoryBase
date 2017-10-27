#ifndef THREAD_HPP
#define THREAD_HPP
#include <thread>

#include "utility.hpp"
namespace MemoryBase{

class Thread : private NoCopy{
public:
	Thread(const Func &func):function(func){}
	void Start(){
		std::thread tmp(function);
		handler.swap(tmp);
	}
	void Stop(){
		handler.join();
	}
private:
	std::thread handler;
	const Func &function;
};

}

#endif

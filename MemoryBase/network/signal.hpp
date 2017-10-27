#ifndef SIGNAL_HPP
#define SIGNAL_HPP
#include <csignal>
#include <unordered_map>

#include "../include/utility.hpp"

namespace MemoryBase{
class Signal{
public:
	static void Register(int sig, const Func &func){
		handlers[sig] = func;
		signal(sig, Signal_Handle);
	}
	void Signal_Handle(int sig){
		handlers[sig]();
	}
private:
	static std::unordered_map<int, Func> handlers;
};
std::unordered_map<int, Func> Signal::handlers;	
}
#endif

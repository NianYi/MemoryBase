#ifndef ATOMIC_HPP
#define ATOMIC_HPP
#include <cstdint>

#include "utility.hpp"
namespace MemoryBase{

template<typename T>
class Atomic : private NoCopy{
public:
	Atomic(){};
	Atomic(const T &val) : value(val){};
	T get(){
		return __sync_val_compare_and_swap(&value, 0, 0);
	};
	T operator++(){
		return __sync_add_and_fetch(&value, 1);
	};
	T operator--(){
		return __sync_sub_and_fetch(&value, 1);
	};
	T operator++(int){
		return __sync_fetch_and_add(&value, 1);
	};
	T operator--(int){
		return __sync_fetch_and_sub(&value, 1);
	};
	Atomic& operator=(const T &val){
		assert(__sync_bool_compare_and_swap(&value, value, val));
		return *this;
	};
private:
	T value;
};

typedef Atomic<uint8_t> atomic_8_t;
typedef Atomic<uint16_t> atomic_16_t;
typedef Atomic<uint32_t> atomic_32_t;

}
#endif

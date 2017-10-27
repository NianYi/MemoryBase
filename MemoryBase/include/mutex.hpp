#ifndef MUTEX_HPP
#define MUTEX_HPP
#include <pthread.h>
#include <cassert>

#include "utility.hpp"
namespace MemoryBase{

class Cond;

class Mutex : private NoCopy{
friend class Cond;
public:
	Mutex(){
		assert(!pthread_mutex_init(&mutex, NULL));
	}
	void Lock(){
		pthread_mutex_lock(&mutex);
	}
	void Unlock(){
		pthread_mutex_unlock(&mutex);
	}
	~Mutex(){
		assert(!pthread_mutex_destroy(&mutex));
	}
private:
	pthread_mutex_t mutex;
};

}
#endif

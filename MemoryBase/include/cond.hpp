#ifndef COND_HPP
#define COND_HPP
#include <sys/time.h>

#include "mutex.hpp"
namespace MemoryBase{

class Cond : private NoCopy{
public:
	Cond(){
		assert(!pthread_cond_init(&cond, NULL));
	}
	void Wait(Mutex &mut){
		pthread_cond_wait(&cond, &mut.mutex);
	}
	void Signal(){
		pthread_cond_signal(&cond);
	}
	void BroadCast(){
		pthread_cond_broadcast(&cond);
	}
	bool TimedWait(Mutex &mut, uint64_t milliseconds){
		struct timeval now;
		struct timespec tsp;
		gettimeofday(&now, NULL);
		tsp.tv_sec = now.tv_sec;
		tsp.tv_nsec = now.tv_usec * 1000;

		tsp.tv_sec += milliseconds / 1000;
		tsp.tv_nsec += (milliseconds % 1000) * 1000000;
		return pthread_cond_timedwait(&cond, &mut.mutex, &tsp) == ETIMEDOUT;
	}
	~Cond(){
		assert(!pthread_cond_destroy(&cond));
	}
private:
	pthread_cond_t cond;
};

}
#endif

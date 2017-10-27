#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include "atomic.hpp"
#include "thread.hpp"
#include "utility.hpp"
#include "bounded_queue.hpp"
namespace MemoryBase{

template<typename T>
class ThreadPool : private NoCopyTemplate<T>{
public:
	ThreadPool(uint32_t tnum, BoundedQueue<T> *que);
	void Run();
	void Clear();
	~ThreadPool();
private:
	BoundedQueue<T> *queue;
	Thread **threads;
	
	uint32_t threads_num;
	atomic_8_t runing;
};
template<typename T>
ThreadPool<T>::ThreadPool(uint32_t tnum, BoundedQueue<T> *que):threads_num(tnum), queue(que){
	if(threads_num<=1) threads_num=1;
	if(threads_num>=8) threads_num=8;
	threads = new Thread*[threads_num];
	for(int i=0; i<threads_num; ++i){
		threads[i] = new Thread([this](){
			this->Run();
		});
		threads[i]->Start();
	}
	runing=1;
}
template<typename T>
void ThreadPool<T>::Run(){
	while(true){
		T task = queue->Pop();
		if(!runing.get()) break;
		task();
	}
}
template<typename T>
void ThreadPool<T>::Clear(){
	if(!runing.get()) return;
	runing = 0;
	queue->Clear();
	for(int i=0; i<threads_num; ++i){
		threads[i]->Stop();
	}
}
template<typename T>
ThreadPool<T>::~ThreadPool(){
	for(int i=0; i<threads_num; ++i){
		delete threads[i];
	}
	delete []threads;
}

}
#endif

#ifndef BOUNDED_QUEUE_HPP
#define BOUNDED_QUEUE_HPP
#include "cond.hpp"
#include "mutex.hpp"
#include "utility.hpp"
namespace MemoryBase{

template<typename T>
class BoundedQueue : private NoCopyTemplate<T>{
public:
	BoundedQueue(int len=0):length(len),begin(0),end(0),clear(false){
		if(length<=0) length=8;
		if(length>1024) length=1024;
		queue=new T[length];
	}
	void Push(T &&in){
		mutex.Lock();
		while((end+1)%length==begin)
			full.Wait(mutex);
		queue[end] = in;
		if(++end == length) end=0;
		mutex.Unlock();
		empty.Signal();
	}
	T Pop(){
		mutex.Lock();
		while(!clear && begin==end)
			empty.Wait(mutex);
		if(clear) {
			mutex.Unlock();
			return T();
		}
		T ret = std::move(queue[begin]);
		if(++begin == length) begin=0; 
		mutex.Unlock();
		full.Signal();
		return ret;
	}
	void Clear(){
		mutex.Lock();
		if(clear){
			mutex.Unlock();
			return;
		}
		clear = true;
		empty.BroadCast();
		mutex.Unlock();
	}
	bool Empty(){
		return begin == end;
	}
	~BoundedQueue(){
		delete []queue;
	}
private:
	int length;
	int begin;
	int end;
	bool clear;
	T *queue;
	
	Cond empty;
	Cond full;
	Mutex mutex;
};

}
#endif

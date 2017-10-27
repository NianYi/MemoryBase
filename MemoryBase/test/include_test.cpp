/**
	The file test include directory components
*/
#include <unistd.h>

#include "unit.h"
#include "../include/thread.hpp"
#include "../include/utility.hpp"
#include "../include/atomic.hpp"
#include "../include/bounded_queue.hpp"
#include "../include/thread_pool.hpp"
using namespace MemoryBase;

template<typename T>
static inline void Atomic_Unit(){
	T atom;
	atom = 10;
	ASSERT_TRUE(atom.get() == 10);
	ASSERT_TRUE(atom++ == 10);
	ASSERT_TRUE(atom-- == 11);
	ASSERT_TRUE(++atom == 11);
	ASSERT_TRUE(--atom == 10);
}

TEST(ATOMIC_UNIT){
	Atomic_Unit<atomic_8_t>();
	Atomic_Unit<atomic_16_t>();
	Atomic_Unit<atomic_32_t>();
}
TEST(ATOMIC_THREAD_UNIT){
	Thread **threads = new Thread*[8];
	atomic_16_t atom;
	atom = 10;
	for(int i=0; i<8; i++){
		threads[i] = new Thread([&atom]{
			atom++;
			atom--;
			++atom;
			atom--;
		});
		threads[i]->Start();
	}
	for(int i=0; i<8; ++i){
		threads[i]->Stop();
		delete threads[i];
	}
	ASSERT_TRUE(atom.get() == 10);
	delete []threads;
}
TEST(THREAD_POOL_UNIT){
	BoundedQueue<Task> *que = new BoundedQueue<Task>(8);
	ThreadPool<Task> *thread_pool = new ThreadPool<Task>(4, que);
	que->Push([&que]{
		for(int i=0; i<20; ++i){
			que->Push(std::move([i](){
				printf("push queue id : %d\n", i);
			}));
		}
	});
	sleep(3);
	while(!que->Empty());
	thread_pool->Clear();
	que->Clear();
	delete que;
	delete thread_pool;
}
TEST(COND_UNIT){
	Cond cond;
	Mutex mutex;
	int a=0;
	Thread thread1([&]{
		mutex.Lock();
		while(a!=10) cond.Wait(mutex);
		printf("cond success : a = 10\n");
		mutex.Unlock();
	});
	Thread thread2([&]{
		mutex.Lock();
		while(a!=10){
			a++;
			printf("%d",a);
		}
		mutex.Unlock();
		cond.Signal();
	});
	thread1.Start();
	thread2.Start();
	thread1.Stop();
	thread2.Stop();
}
int main(){
	RUN_ALL_TESTS("");
}

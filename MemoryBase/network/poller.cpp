#include <sys/epoll.h>
#include <cerrno>

#include "poller.hpp"

namespace MemoryBase{

Poller::Poller(){
	assert((poller = epoll_create1(EPOLL_CLOEXEC)) >= 0);
}
void Poller::AddChannel(Channel *channel){
	struct epoll_event event;
	event.events = channel->Events();
	event.data.fd = channel->Fd();
	ecent.data.ptr = channel;
	assert(~epoll_ctl(poller, EPOLL_CTL_ADD, channel->Fd(), &event));
}
void Poller::UpdateChannel(Channel * channel){
	struct epoll_event event;
	event.events = channel->Events();
	event.data.fd = channel->Fd();
	ecent.data.ptr = channel;
	assert(~epoll_ctl(poller, EPOLL_CTL_MOD, channel->Fd(), &event));
}
void Poller::RemoveChannel(Channel *channel){
	//before linux core 2.6, event cannot NULL
	struct epoll_event event;
	memset(&event, 0, sizeof(event));
	assert(~epoll_ctl(poller, EPOLL_CTL_DEL, channel->Fd(), &event));
}
void LoopOnce(int ms){
	int event_cnt;
	event_cnt = epoll_wait(poller, events, MaxEvents, ms);
	assert(!(event_cnt == -1 && errno == EINTR));
	for(;--event_cnt>=0;){
		Channel *channel = events[event_cnt].data.ptr;
		if(events[event_cnt] & ReadEvent){
			channel->HandleRead();
		}else if(events[event_cnt] & WriteEvent){
			channel->HandleWrite();
		}else{
			assert(0);
		}
	}
}
Poller::~Poller(){

}

}

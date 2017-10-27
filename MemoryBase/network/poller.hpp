#ifndef POLLER_HPP
#define POLLER_HPP
#include <sys/epoll.h>
namespace MemoryBase{
const uint32_t ReadEvent = EPOLLIN;
const uint32_t WriteEvent = EPOLLOUT;
const uint32_t MaxEvents = 1024;

class Channel;
class Poller : private NoCopy{
public:
	Poller();
	void AddChannel(Channel *);
	void UpdateChannel(Channel *);
	void RemoveChannel(Channel *);
	void LoopOnce(int ms);
	~Poller();
private:
	int poller;
	struct epoll_event events[MaxEvents];
}
}

#endif

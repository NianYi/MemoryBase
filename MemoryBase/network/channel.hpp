#ifndef CHANNEL_HPP
#define CHANNEL_HPP
#include "connection.hpp"
#include "poller.hpp"
#include "callback.hpp"

namespace MemoryBase{

class Channel : private NoCopy{
friend class Connection;
public:
	Channel(int fd, Poller *pol, const ReadCallBack &rcb, const WriteCallBack &wcb):filedesc(fd),poller(pol),readcb(rcb),writecb(wcb){
		events = ReadEvent;
		poller->AddChannel(this);
	}
	int Fd() const{
		return filedesc;
	}
	uint32_t Events() const{
		return events;
	}
	bool CanRead() const{
		return (events & ReadEvent);
	}
	bool CanWrite() const{
		return (events & WriteEvent);
	}
	void OnRead(const ReadCallBack &rcb){
		readcb = rcb;
	}
	void OnWrite(const WriteCallBack &wrb){
		writecb = wcb;
	}
	void EnableRead(bool flag){
		if(flag) events |= ReadEvent;
		else events &= (~WriteEvent);
		poller->UpdateChannel(this);
	}
	void EnableWrite(bool flag){
		if(flag) events |= WriteEvent;
		else events &= (~WriteEvent);
		poller->UpdateChannel(this);
	}
	void HandleRead{
		readcb();
	}
	void HandleWrite{
		writecb();
	}
    ~Channel(){
        poller->RemoveChannel(this);
    }
private:
	int filedesc;
	uint32_t events;
	Poller *poller;
	
	ReadCallBack readcb;
	WriteCallBack writecb;
}
}
#endif

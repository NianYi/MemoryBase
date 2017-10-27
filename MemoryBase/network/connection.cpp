#include <cassert>
#include <cerron>
#include <cstdio>
#include <cstring>

#include "endpoint.hpp"
#include "poller.hpp"
#include "socket.hpp"
#include "channel.hpp"
#include "buffer.hpp"
#include "readcallback.hpp"
#include "writecallback.hpp"
#include "connection.hpp"

namespace MemoryBase{

Connection::Connection(const Socket &sock, Poller *poll):socket(sock), connected(true), readcb(0), writecb(0){
	assert(socket.SetNoBlock());
	channel = new Channel(socket.Fd(), poller, [this]{this->HandleRead();}, [this]{this->HandleWrite();});	
}
Connection::Connection(const EndPoint &point, Poller *poll):connected(true), readcb(0), writecb(0){
	socket.Create();
	assert(socket.SetNoBlock());
	if(!socket.Connect(point)){
		printf("connect server: %s fail, error: %s", point.ToString().c_str(), strerror(errno));
		return;
	}
	
	channel = new Channel(socket.Fd(), poller, [this]{this->HandleRead();}, [this]{this->HandleWrite();});
}
bool Connection::Sucess() const{
	return connected;
}
bool Connection::Close(){
	if(socket.Valid()){
		connected = false;
		delete channel;
		return socket.Close();
	}
	return true;
}
Buffer& Connection::GetInput(){
	return input;
}
Buffer& Connection::GetOutput(){
	return output;
}
void Connection::HandleRead(){
	if(!connected){
		printf("connection has closed!");
		return;
	}
	bool blocked = 0;
	uint32_t len = socket.Read(input.Begin(), input.Size(), &blocked);
	if(!len && !blocked){
		Close();
		return;
	}
	input.AdvanceTail(len);
	if(len && readcb) readcb();
}
void Connection::HandleWrite(){
	SendOutput();
	if(output.Empty()){
		if(writecb) writecb();
		if(channel->CanWrite()) channel->EnableWrite(false);
	}
}
void Connection::OnRead(const ReadCallBack &rcb){
	readcb = rcb;
}
void Connection::OnWrite(const WriteCallBack &wcb){
	writecb = wcb;
}
void Connection::Send(const char* buff){
	Send(buff, strlen(buff));
}
void Connection::Send(const char* buff, uint32_t len){
	output.Read(buff, len);
	SendOutput();
}
void Connection::Send(Buffer &buff){
	output.Read(buff.Begin(), buff.Size());
	buff.Clear();
	SendOutput();
}
void Connection::SendOutput();
	if(!connected){
		printf("connection has closed!");
		return;
	}
	bool blocked = 0;
	uint32_t len = socket.Write(output.Begin(), output.Size(), &blocked);
	if(!len && !blocked){
		Close();
		return;
	}
	output.AdvanceHead(len);
	if(output.Size()) output.Adjust();
}

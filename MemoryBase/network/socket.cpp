#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstring>
#include <cerrno>
#include <cassert>

#include "socket.hpp"

namespace MemoryBase{

Socket::Socket(){};
Socket::Socket(int fd):filedesc(fd){};
int Socket::Getfd() const{
	return filedesc;
}
bool Socket::Valid() const{
	return fd != -1;
}
bool Create(){
	filedesc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return filedesc != -1;
}
bool Close(){
	bool ret = true;
	if(filedesc != -1){
		ret = !close(filedesc);
		filedesc = -1;
	}
	return ret;
}
bool Connect(const EndPoint& endpoint){
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(endpoint.Port());
	server.sin_addr.s_addr = endpoint.Address();
	return !connect(filedesc, (const struct sockaddr*)(&server), sizeof(server));
}
bool Bind(uint16_t port){
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	return !bind(filedesc, (const struct sockaddr*)&server, sizeof(server));
}
bool Listen(){
	return !listen(filedesc, 32);
}
int Accept(){
	/*	accept() in page : 490 (advance programing unix)
		first argument:		file describe;
		second argument:	client sockaddr_in struct
		third argument:		size of sockaddr_in
	*/
	return accept(filedesc, NULL, NULL);
}
uint32_t Read(char *buff, uint32_t len, bool *blocked){
	uint32_t has_read = 0;
	int ret;
	while(has_read < len && (ret = read(filedesc, buff+has_read, len-has_read))){
		if(ret < 0){
			if(errno == EINTR) continue;
			else if(errno == EAGAIN || errno == EWOULDBLOCK) *blocked = true;
			else perror("Socket Read() error : ");
			break;
		}
		has_read += ret;
	}
	return has_read;
}
uint32_t Write(const char *buff, uint32_t len, bool *blocked){
	uint32_t has_write = 0;
	int ret;
	while(has_write < len && (ret = write(filedesc, buff+has_write, len-has_write))){
		if(ret < 0){
			if(errno == EINTR) continue;
			else if(error == EAGAIN || error == EWOULDBLOCK) *blocked = true;
			else perror("Socket Write() error : ");
			break;
		}
		has_write += ret;
	}
	if(!ret) perror("Socket Write() error : ");
	return has_write;
}
bool SetOption(int value, bool flag){
	return !setsockopt(filedesc, SOL_SOCKET, value, &flag, sizeof(flag));
}
bool GetOption(int value, int *ret){
	socklen_t len = sizeof(*ret);
	return !getsockopt(filedesc, SOL_SOCKET, value, ret, &len);
}
/*
	peer : connection peer address
	sock : local address
	page : 487
*/
bool GetPeerName(EndPoint *point){
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	socklen_t len = sizeof(addr);
	if(!getpeername(filedesc, (struct sockaddr*)&addr, &len)){
		*point = EndPoint(ntohs(addr.sin_port), addr.sin_addr.s_addr);
		return true;
	}else {
		perror("getpeername() error : ");
		return false;
	}
}
bool GetSockName(EndPoint *point){
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));
	socklen_t len = sizeof(addr);
	if(!getsockname(filedesc, (struct sockaddr*)&addr, &len)){
		*point = EndPoint(ntohs(addr.sin_port), addr.sin_addr.s_addr);
		return true;
	}else{
		perror("getsockname() error : ");
		return false;
	}
}
bool AddFlag(int flag){
	int state = fcntl(filedesc, F_GETFL, 0);
	assert(~state);
	return !fcntl(filedesc, F_SETFL, state | flag);
}
bool SetNoBlock(){
	int state = fcntl(filedesc, F_GETFL, 0);
	assert(~state);
	return !fcntl(filedesc, F_SETFL, state | O_NOBLOCK);
}
bool SetReuseAddress(){
	int flag=1;
	return !setsockopt(filedesc, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
}
Socket::~Socket(){
	if(~filedesc){
		close(filedesc);
	}
}
}

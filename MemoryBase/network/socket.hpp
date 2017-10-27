#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <sys/socket.h>
namespace MemoryBase{

class Socket : private NoCopy{
public:
	Socket();
	Socket(int fd);
	int Getfd() const;
	bool Valid() const;
	bool Create();
	bool Close();
	bool Connect(const EndPoint& enpoint);
	bool Bind(uint16_t port);
	bool Listen();
	int Accept();
	uint32_t Read(char *buff, uint32_t len, bool *blocked);
	uint32_t Write(const char *buff, uint32_t len, bool *blocked);
	bool SetOption(int value, bool flag);
	bool GetOption(int value, int *ret);
	bool GetPeerName(EndPoint *point);
	bool GetSockName(EndPoint *point);
	bool AddFlag(int flag);
	bool SetNoBlock();
	bool SetReuseAddress();
	~Socket();

private:
	int filedesc;
};

}

#endif

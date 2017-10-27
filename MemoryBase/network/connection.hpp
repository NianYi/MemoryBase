#ifndef CONNECTION_HPP
#define CONNECTION_HPP

namespace MemoryBase{

class EndPoint;
class Poller;
class Socket;
class Channel;
class Buffer;
class ReadCallBack;
class WriteCallBack;

class Connection : private NoCopy{
public:
	Connection(const Socket &sock, Poller *poller);
	Connection(const EndPoint &point, Poller *poller);
	bool Sucess() const;
	bool Close();
	Buffer& GetInput();
	Buffer& GetOutput();
	void HandleRead();
	void HandleWrite();
	void OnRead(const ReadCallBack &rcb);
	void OnWrite(const WriteCallBack &wcb);
	void Send(const char* buff);
	void Send(const char* buff, uint32_t len);
	void Send(Buffer &buff);
	void SendOutput();
	virtual ~Connection();
private:
	Socket socket;
	bool connected;
	Channel *channel;
	
	Buffer input;
	Buffer output;
	ReadCallBack readcb;
	WriteCallBack writecb;
};

}
#endif

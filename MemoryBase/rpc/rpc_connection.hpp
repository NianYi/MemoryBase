#ifndef RPC_CONNECTION_HPP
#define RPC_CONNECTION_HPP
#include <map>

#include "../include/utility.hpp"
#include "../include/atomic.hpp"

namespace MemoryBase{

class Marshaller;
class Poller;
class Mutex;
class RpcConnection : public Connection{
public:
	static int RpcId;
	RpcConnection(const EndPoint &poin, Poller *poll);
	RpcConnection(const Socket &sock, Poller *poll);
	template<typename T1, typename T2>
	void Call(const char* str, const T1 *args, Future<T2> *fu);
	template<typename T>
	void RemoveFuture(Future<T> *fu);
	bool Close();
	bool Disabled();
	bool Disable();
	bool Enable();
	Marshaller& GetMarshaller();
	using Connection::OnRead;
private: 
	atomic_32_t disable;
	Marshaller mar;
	Mutex mutex;
	std::map<uint32_t, Func> futures;
	using Connection::Send;
	using Connection::OnWrite;
};
	
}//namespace MemoryBase
#endif

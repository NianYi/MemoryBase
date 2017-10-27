#include "../include/mutex.hpp"
#include "../network/poller.hpp"

#include "rpc.hpp"
#include "marshaller.hpp"
#include "rpc_connection.hpp"

namespace MemoryBase{
uint32_t RpcConnection::RpcId = 0;
RpcConnection::RpcConnection(const EndPont &poin, Poller *poll):Connection(poin, poll), disable(0), mar(input, output){

}
RpcConnection::RpcConnection(const Socket &sock, Poller *poll):Connection(poin, poll), disable(0), mar(input, output){

}
template<typename T1, typename T2>
void RpcConnection::Call(const char *str, const T2 *args, Future<T2> *fu){
	uint32_t id = Rpc::Hash(str);
	uint32_t rid = RpcId++;
	fu.SetId(rid);
	if(futures.find(rid) == futures.end()){
		futures.insert({rid, [fu, this]{
			fu->Notify(mar);
		}})
	}

}
}//namespace MemoryBase

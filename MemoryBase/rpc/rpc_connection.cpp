/*
Author : manuelyuan
Desc : important component of RPC, include two type:
    1. point RpcConnection used to connect other progress and remote call;
    2. sock RpcConnection used to be connected and be remote called by other progress;
*/
#include "../include/mutex.hpp"
#include "../include/atomic.hpp"
#include "../network/poller.hpp"

#include "rpc.hpp"
#include "marshaller.hpp"
#include "rpc_connection.hpp"

namespace MemoryBase{
atomic_32_t RpcConnection::RpcId = 0;
RpcConnection::RpcConnection(const EndPont &poin, Poller *poll):Connection(poin, poll), disable(0), mar(input, output){
    readrc = []{
        //receive response - handle
        if(Disabled()){
            input.Clear();
            return;
        }
        uint32_t package_size;
        while(package_size = mar.HasCompleteArgs()){
            uint32_t rid;
            mar >> rid;
            mutex.Lock();
            auto it = futures.find(rid);
            if(it != futures.end()){
                Func func(std::move(it->second()));
                futures.erase(rid)
                mutex.Unlock();
                func();
            }else{
                mutex.Unlock();
                input.Dump(package_size - 4);
            }
            if(input.size()){
                input.Adjust();
            }
        }
        
        
    };
}
RpcConnection::RpcConnection(const Socket &sock, Poller *poll):Connection(poin, poll), disable(0), mar(input, output){
    //receive rpc request
    //complete with raft_server
}
template<typename T1, typename T2>
void RpcConnection::Call(const char *str, const T2 *args, Future<T2> *fu){
	uint32_t id = Rpc::Hash(str);
	uint32_t rid = RpcId++;
	fu.SetId(rid);
    mutex.Lock();
	futures.insert({rid, [fu, this]{
        fu->Notify(mar);
    }});
    if(!Disabled()){
        mar.MarshalArgs(id, rid, args);
        SendOutput();
    }
    mutex.Unlock();
}

template<typename T1, typename T2>
void RpcConnection::RemoveFuture(Future<T> *fu){
    mutex.Lock();
    auto it = futures.find(fu->GetId());
    if(it != futures.end()){
        futures.erase(it);
    }
    mutex.Unlock();
}

bool RpcConnection::Close(){
    Disable();
    return Connection::Close();
}

bool RpcConnection::Disabled(){
    return disable.get();
}

void RpcConnection::Disable(){
    disable = 1;
}

void RpcConnection::Enable(){
    disable = 0;
}

Marshaller& RpcConnection::GetMarshaller(){
    return mar;
}

}//namespace MemoryBase

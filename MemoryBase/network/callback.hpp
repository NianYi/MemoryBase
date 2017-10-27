#ifndef CALLBACK_HPP
#define CALLBACK_HPP
#include <functional>
namespace MemoryBase{
class Connection;
typedef std::function<void()> ReadCallBack;
typedef std::function<void()> WriteCallBack;

typedef std::function<void(Connection *)> ConnectCallBack;
}
#endif

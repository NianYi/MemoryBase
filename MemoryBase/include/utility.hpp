#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <errno.h>
#include <cassert>
#include <functional>
namespace MemoryBase{

class NoCopy{
public:
	NoCopy() = default;
private:
	NoCopy(const NoCopy&) = delete;
	NoCopy& operator=(const NoCopy&) = delete;
};

template<typename T>
class NoCopyTemplate{
public:
	NoCopyTemplate() = default;
private:
	NoCopyTemplate(const NoCopyTemplate&) = delete;
	NoCopyTemplate& operator=(const NoCopyTemplate&) = delete;
};

typedef std::function<void()> Func;
typedef std::function<void()> Task;
}
#endif

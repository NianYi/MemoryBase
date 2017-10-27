#ifndef BUFFER_HPP
#define BUFFER_HPP
#include "../include/utility.hpp"

namespace MemoryBase{

class Buffer : private NoCopy{
public:
	Buffer();
	uint32_t Size() const;
	bool Empty() const;
	char* Begin() const;
	char* End() const;
	char* Data() const;
	uint32_t Space() const;
	void Read(const char *buff, uint32_t len);
	void Write(char *buff, uint32_t len);
	void AdvanceHead(uint32_t len);
	void AdvanceTail(uint32_t len);
	void Reset();
	void Clear();
	void Adjust();
	void InadvanceHead(uint32_t len);
	~Buffer();
private:
	char *buffer;
	uint32_t begin;
	uint32_t end;
	uint32_t size;
	uint32_t capacity;
};

}
#endif

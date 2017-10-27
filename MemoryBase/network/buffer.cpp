#include <cstring>
#include <cassert>

#include "buffer.hpp"

namespace MemoryBase{
Buffer::Buffer():begin(0), end(0), size(0), capacity(4096){
	buffer = new char[capacity];
	memset(buffer, 0, capacity);
}
uint32_t Buffer::Size() const{
	return size;
}
bool Empty() const{	
	return !size;
}
char* Begin() const{
	return buffer + begin;
}
char* End() const{
	return buffer + end;
}
uint32_t Space() const{
	return capacity - end;
}
void Read(const char *buff, uint32_t len){
	assert(buff && end+len <= capacity);
	memmove(End(), buff, len);
	end += len;
	size += len;
}
void Write(char *buff, uint32_t len){
	assert(buff && size >= len);
	memmove(buff, Begin(), len);
	begin += len;
	size -= len;
}
void AdvanceHead(uint32_t len){
	assert(size >= len);
	begin += len;
	size -= len;
}
void AdvanceTail(uint32_t len){
	assert(end+len <= capacity);
	end += len;
	size += len;	
}
void Reset(){
	if(Empty()){
		assert(begin == end);
		Clear();
	}
	/*
	else if(begin){
		memmove(buffer, Begin(), size);
		begin = 0;
		end = size;
	}
	*/
}
void Clear(){
	begin = 0;
	end = 0;
	size = 0;
}
void Adjust(){
	if(begin){
		memmove(buffer, Begin(), size);
		begin = 0;
		end = size;
	}
}
void InadvanceHead(uint32_t len){
	assert(begin >= len);
	begin -= len;
	size += len;
}
Buffer::~Buffer(){
	delete []buffer;
}

}

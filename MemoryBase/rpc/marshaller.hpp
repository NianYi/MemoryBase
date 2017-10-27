#ifndef MARSHALLER_HPP
#define MARSHALLER_HPP
#include <vector>

#include "buffer.hpp"
#include "../include/utility.hpp"

namespace MemoryBase{

class Marshaller : private NoCopy{
public:
	Marshaller():input(0),output(0){};
	Marshaller(Buffer* in, Buffer *out):input(in),output(out){};
	template<typename T>
	void MarshalArgs(uint32_t id, uint32_t rid, const T &arg);
	template<typename T>
	void MarshalReply(uint32_t rid, const T &reply);
	void Read(const void *buff, uint32_t len);
	void Write(void *buff, uint32_t len);
	uint32_t HasCompleteArgs();
	void Dump(uint32_t len);
	~Marshaller();
private:
	Buffer *input;
	Buffer *output;

	void InadvanceHead(uint32_t len);
};

inline Marshaller& operator>>(Marshaller &mar, uint8_t &val){
	mar.Write(&val, 1);
	return mar;
}
inline Marshaller& operator>>(Marshaller &mar, int &val){
	mar.Write(&val, 4);
	return mar;
}
inline Marshaller& operator>>(Marshaller &mar, uint32_t &val){
	mar.Write(&val, 4);
	return mar;
}
template<typename T>
inline Marshaller& operator>>(Marshaller &mar, std::vector<T> &val){
	uint32_t len;
	mar>>len;
	val.reserve(len);
	for(int i=0; i<len; ++i){
		T t;
		mar>>t;
		val.push_back(t);
	}
	return mar;
}
inline Marshaller& operator<<(Marshaller &mar, const uint8_t &val){
	mar.Read(&val, 1);
	return mar;
}
inline Marshaller& operator<<(Marshaller &mar, const int &val){
	mar.Read(&val, 4);
	return mar;
}
inline Marshaller& operator<<(Marshaller &mar, const uint32_t &val){
	mar.Read(&val, 4);
	return mar;
}
template<typename T>
inline Marshaller& operator<<(Marshaller &mar, const std::vector<T> &val){
	uint32_t len = (uint32_t)val.size();
	mar<<len;
	for(int i=0; i<len; ++i) mar<<val[i];
	return mar;
}
template<typename T>
void Marshaller::MarshalArgs(uint32_t id, uint32_t rid, const T *arg){
	output->Reset();
	uint32_t *len = (uint32_t*)output->End();
	output->AdvanceTail(sizeof(uint32_t));
	uint32_t before = output->Size();
	(*this) << id;
	(*this) << rid;
	(*this) << *arg;
	*len = output->Size() - before;
}
template<typename T>
void Marshaller::MarshlReply(uint32_t rid, const T *reply){
	output->Reset();
	uint32_t *len = output->End();
	output->AdvanceTail(sizeof(uint32_t));
	uint32_t before = output->Size();
	(*this) << rid;
	(*this) << *reply;
	*len = output->Size() - before;
}
void Read(const void *buff, uint32_t len){
	output->Read((const char*)buff, len);
}
void Write(void *buff, uint32_t len){
	input->Write((char*)buff, len);
}
uint32_t HasCompleteArgs(){
	if(input->Size() < 4) return 0;
	uint32_t package_len;
	*this >> package_len;
	if(package_len <= input.Size()){
		return package_len;
	}else{
		InadvanceHead(sizeof(uint32_t));
		return 0;
	}
}
void Dump(uint32_t len){
	output->AdvanceHead(len);
}
Marshaller::~Marshaller(){
	if(input) delete input;
	if(output) delete output;
}
void InadvanceHead(uint32_t len){
	output->InadvanceHead(len);
}

}//namespace MemoryBase

#endif

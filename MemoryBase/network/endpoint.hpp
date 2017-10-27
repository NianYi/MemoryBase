#ifndef ENDPOINT_HPP
#define ENDPOINT_HPP
#include <string>
#include <cassert>
#include <arpa/inet.h>
namespace MemoryBase{

class EndPoint{
public:
	static const uint8_t MaxLength = 16;
	EndPoint(uint16_t por, uint32_t addr);
	EndPoint(uint16_t por, const char *addr);
	uint16_t Port() const;
	uint32_t Address() const;
	std::string ToString() const;
private:
	uint16_t port;
	uint32_t address;
};
EndPoint::EndPoint(uint16_t por, uint32_t addr):port(por), address(addr){};
EndPoint::EndPoint(uint16_t por, const char *addr):port(por){
	assert(inet_pton(AF_INET, addr, &address) == 1);
}
uint16_t Port() const{
	return port;
}
uint32_t Address() const{
	return address;
}
std::string Tostring() const{
	char buf[MaxLength];
	assert(inet_ntop(AF_INET, &address, buf, MaxLength));
	return std::string(buf) + " port " + std::string(port);
}
}
#endif

#pragma once
#include <cstdint>

class SMS;

class Controller
{
public:
	Controller();
	~Controller();

public:
	void ConnectBus(SMS* n) { sms = n; }
	void SetRegion(uint8_t nReg);
	bool read(uint8_t addr, uint8_t& data);
	bool write(uint8_t addr, uint8_t data);

public:
	uint8_t portA;
	uint8_t portB;
	uint8_t portC;

private:
	//Pointer to BUS Object
	SMS* sms = nullptr;

	//Define Region (JP, US od EU)
	uint8_t nRegion;
};


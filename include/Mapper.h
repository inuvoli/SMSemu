#pragma once
#include <cstdint>
#include <array>

//Define Mappers
constexpr auto SEGA = 1U;
constexpr auto CODEMASTER = 2U;


class Mapper
{
public:
	Mapper();
	~Mapper();

	//Virtual Members, defined by derived class for each Mapper implementation
	virtual bool readMap(uint16_t addr, uint8_t& data) = 0;
	virtual bool writeMap(uint16_t addr, uint8_t data) = 0;

public:
	uint8_t* pROM;
	uint8_t* pRAM;
};

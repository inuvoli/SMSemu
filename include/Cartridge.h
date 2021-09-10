#pragma once
#include <cstdint>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <array>
#include <memory>

#include "Mapper.h"
#include "MapperSega.h"
#include "MapperCodemaster.h"

class Cartridge
{
public:
	Cartridge(const std::string& sFileName, uint8_t nMapper);
	~Cartridge();

private:
	//Cartridge ROM & RAM Memory
	uint8_t* cROM;
	uint8_t* cRAM;

	//Memory Mapper
	std::shared_ptr<Mapper> pMapper;

	//ROM Header
	std::array<uint8_t, 11> SegaString = { };
	uint16_t checksum;
	uint16_t serialnumber;
	uint8_t version;
	uint8_t sizecode;

public:
	bool read(uint16_t addr, uint8_t &data);
	bool write(uint16_t addr, uint8_t data);
};


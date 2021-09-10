#pragma once
#include <cstdint>
#include <array>

#include "Z80A.h"
#include "Bios.h"
#include "Mapper.h"
#include "Cartridge.h"
#include "PSG.h"
#include "VDP.h"
#include "Controller.h"
#include "MemoryManager.h"
#include "DebugConsole.h"
#include "Emuconst.h"

class SMS
{
public:
	SMS();
	SMS(const std::string& sFileName, uint8_t nMapper, int nRegion);
	~SMS();
	
	//Connected Devices
	Z80A cpu;									//Z80A CPU
	VDP vdp;									//VDP Video Device Processor
	PSG psg;									//PSG Programmable Sound Generator
	Controller cnt;								//Controller A & B
	DebugConsole dbg;							//Debug Console
	MemoryManager mem;							//Memory Manager
	
	std::map<uint16_t, std::string> mapAsm;		//Disassembler

	std::shared_ptr<Bios> bios;					//Bios ROM - Always in Page 0, 0000h - 1fffh 
	std::shared_ptr<Cartridge> cart;			//Cartridge ROM, 48K from 0000h - bfffh 							
	std::array<uint8_t, 8 * 1024> ram;			//System 8K RAM (C000h - DFFFh, Mirrored at E000h - FFFFh)
	uint64_t masterclock_cycles;
	
private:
	bool bCartInserted;

public:
	//Read & Write from Memory (Emulate MREQ active low)
	uint8_t readMem(uint16_t addr);
	void writeMem(uint16_t addr, uint8_t data);

	//Read & Write from I/O Ports (Emulate IORQ active low)
	uint8_t readIO(uint8_t addr);
	void writeIO(uint8_t addr, uint8_t data);

	bool SetRegion(const int nRegion);
	bool InsertCartridge();
	bool RemoveCartridge();
	bool NewFrame();
	bool reset();
	bool reset(uint16_t org);
	bool clock();
};


#include "Controller.h"
#include "SMS.h"

Controller::Controller()
{
	//Port are Active LOW!
	portA = 0xff;
	portB = 0xff;
	portC = 0xff;
}

Controller::~Controller()
{
}

void Controller::SetRegion(uint8_t nReg)
{
	nRegion = nReg;
}

bool Controller::read(uint8_t addr, uint8_t& data)
{
	switch (addr)
	{
	case 0x01: data = portC; break;
	case 0xc0: data = portA; break;
	case 0xc1: data = portB; break;
	}
	
	return true;
}

bool Controller::write(uint8_t addr, uint8_t data)
{
	uint8_t out_bit;

	//Nationalization Control
	portC = data;

	//Set PortB according to Regional Settings
	if (nRegion == 0)
	{
		//SMS is Japanese
		out_bit = (portC & 0x80) + ((portC & 0x20) << 1);
		out_bit = ~out_bit;
		out_bit &= 0xc0;

		portB = (portB & 0x3f) + out_bit;
	}
	else
	{
		//SMS is Japanese
		out_bit = (portC & 0x80) + ((portC & 0x20) << 1);
		out_bit &= 0xc0;

		portB = (portB & 0x3f) + out_bit;
	}

	//printf("CONTROLLER - write: addr (%.2x), data (%.2x)\n", addr, data);

	return true;
}

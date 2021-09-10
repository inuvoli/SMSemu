#include "DebugConsole.h"
#include "SMS.h"

DebugConsole::DebugConsole()
{
}

DebugConsole::~DebugConsole()
{
}

bool DebugConsole::read(uint8_t addr, uint8_t& data)
{
	return false;
}

bool DebugConsole::write(uint8_t addr, uint8_t data)
{
	//printf("DBG - write: addr(%.2x) data(%.2x) char(%c)\n", addr, data, data);
	if (addr == 0xfd)
		printf("%c", data);

	return true;
}

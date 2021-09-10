#pragma once
#include <cstdint>

class FrameBuffer
{
public:
	FrameBuffer();
	FrameBuffer(int xRes, int yRes);
	~FrameBuffer();

public:
	bool SetResolution(int xRes, int yRes);
	bool SetPixel(int x, int y, int c);
	uint32_t GetPixel(int x, int y);
	uint32_t* GetBuffer();

private:
	int m_nWidth;
	int m_nHeight;
	
	uint32_t* m_pBuffer = nullptr;
};


#include "FrameBuffer.h"

FrameBuffer::FrameBuffer()
{
	m_nWidth = 0;
	m_nHeight = 0;

	m_pBuffer = nullptr;
}

FrameBuffer::FrameBuffer(int xRes, int yRes)
{
	if (m_pBuffer != nullptr)
		delete[] m_pBuffer;

	m_nWidth = xRes;
	m_nHeight = yRes;

	m_pBuffer = new uint32_t[(xRes * yRes)];
}

FrameBuffer::~FrameBuffer()
{
	if (m_pBuffer != nullptr)
		delete[] m_pBuffer;
}

bool FrameBuffer::SetResolution(int xRes, int yRes)
{
	if (m_pBuffer != nullptr)
		delete[] m_pBuffer;

	m_nWidth = xRes;
	m_nHeight = yRes;

	m_pBuffer = new uint32_t[(xRes * yRes)];

	return true;
}

bool FrameBuffer::SetPixel(int x, int y, int c)
{
	m_pBuffer[x + y * m_nWidth] = c;
	return false;
}

uint32_t FrameBuffer::GetPixel(int x, int y)
{
	return m_pBuffer[x + y * m_nWidth];
}

uint32_t* FrameBuffer::GetBuffer()
{
	return m_pBuffer;
}

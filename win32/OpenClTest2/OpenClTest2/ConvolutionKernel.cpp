#include "ConvolutionKernel.h"


ConvolutionKernel::ConvolutionKernel(cl::Context ctx)
{
	m_ctx = ctx;
	SetGenerated(false);
}


ConvolutionKernel::~ConvolutionKernel()
{
}

cl::Buffer & ConvolutionKernel::GetBuffer() const
{
	if (!m_generated) {
		Generate();
		SetGenerated(true);
	}
	return m_buf;
}

void ConvolutionKernel::SetWidth(int width)
{
	m_width = width;
	SetGenerated(false);
}

int ConvolutionKernel::GetWidth() const
{
	return m_width;
}

void ConvolutionKernel::SetHeight(int height)
{
	m_height = height;
	SetGenerated(false);
}

void ConvolutionKernel::SetCachedBuffer(cl::Buffer buf) const
{
	m_buf = buf;
}

void ConvolutionKernel::SetGenerated(bool generated) const
{
	m_generated = generated;
}

cl::Context ConvolutionKernel::GetContext() const
{
	return m_ctx;
}

int ConvolutionKernel::GetHeight() const
{
	return m_height;
}

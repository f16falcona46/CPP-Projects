#pragma once
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

class ConvolutionKernel
{
public:
	ConvolutionKernel(cl::Context ctx);
	~ConvolutionKernel();
	cl::Buffer& GetBuffer() const;
	int GetWidth() const;
	int GetHeight() const;
protected:
	virtual void Generate() const = 0;
	void SetWidth(int width);
	void SetHeight(int height);
	void SetCachedBuffer(cl::Buffer buf) const;
	void SetGenerated(bool generated) const;
	cl::Context GetContext() const;
private:
	cl::Context m_ctx;
	int m_width;
	int m_height;
	mutable cl::Buffer m_buf;
	mutable bool m_generated;
};


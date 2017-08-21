#pragma once
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include "ConvolutionKernel.h"

class OCLConvolve
{
public:
	OCLConvolve();
	~OCLConvolve();
	void SetImageData(const float* data);
	void SetImageWidth(int width);
	void SetImageHeight(int height);
	int GetOutImageWidth() const;
	int GetOutImageHeight() const;
	void CopyImage(float* out) const;
	void Execute();
	void SetConvolutionKernel(const ConvolutionKernel* kernel);
	cl::Context GetContext() const;
private:
	cl::Context m_ctx;
	cl::Kernel m_kernel;
	cl::CommandQueue m_cqueue;
	cl::Buffer m_in_buf;
	cl::Buffer m_out_buf;
	int m_image_width;
	int m_image_height;
	int m_lwg_size;
	bool m_img_setup = false;
	bool m_out_setup = false;
	const ConvolutionKernel* m_convkern;
};


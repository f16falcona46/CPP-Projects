#pragma once
#include "ConvolutionKernel.h"
class BlurKernel :
	public ConvolutionKernel
{
public:
	BlurKernel(cl::Context ctx);
	~BlurKernel();
	void SetRadius(int r);
protected:
	virtual void Generate() const override;
private:
	int m_radius;
};


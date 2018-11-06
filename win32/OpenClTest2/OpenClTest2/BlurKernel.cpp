#include "BlurKernel.h"
#include <cmath>
#include <vector>
#include <algorithm>

BlurKernel::BlurKernel(cl::Context ctx) : ConvolutionKernel(ctx)
{
	SetRadius(1);
}


BlurKernel::~BlurKernel()
{
}

void BlurKernel::Generate() const
{
	int n_elements = GetWidth() * GetHeight();
	std::vector<float> filt(n_elements);
	float sum = 0;
	for (int i = 0; i < GetHeight(); ++i) {
		for (int j = 0; j < GetHeight(); ++j) {
			int x = j - GetWidth() / 2;
			int y = i - GetHeight() / 2;
			filt[i * GetWidth() + j] = std::exp(-std::sqrt((x * x + y * y) / n_elements));
			sum += filt[i * GetWidth() + j];
		}
	}
	std::transform(filt.begin(), filt.end(), filt.begin(), [sum](float x) {return x / sum; });
	SetCachedBuffer(cl::Buffer(GetContext(), CL_MEM_COPY_HOST_PTR, n_elements * sizeof(float), filt.data()));
}

void BlurKernel::SetRadius(int r)
{
	SetWidth(r * 2 - 1);
	SetHeight(r * 2 - 1);
	m_radius = r;
	SetGenerated(false);
}

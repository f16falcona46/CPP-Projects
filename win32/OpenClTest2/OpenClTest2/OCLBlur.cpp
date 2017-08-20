#include "OCLBlur.h"
#include <algorithm>
#include <vector>
#include <cmath>
#include <numeric>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdexcept>

template<typename T, typename U>
T RoundToMultiple(T n, U multiple)
{
	T remainder = n % multiple;
	if (remainder == 0) return n;
	else {
		return n + multiple - remainder;
	}
}

std::string ReadString(const std::string& file)
{
	std::ifstream is(file, std::ifstream::in | std::ifstream::binary);
	std::stringstream ss;
	ss << is.rdbuf();
	return ss.str();
}

OCLBlur::OCLBlur()
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	cl::Platform target_platform = platforms[0];
	for (const cl::Platform& platform : platforms) {
		std::string vendor = platform.getInfo<CL_PLATFORM_VENDOR>();
		if (vendor.substr(0, vendor.length() - 1) == "NVIDIA Corporation") {
			target_platform = platform;
			break;
		}
	}
	cl_context_properties cprops[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)target_platform(), 0 };
	m_ctx = cl::Context(CL_DEVICE_TYPE_GPU, cprops, nullptr, nullptr);
	std::vector<cl::Device> devices = m_ctx.getInfo<CL_CONTEXT_DEVICES>();
	std::string device_name = devices[0].getInfo<CL_DEVICE_NAME>();
	std::cout << "Using device: " << device_name << '\n';
	m_cqueue = cl::CommandQueue(m_ctx, devices[0]);
	m_lwg_size = devices[0].getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
	std::cout << "Local workgroup max size: " << m_lwg_size << '\n';

	//get and build program
	const std::string& kernel_source = ReadString("kernel.cl");
	cl::Program::Sources sources(1, std::make_pair(kernel_source.c_str(), kernel_source.length()));
	cl::Program program(m_ctx, sources);
	program.build(devices);

	//get kernel and bind args
	m_kernel = cl::Kernel(program, "Convolve");
}


OCLBlur::~OCLBlur()
{
}

void OCLBlur::SetImageData(const float * data)
{
	if (!m_img_setup) {
		m_in_buf = cl::Buffer(m_ctx, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * m_image_width * m_image_height, const_cast<float*>(data));
		m_img_setup = true;
		m_out_setup = false;
	}
	else {
		m_cqueue.enqueueWriteBuffer(m_in_buf, true, 0, sizeof(cl_float) * m_image_width * m_image_height, data);
		m_cqueue.finish();
	}
}

void OCLBlur::SetImageWidth(int width)
{
	m_image_width = width;
	m_img_setup = false;
}

void OCLBlur::SetImageHeight(int height)
{
	m_image_height = height;
	m_img_setup = false;
}

int OCLBlur::GetOutImageWidth() const
{
	return m_image_width - m_filt_width + 1;
}

int OCLBlur::GetOutImageHeight() const
{
	return m_image_height - m_filt_height + 1;
}

void OCLBlur::CopyImage(float * out) const
{
	m_cqueue.enqueueReadBuffer(m_out_buf, CL_TRUE, 0, sizeof(cl_float) * (m_image_width - m_filt_width + 1) * (m_image_height - m_filt_height + 1), out);
	m_cqueue.finish();
}

void OCLBlur::Execute()
{
	if (!(m_img_setup && m_filt_setup)) throw std::runtime_error("Didn't provide sizes of filter and image.");
	if (!m_out_setup) {
		m_out_buf = cl::Buffer(m_ctx, CL_MEM_WRITE_ONLY, sizeof(cl_float) * (m_image_width - m_filt_width + 1) * (m_image_height - m_filt_height + 1));
		m_out_setup = true;
	}
	
	m_kernel.setArg(0, m_in_buf);
	m_kernel.setArg(1, (cl_uint)(m_image_width - m_filt_width + 1));
	m_kernel.setArg(2, (cl_uint)(m_image_height - m_filt_height + 1));
	m_kernel.setArg(3, m_filt_buf);
	m_kernel.setArg(4, m_out_buf);
	m_kernel.setArg(5, (cl_uint)m_filt_width);
	m_kernel.setArg(6, (cl_uint)m_filt_height);

	int num_workitems = m_image_width * m_image_height;
	cl::Event e;
	m_cqueue.enqueueNDRangeKernel(m_kernel, cl::NDRange(0, 0), cl::NDRange(RoundToMultiple(m_image_width - m_filt_width + 1, m_lwg_size / 16), RoundToMultiple(m_image_height - m_filt_height + 1, 16)), cl::NDRange(m_lwg_size / 16, 16), nullptr, &e);
	e.wait();

	m_cqueue.finish();
}

void OCLBlur::SetBlurRadius(int r)
{
	m_filt_height = r * 2 - 1;
	m_filt_width = r * 2 - 1;
	GenerateFilter();
	m_filt_setup = true;
	m_out_setup = false;
}

void OCLBlur::GenerateFilter()
{
	int n_elements = m_filt_height * m_filt_width;
	std::vector<float> filt(n_elements);
	for (int i = 0; i < m_filt_height; ++i) {
		for (int j = 0; j < m_filt_width; ++j) {
			int x = j - m_filt_width / 2;
			int y = i - m_filt_height / 2;
			filt[i * m_filt_width + j] = std::exp(-std::sqrt((x * x + y * y) / n_elements));
		}
	}
	float sum = std::accumulate(filt.begin(), filt.end(), 0.0f, [](float x, float y) { return x + y; });
	std::transform(filt.begin(), filt.end(), filt.begin(), [sum](float x) { return x / sum; });
	m_filt_buf = cl::Buffer(m_ctx, CL_MEM_COPY_HOST_PTR, n_elements * sizeof(float), filt.data());
}

#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <locale>
#include <codecvt>
#include <stdexcept>
#include <memory>
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <png++/png.hpp>
#include <Windows.h>

/*
BOOL check_error(cl_int err, const std::wstring& str)
{
	if (err != CL_SUCCESS) {
		MessageBox(NULL, (str + L" Error code: " + std::to_wstring(err)).c_str(), L"OpenCL error", MB_OK);
		return TRUE;
	}
	return FALSE;
}
*/

std::string ReadString(const std::string& file)
{
	std::ifstream is(file, std::ifstream::in | std::ifstream::binary);
	std::stringstream ss;
	ss << is.rdbuf();
	return ss.str();
}

template<typename T, typename U>
T RoundToMultiple(T n, U multiple)
{
	T remainder = n % multiple;
	if (remainder == 0) return n;
	else {
		return n + multiple - remainder;
	}
}

int main()
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16conv;

	png::image<png::gray_pixel> img("test2.png");
	std::unique_ptr<cl_float[]> in_image(new cl_float[img.get_width() * img.get_height()]);
	for (size_t i = 0; i < img.get_width(); ++i) {
		for (size_t j = 0; j < img.get_height(); ++j) {
			in_image[j * img.get_width() + i] = img.get_pixel(i, j) / 255.0f;
		}
	}
	std::unique_ptr<cl_float[]> out_image(new cl_float[(img.get_width() - 2) * (img.get_height() - 2)]);
	cl_float filter[] =
		{ -1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, -1.0f };
	cl_float blurfilter[] =
	{ 1 / 16.0f, 2/ 16.0f, 2/ 16.0f,
	2/ 16.0f, 4/ 16.0f, 2/ 16.0f,
	1 / 16.0f, 2/ 16.0f, 1/ 16.0f };
	try {
		//set up CL, get device
		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);
		cl::Platform target_platform = platforms[0];
		for (const cl::Platform& platform : platforms) {
			std::string vendor = platform.getInfo<CL_PLATFORM_VENDOR>();
			if (vendor.substr(0, vendor.length() - 1)  == "NVIDIA Corporation") {
				target_platform = platform;
				break;
			}
		}
		cl_context_properties cprops[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)target_platform(), 0 };
		cl::Context ctx(CL_DEVICE_TYPE_GPU, cprops, nullptr, nullptr);
		std::vector<cl::Device> devices = ctx.getInfo<CL_CONTEXT_DEVICES>();
		std::string device_name = devices[0].getInfo<CL_DEVICE_NAME>();
		std::cout << "Using device: " << device_name << '\n';
		cl::CommandQueue cqueue(ctx, devices[0]);
		const int lwg_size = devices[0].getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
		std::cout << "Local workgroup max size: " << lwg_size << '\n';

		cl::Buffer in_image_buf(ctx, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * img.get_width() * img.get_height(), in_image.get());
		cl::Buffer out_image_buf(ctx, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * (img.get_width() - 2) * (img.get_height() - 2), out_image.get());
		cl::Buffer filter_buf(ctx, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * 9, blurfilter);

		//get and build program
		const std::string& kernel_source = ReadString("kernel.cl");
		cl::Program::Sources sources(1, std::make_pair(kernel_source.c_str(), kernel_source.length()));
		cl::Program program(ctx, sources);
		program.build(devices);

		//get kernel and bind args
		cl::Kernel kernel(program, "Convolve");
		kernel.setArg(0, in_image_buf);
		kernel.setArg(1, (cl_uint)(img.get_width() - 2));
		kernel.setArg(2, (cl_uint)(img.get_height() - 2));
		kernel.setArg(3, filter_buf);
		kernel.setArg(4, out_image_buf);
		kernel.setArg(5, (cl_uint)3);
		cqueue.finish();

		cl::Event e;
		cqueue.enqueueNDRangeKernel(kernel, cl::NDRange(), cl::NDRange(RoundToMultiple(img.get_width() - 2, 32), RoundToMultiple(img.get_height() - 2, lwg_size / 32)), cl::NDRange(32, lwg_size / 32), 0, &e);
		e.wait();

		cqueue.finish();
		
		cqueue.enqueueReadBuffer(out_image_buf, CL_TRUE, 0, sizeof(cl_float) * (img.get_width() - 2) * (img.get_height() - 2), out_image.get());
		cqueue.finish();
		png::image<png::gray_pixel> out(img.get_width() - 2, img.get_height() - 2);
		for (size_t i = 0; i < img.get_width() - 2; ++i) {
			for (size_t j = 0; j < img.get_height() - 2; ++j) {
				//std::cout << out_image[j * (img.get_width() - 2) + i] << '\n';
				int pix = 255 * out_image[j * (img.get_width() - 2) + i];
				/*if (pix < 0) pix = 0;
				if (pix > 255) pix = 255;*/
				out.set_pixel(i, j, (uint8_t)(pix));
			}
		}
		out.write("out.png");
	}
	catch (const cl::Error& e) {
		std::cout << e.what() << ' ' << e.err() << '\n';
	}
	catch (const std::exception& e) {
		std::cout << e.what() << '\n';
	}
	std::wcout << L"Press ENTER to continue...";
	std::wstring dummy;
	std::getline(std::wcin, dummy);
	return 0;
}
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

int main()
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16conv;

	png::image<png::gray_pixel> img("test.png");
	std::unique_ptr<cl_float[]> in_image(new cl_float[img.get_width() * img.get_height()]);
	for (size_t i = 0; i < img.get_width(); ++i) {
		for (size_t j = 0; j < img.get_height(); ++j) {
			in_image[j * img.get_width() + i] = img.get_pixel(i, j) / 255.0f;
		}
	}
	std::unique_ptr<cl_float[]> out_image(new cl_float[(img.get_width() - 2) * (img.get_height() - 2)]);
	cl_float filter[] =
		{ -1.0f, -1.0f, -1.0f,
		-1.0f, 8.0f, -1.0f,
		-1.0f, -1.0f, -1.0f };
	try {
		//set up CL, get device
		std::vector<cl::Platform> platforms;
		cl::Platform::get(&platforms);
		cl::Platform target_platform = platforms[0];
		for (const cl::Platform& platform : platforms) {
			if (platform.getInfo<CL_PLATFORM_VENDOR>() == "NVIDIA Corporation") {
				target_platform = platform;
				break;
			}
		}
		cl_context_properties cprops[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)target_platform(), 0 };
		cl::Context ctx(CL_DEVICE_TYPE_GPU, cprops, nullptr, nullptr);
		std::vector<cl::Device> devices = ctx.getInfo<CL_CONTEXT_DEVICES>();
		cl::CommandQueue cqueue(ctx, devices[0]);

		cl::Buffer in_image_buf(ctx, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * img.get_width() * img.get_height(), in_image.get());
		cl::Buffer out_image_buf(ctx, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * (img.get_width() - 2) * (img.get_height() - 2), out_image.get());
		cl::Buffer filter_buf(ctx, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * 9, filter);

		//get and build program
		const std::string& kernel_source = ReadString("kernel.cl");
		cl::Program::Sources sources(1, std::make_pair(kernel_source.c_str(), kernel_source.length()));
		cl::Program program(ctx, sources);
		program.build(devices);

		//get kernel and bind args
		cl::Kernel kernel(program, "Convolve");
		kernel.setArg(0, in_image_buf);
		kernel.setArg(1, filter_buf);
		kernel.setArg(2, out_image_buf);
		kernel.setArg(3, 3);
		cqueue.finish();

		cl::Event e;
		cqueue.enqueueNDRangeKernel(kernel, cl::NDRange(), cl::NDRange(img.get_width() - 2, img.get_height() - 2), cl::NDRange(1, 1), 0, &e);
		e.wait();

		cqueue.finish();
		
		cqueue.enqueueReadBuffer(out_image_buf, CL_TRUE, 0, sizeof(cl_float) * (img.get_width() - 2) * (img.get_height() - 2), out_image.get());
		cqueue.finish();
		png::image<png::gray_pixel> out(img.get_height() - 2, img.get_width() - 2);
		for (size_t i = 0; i < img.get_width() - 2; ++i) {
			for (size_t j = 0; j < img.get_height() - 2; ++j) {
				//std::cout << out_image[j * (img.get_width() - 2) + i] << '\n';
				out.set_pixel(i, j, (uint8_t)(255 * out_image[j * (img.get_width() - 2) + i]));
			}
		}
		out.write("out.png");

		std::wcout << L"Press ENTER to continue...";
		std::wstring dummy;
		std::getline(std::wcin, dummy);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << '\n';
	}
	return 0;
}
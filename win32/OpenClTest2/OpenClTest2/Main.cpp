#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <sstream>
#include <locale>
#include <codecvt>
#include <stdexcept>
#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>	

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
	/*
	cl_int err = CL_SUCCESS;
	std::vector<cl::Platform> platforms;
	err = cl::Platform::get(&platforms);
	if (check_error(err, L"getPlatforms()")) std::exit(-1);
	std::wstringstream ss;
	for (const cl::Platform& platform : platforms) {
		std::vector<cl::Device> devices;
		std::string buf;
		platform.getInfo(CL_PLATFORM_NAME, &buf);
		ss << L"\n\nPlatform:" << utf16conv.from_bytes(buf) << L"\n\n";
		err = platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
		if (check_error(err, L"getDevices()")) std::exit(-1);
		for (const cl::Device& device : devices) {
			std::string details;
			std::string device_version;
			std::string driver_version;
			err = device.getInfo(CL_DEVICE_NAME, &details);
			if (check_error(err, L"device.getInfo()")) std::exit(-1);
			err = device.getInfo(CL_DEVICE_VERSION, &device_version);
			if (check_error(err, L"device.getInfo()")) std::exit(-1);
			err = device.getInfo(CL_DRIVER_VERSION, &driver_version);
			if (check_error(err, L"device.getInfo()")) std::exit(-1);
			ss << L"Device Name: " << utf16conv.from_bytes(details) << L", Driver version: " << utf16conv.from_bytes(driver_version) << L", Device Version: " << utf16conv.from_bytes(device_version) << L"\n";
		}
	}
	std::wcout << ss.str();
	*/
	cl_float in_image[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f };
	cl_float out_image[4];
	cl_float filter[] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	cl_float foo[50000] = { -1.0f };
	std::cout << "out\n";
	for (int i = 0; i < 4; ++i) {
		std::cout << out_image[i] << '\n';
	}
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

		cl::Buffer in_image_buf(ctx, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * 16, in_image);
		cl::Buffer out_image_buf(ctx, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * 4, out_image);
		cl::Buffer filter_buf(ctx, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * 9, filter);
		cl::Buffer foo_buf(ctx, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * 50000, foo);

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
		kernel.setArg(4, foo_buf);
		cqueue.finish();

		cl::Event e;
		cqueue.enqueueNDRangeKernel(kernel, cl::NDRange(), cl::NDRange(2, 2), cl::NDRange(2, 2), 0, &e);
		e.wait();

		cqueue.finish();
		
		cqueue.enqueueReadBuffer(out_image_buf, CL_TRUE, 0, sizeof(cl_float) * 4, out_image);
		cqueue.enqueueReadBuffer(foo_buf, CL_TRUE, 0, sizeof(cl_float) * 50000, foo);
		cqueue.finish();
		std::cout << "out\n";
		for (int i = 0; i < 4; ++i) {
			std::cout << out_image[i] << '\n';
		}

		std::cout << "in\n";
		for (int i = 0; i < 16; ++i) {
			std::cout << in_image[i] << '\n';
		}

		std::cout << "filt\n";
		for (int i = 0; i < 9; ++i) {
			std::cout << filter[i] << '\n';
		}

		std::cout << "foo\n";
		for (int i = 0; i < 50000; ++i) {
			if (foo[i]) std::cout << i << '=' << foo[i] << '\n';
		}

		std::wcout << L"Press ENTER to continue...";
		std::wstring dummy;
		std::getline(std::wcin, dummy);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << '\n';
	}
	return 0;
}
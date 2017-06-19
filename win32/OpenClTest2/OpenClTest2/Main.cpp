#include <cstdlib>
#include <string>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <sstream>
#include <locale>
#include <codecvt>
#include <CL/cl.hpp>	

BOOL check_error(cl_int err, const std::wstring& str)
{
	if (err != CL_SUCCESS) {
		MessageBox(NULL, (str + L" Error code: " + std::to_wstring(err)).c_str(), L"OpenCL error", MB_OK);
		return TRUE;
	}
	return FALSE;
}

int main()
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> utf16conv;
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
	std::wcout << L"Press ENTER to continue...";
	std::wstring dummy;
	std::getline(std::wcin, dummy);
	return 0;
}
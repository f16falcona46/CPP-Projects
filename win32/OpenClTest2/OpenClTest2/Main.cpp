#include <iostream>
#include <memory>
#include <vector>
#include <png++/png.hpp>
#include <Windows.h>

#include "OCLBlur.h"

int main()
{
	png::image<png::gray_pixel> img("test4.png");
	std::unique_ptr<cl_float[]> in_image(new cl_float[img.get_width() * img.get_height()]);
	for (size_t i = 0; i < img.get_width(); ++i) {
		for (size_t j = 0; j < img.get_height(); ++j) {
			in_image[j * img.get_width() + i] = img.get_pixel(i, j) / 255.0f;
		}
	}
	try {
		OCLBlur b;
		b.SetImageWidth(img.get_width());
		b.SetImageHeight(img.get_height());
		b.SetImageData(in_image.get());
		b.SetBlurRadius(4);
		std::vector<float> img_out(b.GetOutImageWidth() * b.GetOutImageHeight());
		b.Execute();
		b.CopyImage(img_out.data());

		png::image<png::gray_pixel> out_png_img(b.GetOutImageWidth(), b.GetOutImageHeight());
		for (size_t y = 0; y < b.GetOutImageHeight(); ++y) {
			for (size_t x = 0; x < b.GetOutImageWidth(); ++x) {
				out_png_img.set_pixel(x, y, 255 * img_out[y * b.GetOutImageWidth() + x]);
			}
		}
		out_png_img.write("out_image.png");
	}
	catch (const cl::Error& e) {
		std::cout << e.what() << ' ' << e.err() << '\n';
		throw e;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << '\n';
		throw e;
	}
	std::wcout << L"Press ENTER to continue...";
	std::wstring dummy;
	std::getline(std::wcin, dummy);
	return 0;
}
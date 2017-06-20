__kernel void Convolve(const __global float * InImage, __constant float * Filter, __global float * OutImage, const int FilterWidth)
{
	const int width = get_global_size(0);
	const int x = get_global_id(0);
	const int y = get_global_id(1);
	const int in_width = width + FilterWidth - 1;
	float sum = 0.0f;
	for (int y_filt = 0; y_filt < FilterWidth; ++y_filt) {
		const int y_filt_idx = FilterWidth * y_filt;
		const int y_in_idx = in_width * (y + y_filt);
		for (int x_filt = 0; x_filt < FilterWidth; ++x_filt) {
			sum += Filter[x_filt + y_filt_idx] * InImage[x_filt + x + y_in_idx];
		}
	}
	OutImage[x + y * width] = sum;
}
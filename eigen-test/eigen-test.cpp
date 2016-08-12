#include <Eigen/Dense>

int main() {
	Eigen::MatrixXf mat1 = Eigen::MatrixXf::Identity(1000, 1000);
	Eigen::MatrixXf mat2 = Eigen::MatrixXf::Identity(1000, 1000);
	for (size_t i = 0; i < 100; ++i) {
		mat1 *= mat2;
	}
	return 0;
}

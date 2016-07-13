#include "microscopy_funcs.hxx"
#include <vector>
#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

void ifx::read_experiment_params(std::string path, Experiment_Params* params) {
	std::regex width ("Pixels: ([-0-9]+)");
	std::regex height ("Y Pixels: ([-0-9]+)");
	std::regex z_stepsize ("Z StepSize: ([-.0-9]+)");
	std::regex x_frames ("Frames in one X Line: ([0-9]+)");
	std::regex y_frames ("Frames in one Y Line: ([0-9]+)");
	std::regex z_frames ("Frames in one Y Line: ([0-9]+)");
	std::regex calibrationdate ("calibration date: (.*)");
	std::smatch match;
	
	std::ifstream fin(path);
	std::string line;
	while (std::getline(fin, line)) {
		std::cout << line << std::endl;
	}
}

std::vector<std::string> ifx::get_rawfile_filenames(std::string path) {
	std::vector<std::string> files;
	std::regex rawbin (".*/Raw_[0-9]+\\.bin");
	std::smatch match;
	boost::filesystem::path p (path+"/RawFast");
	for (boost::filesystem::directory_entry& x : boost::filesystem::directory_iterator(p)) {
		std::string pathstring = x.path().generic_string();
		if (std::regex_match(pathstring, match, rawbin)) {
			files.push_back(pathstring);
		}
	}
	return files;
}
#ifndef MICROSCOPY_EXPERIMENT_PARAM_STRUCT_FUNCTIONS
#define MICROSCOPY_EXPERIMENT_PARAM_STRUCT_FUNCTIONS

#include "microscopy_structs.hxx"
#include <vector>
#include <string>
namespace ifx{
void read_experiment_params(std::string path, Experiment_Params* params);
std::vector<std::string> get_rawfile_filenames(std::string path);
}

#endif
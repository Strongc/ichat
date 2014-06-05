#include "StdAfx.h"
#include "utils.h"

std::string current_dir()
{
    return boost::filesystem::initial_path<boost::filesystem::path>().string(); 
}
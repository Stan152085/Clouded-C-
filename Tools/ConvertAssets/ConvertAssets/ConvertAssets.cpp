#include "stdafx.h"
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include <windows.h>
#include <direct.h>

namespace fs = std::experimental::filesystem;

std::string ExePath() {
  char buffer[MAX_PATH];
  _getcwd(buffer, MAX_PATH);
  return std::string(buffer);
}

void FindFilesToConvert(std::string& base_path)
{
  std::vector<std::string> files_to_convert;
  for (auto & p : fs::recursive_directory_iterator(base_path))
  {
    std::replace(base_path.begin(), base_path.end(), '\\', '/');
    std::cout << p << std::endl;
    std::string path = p.path().generic_string();
    if (path.find(".fbx") != std::string::npos)
    {
      path.erase(0, base_path.size());
      files_to_convert.push_back(path);
    }
  }

  std::string FBX2glTF_name = "FBX2glTF.exe" ;
  std::string::iterator name_it;
  for (name_it = FBX2glTF_name.begin(); name_it != FBX2glTF_name.end(); ++name_it)
  {
    if (*name_it == '\\')
    {
      *name_it = '/';
    }
  }

  std::vector<std::string>::iterator it = files_to_convert.begin();
  for (; it != files_to_convert.end(); ++it)
  {
    std::string res_path = *it;
    //res_path.erase(res_path.begin());
    size_t pos = res_path.find(".fbx");
    res_path.erase(pos, res_path.size());

    char command[512];
    std::sprintf(command, "%s --verbose --input %s --output %s", FBX2glTF_name.c_str(), (*it).c_str(), res_path.c_str());
    std::system(command);
    std::cout << command << std::endl;
  }
}


int main()
{
  std::string exe_path = ExePath();
  std::cout << "my directory is " << exe_path << "\n";
  FindFilesToConvert(exe_path);
  char res;
  std::cin >> res;
}
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <yaml-cpp/yaml.h>
using namespace std;

class PPMessages
{
   vector<string> langList;
   YAML::Node messages;
   std::string language;
public:
   PPMessage();
};

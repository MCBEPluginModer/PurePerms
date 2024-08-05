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
   string language;
   string replaceString(string str, const string& from, const string& to);
   YAML::Node getNestedNode(const string& node);
public:
   PPMessage();
   void registerLanguages();
   std::string getMessage(const string& node, const vector<string>& vars = {});
   inline string getVersion() {return messages["messages-version"].as<string>();}
   void loadMessages();
   void reloadMessages();
};

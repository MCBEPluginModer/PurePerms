#pragma once
#include <yaml-cpp/yaml.h>
#include <map>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <mc/world/actor/player/Player.h>
#include "MyMod.h"
#include <variant>
#include <tuple>
#include <optional>
#include <ll/api/Logger.h>
using namespace std;

class PPGroup
{
public:
   std::string name,alias;
   bool IsDefault;
   vector<string> permissions;
   YAML::Node worlds;
   mcpm::PurePerms* plugin;
   std::vector<PPGroup*> parents;
   PPGroup() {}
   PPGroup(mcpm::PurePerms* _plugin,std::string _name) : plugin(_plugin),name(_name) {}
   string __toString() {return name;}
   bool addParent(PPGroup* parent);
   void createWorldData(string levelName);
   variant<YAML::Node,string> getAlias();
   variant<YAML::Node,string> getData();
   std::vector<string> getGroupPermissions(string levelName = "");
   string getName() {return name;}
   YAML::Node getNode(string node);
   vector<PPGroup*> getParentGroups();
   YAML::Node getWorldData(string levelName);
   std::optional<YAML::Node> getWorldNode(string levelName,string node);
   bool isDefault(string levelName = "");
   void removeNode(string node);
   bool removeParent(PPGroup* parent);
   void removeWorldNode(string levelName,string node);
   void setData(tuple<string,bool,vector<string>,vector<string>,YAML::Node> data);
   void setDefault(string levelName = "");
   bool setGroupPermission(string permission,string levelName = "");
   void setNode(string node,std::variant<bool,int,float,double,string> value);
   void setWorldData(string levelName,tuple<bool,vector<string>> worldData);
   void setWorldNode(string levelName,string node,variant<bool,vector<string>> value);
   void sortPermissions();
   bool unsetGroupPermission(string permission,string levelName = "");
};

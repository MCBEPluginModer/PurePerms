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
using namespace std;

class PPGroup
{
   std::string name;
   mcpm::PurePerms* plugin;
   std::vector<PPGroup*> parents;
public:
   PPGroup(mcpm::PurePerms* _plugin,std::string _name) : plugin(_plugin),name(_name) {}
   string __toString() {return name;}
   void addParent(PPGroup* parent);
   void createWorldData(string levelName);
   variant<YAML::Node,string> getAlias();
   variant<PPGroup*,string> getData();
   std::vector<string> getGroupPermissions(string levelName = "");
   string getName() {return name;}
   PPGroup* getNode(string node);
   vector<PPGroup*> getParentGroups();
   YAML::Node getWorldData(string levelName);
   YAML::Node getWorldNode(string levelName,string node);
   bool isDefault(string levelName = "");
   void removeNode(string node);
   bool removeParent(PPGroup* parent);
   void removeWorldNode(string levelName,string node);
   void setData(tuple<string,bool,vector<string>,vector<string>,YAML::Node> data);
   void setDefault(string levelName = "");
   bool setGroupPermission(string permission,string levelName = "");
   void setNode(string node,variant<int,float,double,string> value);
};

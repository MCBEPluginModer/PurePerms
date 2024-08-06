#include <yaml-cpp/yaml.h>
#include <map>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <mc/world/actor/player/Player.h>
#include "MyMod.h"
#include <variant>
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
};

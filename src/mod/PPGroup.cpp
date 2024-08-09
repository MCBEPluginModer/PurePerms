#include "PPGroup.h"

bool PPGroup::addParent(PPGroup* parent)
{
    bool fnd = false;
    for (auto g : parents)
    {
        if (g->getName() == parent->getName())
        {
            fnd = true;
            break;
        }
    }
    if (this == parent || fnd) 
    {
        return false;
    }
    auto data = getData(); 
    get<0>(data)["inheritance"].push_back(parent->getName());
    tuple<string,bool,vector<string>,vector<string>,YAML::Node> data1 = std::make_tuple<string,bool,vector<string>,vector<string>,YAML::Node>(get<0>(data)["alias"].as<string>(),get<0>(data)["isDefault"].as<bool>(),get<0>(data)["inheritance"].as<vector<string>>(),get<0>(data)["permissions"].as<vector<string>>(),get<0>(data)["worlds"].as<YAML::Node>());
    setData(data1);
    //plugin->updatePlayersInGroup(*this); // Notify the plugin to update players in this group

    return true;
}

void PPGroup::createWorldData(string levelName)
{
    auto data = getData();   
    if (!std::get<0>(data)["worlds"][levelName]) 
    {
        std::get<0>(data)["worlds"][levelName]["isDefault"] = false;
        std::get<0>(data)["worlds"][levelName]["permissions"] = YAML::Node(YAML::NodeType::Sequence);
        tuple<string,bool,vector<string>,vector<string>,YAML::Node> data1 = std::make_tuple<string,bool,vector<string>,vector<string>,YAML::Node>(get<0>(data)["alias"].as<string>(),get<0>(data)["isDefault"].as<bool>(),get<0>(data)["inheritance"].as<vector<string>>(),get<0>(data)["permissions"].as<vector<string>>(),get<0>(data)["worlds"].as<YAML::Node>());
        setData(data1);
    }
}

std::variant<YAML::Node, std::string> PPGroup::getAlias()
{
        YAML::Node aliasNode = getNode("alias");
        if (aliasNode.IsNull()) 
        {
            return "";
        }
        return aliasNode;
}

variant<YAML::Node,string> PPGroup::getData()
{
    YAML::Node data;
//data = plugin->getProvider()->getGroupData(this);
    return data;
}

std::vector<string> PPGroup::getGroupPermissions(string levelName)
{
   std::vector<string> perms;
   if (levelName == "")
   {
     perms = getNode("permissions").as<vector<string>>(); 
   }
   else
     perms = getWorldData(levelName)["permissions"].as<vector<string>>();
   if (perms.size() == 0)
   {
      ll::Logger logger("PurePerms");
      string s = "Invalid \'permissions'\ node given to PPGroup::getGroupPermissions";
      logger.fatal(s);
      return {};
   }
   for (auto p : parents)
   {
      auto parPerms = p->getGroupPermissions(levelName);
      perms.insert(perms.end(), parPerms.begin(), parPerms.end());
   }
   return perms;
}

YAML::Node PPGroup::getNode(string node)
{
   YAML::Node nul;
   if (!get<0>(getData()).IsNull())
     return get<0>(getData())[node];
   else
     return nul;
}

#include "PPGroup.h"
#include <mc/world/level/Level.h>
#include <mc/world/level/dimension/DimensionManager.h>
#include <ll/api/service/Bedrock.h>

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

vector<PPGroup*> PPGroup::getParentGroups()
{
    if (parents.empty()) 
    {
            YAML::Node inheritanceNode = getNode("inheritance");
            
            if (!inheritanceNode || !inheritanceNode.IsSequence()) {
                 ll::Logger logger("PurePerms");
                logger.fatal("Invalid \'inheritance'\ node given to " + std::string(__FUNCTION__));
                return {};
            }

            /*for (auto& parentGroupName : inheritanceNode) 
            {
                PPGroup parentGroup = plugin->getGroup(parentGroupName.as<std::string>());
                if (parentGroup) 
                {
                    parents.push_back(parentGroup);
                }
            }*/
    }
    return parents;
}

YAML::Node PPGroup::getWorldData(string levelName)
{
    if (levelName == "")
      return YAML::Node();
    createWorldData(levelName);
    return get<0>(getData())["worlds"][levelName];
}

std::optional<YAML::Node> PPGroup::getWorldNode(string levelName,string node)
{
    YAML::Node worldNode = getWorldData(levelName);

        // Check if the node exists
    if (worldNode.IsNull() || worldNode[node].IsNull()) 
    {
            return std::nullopt;  // Equivalent to returning null in PHP
    }

    return worldNode[node];
}

bool PPGroup::isDefault(string levelName)
{
    if (levelName.empty()) 
    {
        return getNode("isDefault").as<bool>(false);
    } 
    else 
    {
        return getWorldData(levelName)["isDefault"].as<bool>(false);
    }
}

void PPGroup::removeNode(string node)
{
    YAML::Node tempGroupData = get<0>(getData());
    if (tempGroupData[node]) 
    { // Check if the node exists
        tempGroupData.remove(node); // Remove the node
        tuple<string,bool,vector<string>,vector<string>,YAML::Node> data1 = std::make_tuple<string,bool,vector<string>,vector<string>,YAML::Node>(tempGroupData["alias"].as<string>(),tempGroupData["isDefault"].as<bool>(),tempGroupData["inheritance"].as<vector<string>>(),tempGroupData["permissions"].as<vector<string>>(),tempGroupData["worlds"].as<YAML::Node>());
        setData(data1); // Update the group data
    }
}

bool PPGroup::removeParent(PPGroup* parent)
{
    YAML::Node tempGroupData = get<0>(getData());

        // Get the inheritance list and convert it to a vector of strings
        std::vector<std::string> inheritance = tempGroupData["inheritance"].as<std::vector<std::string>>();

        // Remove the parent's name from the inheritance list
        inheritance.erase(std::remove(inheritance.begin(), inheritance.end(), parent->getName()), inheritance.end());

        // Update the inheritance node
        tempGroupData["inheritance"] = inheritance;
 tuple<string,bool,vector<string>,vector<string>,YAML::Node> data1 = std::make_tuple<string,bool,vector<string>,vector<string>,YAML::Node>(tempGroupData["alias"].as<string>(),tempGroupData["isDefault"].as<bool>(),tempGroupData["inheritance"].as<vector<string>>(),tempGroupData["permissions"].as<vector<string>>(),tempGroupData["worlds"].as<YAML::Node>());
        // Set the modified data back to the group
        setData(data1);

        // Update the players in the group
       // plugin->updatePlayersInGroup(*this);

        return true;
}

void PPGroup::removeWorldNode(string levelName,string node)
{
     YAML::Node worldData = getWorldData(levelName);

        if (worldData[node]) {
            worldData.remove(node); // Removing the node
            tuple<bool,vector<string>> worldData1;
            if (node == "isDefault")
             worldData1 = make_tuple<bool,vector<string>>(false,worldData["permissions"].as<vector<string>>());
            else if (node == "permissions")
             worldData1 = make_tuple<bool,vector<string>>(worldData["isDefault"].as<bool>(),{});
            setWorldData(levelName, worldData1);
        }
}

void PPGroup::setData(tuple<string,bool,vector<string>,vector<string>,YAML::Node> data)
{
    //plugin->getProvider()->setGroupData(this,data);
}

void PPGroup::setDefault(string levelName)
{
  if (levelName.empty()) 
  {
            setNode("isDefault", true);
  } 
  else
  {
            YAML::Node worldData = getWorldData(levelName);
            worldData["isDefault"] = true;
            tuple<bool,vector<string>> worldData1 = make_tuple<bool,vector<string>>(worldData["isDefault"].as<bool>(),worldData["permissions"].as<vector<string>>());
            setWorldData(levelName, worldData1);
  }
}

bool PPGroup::setGroupPermission(string permission,string levelName)
{
    if (levelName.empty()) 
    {
        auto tempGroupData = get<0>(getData());
        tempGroupData["permissions"].push_back(permission);
        tuple<string,bool,vector<string>,vector<string>,YAML::Node> data1 = std::make_tuple<string,bool,vector<string>,vector<string>,YAML::Node>(tempGroupData["alias"].as<string>(),tempGroupData["isDefault"].as<bool>(),tempGroupData["inheritance"].as<vector<string>>(),tempGroupData["permissions"].as<vector<string>>(),tempGroupData["worlds"].as<YAML::Node>());
        setData(data1);
    } 
    else 
    {
        auto worldData = getWorldData(levelName);
        worldData["permissions"].push_back(permission);
        tuple<bool,vector<string>> worldData1 = make_tuple<bool,vector<string>>(worldData["isDefault"].as<bool>(),worldData["permissions"].as<vector<string>>());
        setWorldData(levelName, worldData1);
    }
    //plugin->updatePlayersInGroup(*this);
    
    return true;
}

void PPGroup::setNode(string node,std::variant<bool,int,float,double,string> value)
{
    auto tempGroupData = get<0>(getData());
    std::visit([&](auto&& val) {
        tempGroupData[node] = val;
    }, value);
    tuple<string,bool,vector<string>,vector<string>,YAML::Node> data1 = std::make_tuple<string,bool,vector<string>,vector<string>,YAML::Node>(tempGroupData["alias"].as<string>(),tempGroupData["isDefault"].as<bool>(),tempGroupData["inheritance"].as<vector<string>>(),tempGroupData["permissions"].as<vector<string>>(),tempGroupData["worlds"].as<YAML::Node>());
    setData(data1);
}

void PPGroup::setWorldData(string levelName,tuple<bool,vector<string>> worldData)
{
     YAML::Node data = get<0>(getData());

        if (!data["worlds"].IsNull() && !data["worlds"][levelName].IsNull()) {
            data["worlds"][levelName]["isDefault"] = get<0>(worldData);
            data["worlds"][levelName]["permissions"] = get<1>(worldData);
            tuple<string,bool,vector<string>,vector<string>,YAML::Node> data1 = std::make_tuple<string,bool,vector<string>,vector<string>,YAML::Node>(data["alias"].as<string>(),data["isDefault"].as<bool>(),data["inheritance"].as<vector<string>>(),data["permissions"].as<vector<string>>(),data["worlds"].as<YAML::Node>());
            setData(data1);
        }
}

void PPGroup::setWorldNode(string levelName,string node,variant<bool,vector<string>> value)
{
   YAML::Node worldData = getWorldData(levelName);

        // Update the node in the world data based on the type of value
        if (std::holds_alternative<bool>(value)) {
            worldData[node] = std::get<bool>(value);
        } else if (std::holds_alternative<std::vector<std::string>>(value)) {
            worldData[node] = YAML::Node(YAML::NodeType::Sequence);
            for (const auto& str : std::get<std::vector<std::string>>(value)) {
                worldData[node].push_back(str);
            }
        }

        // Prepare the tuple with isDefault (assumed bool) and permissions (vector<string>)
        bool isDefault = worldData["isDefault"].as<bool>();
        std::vector<std::string> permissions = worldData["permissions"].as<std::vector<std::string>>();
        auto worldTuple = std::make_tuple(isDefault, permissions);

        setWorldData(levelName, worldTuple);
}

void PPGroup::sortPermissions()
{
    YAML::Node tempGroupData = get<0>(getData());

        // Sort and remove duplicates from the permissions array
        if (tempGroupData["permissions"]) {
            std::vector<std::string> permissions = tempGroupData["permissions"].as<std::vector<std::string>>();
            std::set<std::string> uniquePermissions(permissions.begin(), permissions.end());
            permissions.assign(uniquePermissions.begin(), uniquePermissions.end());
            std::sort(permissions.begin(), permissions.end());
            tempGroupData["permissions"] = permissions;
        }

        // Check if multi-world permissions are enabled
        bool isMultiWorldPermsEnabled = YAML::LoadFile("plugins/PurePerms/config.yml")["enable-multiworld-perms"].as<bool>();
        if (isMultiWorldPermsEnabled && tempGroupData["worlds"]) {
            auto dimss = ll::service::bedrock::getLevel()->getDimensionManager().mDimensions;
            //std::function<bool(class Dimension&)> callback = [](
            /*for (const auto& world : getWorlds()) {
                std::string WorldName = world.getDisplayName();
                if (tempGroupData["worlds"][WorldName]) {
                    std::vector<std::string> worldPermissions = tempGroupData["worlds"][WorldName]["permissions"].as<std::vector<std::string>>();
                    std::set<std::string> uniqueWorldPermissions(worldPermissions.begin(), worldPermissions.end());
                    worldPermissions.assign(uniqueWorldPermissions.begin(), uniqueWorldPermissions.end());
                    std::sort(worldPermissions.begin(), worldPermissions.end());
                    tempGroupData["worlds"][WorldName]["permissions"] = worldPermissions;
                }
            }*/
            
        }
 tuple<string,bool,vector<string>,vector<string>,YAML::Node> data1 = std::make_tuple<string,bool,vector<string>,vector<string>,YAML::Node>(tempGroupData["alias"].as<string>(),tempGroupData["isDefault"].as<bool>(),tempGroupData["inheritance"].as<vector<string>>(),tempGroupData["permissions"].as<vector<string>>(),tempGroupData["worlds"].as<YAML::Node>());
        setData(data1);
}

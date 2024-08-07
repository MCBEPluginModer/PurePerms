#include "PPGroup.h"

void PPGroup::addParent(PPGroup* parent)
{
    if (this == &group || std::find(parentGroups.begin(), parentGroups.end(), group.getName()) != parentGroups.end()) {
        return false;
    }
    data = getData(); 
    get<YAML::Node>(data)["inheritance"].push_back(parent->getName());
    tuple<string,bool,vector<string>,vector<string>,YAML::Node> data1(getName(), get<YAML::Node>(data)["alias"].as<string>(),get<YAML::Node>(data)["isDefault"].as<bool>(),get<YAML::Node>(data)["inheritance"].as<vector<string>>(),get<YAML::Node>(data)["permissions"].as<vector<string>>(),get<YAML::Node>(data)["worlds"]);
    setData(data1);
    /*data = getData(); // Retrieve the current data
    data->push_back(group.getName()); // Add the group's name to the inheritance

    setData(data); // Update the data*/
    tuple<string,bool,vector<string>,vector<string>,YAML::Node> data;
    
    //plugin->updatePlayersInGroup(*this); // Notify the plugin to update players in this group

    return true;
}

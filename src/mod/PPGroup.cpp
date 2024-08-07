#include "PPGroup.h"

bool PPGroup::addParent(PPGroup* parent)
{
    /*if (this == parent || std::find(parents.begin(), parents.end(), parent->getName()) != parents.end()) {
        return false;
    }*/
    auto data = getData(); 
    get<0>(data)["inheritance"].push_back(parent->getName());
    tuple<string,bool,vector<string>,vector<string>,YAML::Node> data1 = std::make_tuple<string,bool,vector<string>,vector<string>,YAML::Node>(get<0>(data)["alias"].as<string>(),get<0>(data)["isDefault"].as<bool>(),get<0>(data)["inheritance"].as<vector<string>>(),get<0>(data)["permissions"].as<vector<string>>(),get<0>(data)["worlds"].as<YAML::Node>());
    setData(data1);
    /*data = getData(); // Retrieve the current data
    data->push_back(group.getName()); // Add the group's name to the inheritance

    setData(data); // Update the data*/
   // tuple<string,bool,vector<string>,vector<string>,YAML::Node> data;
    
    //plugin->updatePlayersInGroup(*this); // Notify the plugin to update players in this group

    return true;
}

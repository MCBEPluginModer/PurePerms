#include "PPGroup.h"

void PPGroup::addParent(PPGroup* parent)
{
    if (this == &group || std::find(parentGroups.begin(), parentGroups.end(), group.getName()) != parentGroups.end()) {
        return false;
    }

    data = getData(); // Retrieve the current data
    data->push_back(group.getName()); // Add the group's name to the inheritance

    setData(data); // Update the data

    //plugin->updatePlayersInGroup(*this); // Notify the plugin to update players in this group

    return true;
}

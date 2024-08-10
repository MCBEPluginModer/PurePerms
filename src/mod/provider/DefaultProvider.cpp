#include "DefaultProvider.h"
#include "../PPMessages.h"
#include <fstream>

DefaultProvider::DefaultProvider(mcpm::PurePerms* _plugin)
{
  plugin = _plugin;
  ifstream in("plugins/PurePerms/ranks.yaml"),in1("plugins/PurePerms/players.yaml");
  if (in.is_open() & in1.is_open())
  {
    in.close();
    in1.close();
    groups = YAML::LoadFile("plugins/PurePerms/ranks.yaml");
    if (groups.IsNull())
    {
        //throw plugin->getMessage("logger_messages.YAMLProvider_InvalidGroupsSettings");
    }
    players = YAML::LoadFile("plugins/PurePerms/players.yaml");
  }
  else
  {
    in.close();
    in1.close();
    
  }
}

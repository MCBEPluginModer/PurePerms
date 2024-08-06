#include <mc/world/actor/player/Player.h>
#include <string>
#include <vector>
#include <unordered_map>

class PPGroup; // Forward declaration

class ProviderInterface 
{

public:
    virtual ~ProviderInterface() = default;

    virtual std::unordered_map<std::string, std::string> getGroupData(const PPGroup& group) = 0;
    virtual std::vector<std::unordered_map<std::string, std::string>> getGroupsData() = 0;
    virtual std::unordered_map<std::string, std::string> getPlayerData(const Player* player) = 0;
    virtual std::vector<Player*> getUsers() = 0;
    virtual void setGroupData(const PPGroup& group, const std::unordered_map<std::string, std::string>& tempGroupData) = 0;
    virtual void setGroupsData(const std::vector<std::unordered_map<std::string, std::string>>& tempGroupsData) = 0;
    virtual void setPlayerData(const Player* player, const std::unordered_map<std::string, std::string>& tempPlayerData) = 0;
    virtual void close() = 0;
};

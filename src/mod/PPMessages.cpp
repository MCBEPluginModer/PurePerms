#include "PPMessages.h"
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <ll/api/Logger.h>

namespace fs = std::filesystem;

YAML::Node PPMessages::getNestedNode(const std::string& node) 
{
        YAML::Node currentNode = messages;
        std::stringstream ss(node);
        std::string segment;

        while (std::getline(ss, segment, '.')) {
            if (currentNode[segment]) {
                currentNode = currentNode[segment];
            } else {
                return YAML::Node();
            }
        }

        return currentNode;
}

std::string PPMessages::replaceString(std::string str, const std::string& from, const std::string& to) 
{
        size_t startPos = 0;
        while ((startPos = str.find(from, startPos)) != std::string::npos) {
            str.replace(startPos, from.length(), to);
            startPos += to.length();
        }
        return str;
}

PPMessages::PPMessages()
{
        registerLanguages();
        loadMessages();
}

void PPMessages::registerLanguages() 
{
        std::vector<std::string> result;

        for (auto& entry : fs::directory_iterator("plugins/PurePerms")) 
        {
            if (entry.is_regular_file()) 
            {
                std::string filename = entry.path().filename().string();
                if (filename.find("messages-") == 0 && filename.size() >= 6) {
                    result.push_back(filename.substr(9, 2));
                }
            }
        }

        langList = result;
}

std::string PPMessages::getMessage(const std::string& node, const std::vector<std::string>& vars) 
{
        YAML::Node msgNode = getNestedNode(node);

        if (msgNode) {
            std::string msg = msgNode.as<std::string>();
            for (size_t i = 0; i < vars.size(); ++i) {
                std::string placeholder = "%var" + std::to_string(i) + "%";
                msg = replaceString(msg, placeholder, vars[i]);
            }
            return msg;
        }

        return "";
}

#include <cctype>

// Функция для преобразования строки к нижнему регистру
std::string toLowerCase(const std::string& str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c){ return std::tolower(c); });
    return result;
}


void PPMessages::loadMessages()
{
        auto defaultLang = YAML::LoadFile("plugins/PurePerms/config.yml")["default-language"].as<string>();

    for (const auto& langName : langList) {
        if (toLowerCase(defaultLang) == langName) {
            language = langName;
            break;
        }
    }
    ll::Logger logger("PurePerms");
    if (language.empty())
    {
           logger.warn("Language resource " + defaultLang + " not found. Using default language resource by TheMrEnderBro.");
           language = "en";
    }
    std::ofstream fout("plugins/PurePerms/messages-" + language + ".yml");
    YAML::Node lng;
    fout << lng;
    fout.close();
    logger.info("Setting default language to \'" + defaultLang + "'/");
}

void PPMessages::reloadMessages()
{
   messages = YAML::LoadFile("plugins/PurePerms/messages-" + language + ".yml");
}

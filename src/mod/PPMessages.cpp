#include "PPMessages.h"

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

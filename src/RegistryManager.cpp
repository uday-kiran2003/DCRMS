#include "RegistryManager.h"
#include <fstream>
#include <sstream>

RegistryManager::RegistryManager(const std::string& filePath)
    : registryFilePath(filePath) {}

bool RegistryManager::loadRegistry() {
    std::ifstream file(registryFilePath);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        // Parse registry file format
    }

    file.close();
    return true;
}

bool RegistryManager::saveRegistry() {
    std::ofstream file(registryFilePath, std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& pair : registry) {
        // Write registry data to file
    }

    file.close();
    return true;
}

bool RegistryManager::addNode(const NodeRegistry& nodeReg) {
    registry[nodeReg.nodeId] = nodeReg;
    return saveRegistry();
}

bool RegistryManager::removeNode(const std::string& nodeId) {
    auto it = registry.find(nodeId);
    if (it != registry.end()) {
        registry.erase(it);
        return saveRegistry();
    }
    return false;
}

NodeRegistry RegistryManager::getNode(const std::string& nodeId) const {
    auto it = registry.find(nodeId);
    if (it != registry.end()) {
        return it->second;
    }
    return NodeRegistry();
}

const std::map<std::string, NodeRegistry>& RegistryManager::getAllNodes() const {
    return registry;
}

bool RegistryManager::updateNodeState(const std::string& nodeId, NodeState newState) {
    auto it = registry.find(nodeId);
    if (it != registry.end()) {
        it->second.state = newState;
        return saveRegistry();
    }
    return false;
}

bool RegistryManager::updateRedirectionRules(const std::string& nodeId, RedirectionMode mode,
                                            const std::vector<std::string>& priority) {
    auto it = registry.find(nodeId);
    if (it != registry.end()) {
        it->second.redirectionMode = mode;
        it->second.redirectionPriority = priority;
        return saveRegistry();
    }
    return false;
}

bool RegistryManager::nodeExists(const std::string& nodeId) const {
    return registry.find(nodeId) != registry.end();
}

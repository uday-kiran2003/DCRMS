#ifndef REGISTRY_MANAGER_H
#define REGISTRY_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include "Node.h"

/**
 * @struct NodeRegistry
 * @brief Data structure for node registry
 * Used for persistent storage of node information
 */
struct NodeRegistry {
    std::string nodeId;                        // Unique node identifier
    std::string password;                      // Node password
    NodeState state;                           // Current state
    RedirectionMode redirectionMode;           // Redirection mode
    std::vector<std::string> redirectionPriority; // Fallback nodes
    long long enrollmentTime;                  // When node was enrolled
    long long lastHeartbeat;                   // Last heartbeat timestamp
};

/**
 * @class RegistryManager
 * @brief Manages persistent storage of node registry
 * Requirement DCRMS_SR_01: Persistent System Registry
 * Uses .dat files for persistence
 */
class RegistryManager {
private:
    std::string registryFilePath;               // Path to registry.dat file
    std::map<std::string, NodeRegistry> registry; // In-memory registry

public:
    /**
     * @brief Constructor
     * @param filePath Path to registry file
     */
    RegistryManager(const std::string& filePath);

    /**
     * @brief Load registry from file
     * @return true if load successful
     */
    bool loadRegistry();

    /**
     * @brief Save registry to file
     * @return true if save successful
     */
    bool saveRegistry();

    /**
     * @brief Add node to registry
     * @param registry Node registry data
     * @return true if addition successful
     */
    bool addNode(const NodeRegistry& registry);

    /**
     * @brief Remove node from registry
     * @param nodeId Node identifier
     * @return true if removal successful
     */
    bool removeNode(const std::string& nodeId);

    /**
     * @brief Get node from registry
     * @param nodeId Node identifier
     * @return NodeRegistry data
     */
    NodeRegistry getNode(const std::string& nodeId) const;

    /**
     * @brief Get all nodes
     * @return Map of all nodes
     */
    const std::map<std::string, NodeRegistry>& getAllNodes() const;

    /**
     * @brief Update node state
     * @param nodeId Node identifier
     * @param newState New state
     * @return true if update successful
     */
    bool updateNodeState(const std::string& nodeId, NodeState newState);

    /**
     * @brief Update redirection rules
     * @param nodeId Node identifier
     * @param mode Redirection mode
     * @param priority Priority list
     * @return true if update successful
     */
    bool updateRedirectionRules(const std::string& nodeId, RedirectionMode mode,
                               const std::vector<std::string>& priority);

    /**
     * @brief Check if node exists
     * @param nodeId Node identifier
     * @return true if node exists
     */
    bool nodeExists(const std::string& nodeId) const;
};

#endif // REGISTRY_MANAGER_H

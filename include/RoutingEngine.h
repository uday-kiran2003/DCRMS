#ifndef ROUTING_ENGINE_H
#define ROUTING_ENGINE_H

#include "Node.h"
#include "ClientNode.h"
#include "Logger.h"
#include <map>
#include <vector>
#include <mutex>
#include <thread>

/**
 * @class RoutingEngine
 * @brief Central routing server managing all connections
 * Implements design pattern: Singleton Pattern
 * Core component handling all requirements:
 * - DCRMS_SR_01: System Registry
 * - DCRMS_SR_02: Node enrollment/de-enrollment
 * - DCRMS_SR_04: Three redirection trigger modes
 * - DCRMS_SR_05: State controller for runtime rule changes
 * - DCRMS_SR_06: State check on every session request
 * - DCRMS_SR_08: Telemetry logging
 */
class RoutingEngine {
private:
    static RoutingEngine* instance;
    static std::mutex instanceMutex;

    std::map<std::string, ClientNode*> registeredNodes;  // System Registry (DCRMS_SR_01)
    std::vector<std::string> redirectionRules;           // Active redirection rules
    int serverPort;                                       // Server listening port
    bool isRunning;                                       // Engine status
    std::mutex nodesMutex;                                // Thread safety for nodes
    std::thread heartbeatThread;                          // Heartbeat monitoring thread
    bool heartbeatActive;                                 // Heartbeat thread status
    int heartbeatInterval;                                // Heartbeat check interval (seconds)

    RoutingEngine();
    ~RoutingEngine();
    RoutingEngine(const RoutingEngine&) = delete;
    RoutingEngine& operator=(const RoutingEngine&) = delete;

    void monitorHeartbeat();                              // Monitor node health
    bool loadRegistryFromFile();                          // Load registry from persistence
    bool saveRegistryToFile();                            // Save registry to persistence

public:
    /**
     * @brief Get singleton instance
     * @return RoutingEngine instance
     */
    static RoutingEngine* getInstance();

    /**
     * @brief Initialize routing engine
     * @param port Server port
     * @return true if initialization successful
     */
    bool initialize(int port);

    /**
     * @brief Start routing engine
     * @return true if engine started
     */
    bool start();

    /**
     * @brief Stop routing engine
     * @return true if engine stopped
     */
    bool stop();

    /**
     * @brief Register a node
     * Requirement DCRMS_SR_02: Node enrollment
     * @param nodeId Node identifier
     * @param node ClientNode pointer
     * @return true if registration successful
     */
    bool registerNode(const std::string& nodeId, ClientNode* node);

    /**
     * @brief Deregister a node
     * Requirement DCRMS_SR_02: Node de-enrollment
     * @param nodeId Node identifier
     * @return true if deregistration successful
     */
    bool deregisterNode(const std::string& nodeId);

    /**
     * @brief Get node by ID
     * @param nodeId Node identifier
     * @return ClientNode pointer or nullptr
     */
    ClientNode* getNode(const std::string& nodeId) const;

    /**
     * @brief Authenticate node
     * Requirement: Node authentication before participation
     * @param nodeId Node identifier
     * @param password Node password
     * @return true if authentication successful
     */
    bool authenticateNode(const std::string& nodeId, const std::string& password);

    /**
     * @brief Perform state check for routing
     * Requirement DCRMS_SR_06: State check on every inbound session
     * Requirement DCRMS_SR_04: Three trigger modes
     * @param sourceNodeId Source node
     * @param targetNodeId Target node
     * @return Recommended target node ID (may be redirected)
     */
    std::string performStateCheck(const std::string& sourceNodeId,
                                 const std::string& targetNodeId);

    /**
     * @brief Update node redirection rules
     * Requirement DCRMS_SR_05: State controller for runtime changes
     * Requirement DCRMS_CL_02: Modify redirection without restart
     * @param nodeId Node identifier
     * @param mode Redirection mode
     * @param priority Priority list for fallback
     * @return true if update successful
     */
    bool updateRedirectionRules(const std::string& nodeId, RedirectionMode mode,
                               const std::vector<std::string>& priority);

    /**
     * @brief Change node state
     * Requirement: Admin has power to change state
     * @param nodeId Node identifier
     * @param newState New state
     * @return true if state change successful
     */
    bool changeNodeState(const std::string& nodeId, NodeState newState);

    /**
     * @brief Get all registered nodes
     * @return Map of registered nodes
     */
    const std::map<std::string, ClientNode*>& getRegisteredNodes() const;

    /**
     * @brief Check node status
     * @param nodeId Node identifier
     * @return Node state string
     */
    std::string getNodeStatus(const std::string& nodeId) const;

    /**
     * @brief Get all active sessions
     * @return Vector of active session info
     */
    std::vector<std::string> getActiveSessions() const;

    /**
     * @brief Purge inactive nodes
     * @param timeoutSeconds Timeout in seconds
     * @return Number of nodes purged
     */
    int purgeInactiveNodes(int timeoutSeconds);
};

#endif // ROUTING_ENGINE_H

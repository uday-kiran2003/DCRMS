#include "RoutingEngine.h"
#include "Credentials.h"
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

RoutingEngine* RoutingEngine::instance = nullptr;
std::mutex RoutingEngine::instanceMutex;

RoutingEngine::RoutingEngine()
    : serverPort(0), isRunning(false), heartbeatActive(false), heartbeatInterval(5) {}

RoutingEngine::~RoutingEngine() {
    if (isRunning) {
        stop();
    }
    for (auto& pair : registeredNodes) {
        delete pair.second;
    }
    registeredNodes.clear();
}

RoutingEngine* RoutingEngine::getInstance() {
    std::lock_guard<std::mutex> lock(instanceMutex);
    if (instance == nullptr) {
        instance = new RoutingEngine();
    }
    return instance;
}

bool RoutingEngine::initialize(int port) {
    serverPort = port;
    Logger* logger = Logger::getInstance();
    logger->operational("RoutingEngine initialized on port " + std::to_string(port));
    return true;
}

bool RoutingEngine::start() {
    if (isRunning) {
        Logger* logger = Logger::getInstance();
        logger->advisory("RoutingEngine already running");
        return false;
    }

    isRunning = true;
    heartbeatActive = true;
    Logger* logger = Logger::getInstance();
    logger->operational("RoutingEngine started on port " + std::to_string(serverPort));
    heartbeatThread = std::thread(&RoutingEngine::monitorHeartbeat, this);
    return true;
}

bool RoutingEngine::stop() {
    if (!isRunning) {
        return false;
    }

    isRunning = false;
    heartbeatActive = false;

    if (heartbeatThread.joinable()) {
        heartbeatThread.join();
    }

    Logger* logger = Logger::getInstance();
    logger->operational("RoutingEngine stopped");
    return true;
}

void RoutingEngine::monitorHeartbeat() {
    Logger* logger = Logger::getInstance();
    while (heartbeatActive) {
        std::this_thread::sleep_for(std::chrono::seconds(heartbeatInterval));
        std::lock_guard<std::mutex> lock(nodesMutex);

        for (auto& pair : registeredNodes) {
            ClientNode* node = pair.second;
            auto now = std::chrono::system_clock::now().time_since_epoch().count();
            auto timeDiff = (now - node->getLastHeartbeat()) / 1000000000;

            if (timeDiff > 30) {
                if (node->getState() != NodeState::OFFLINE) {
                    node->setState(NodeState::OFFLINE);
                    logger->critical("Node " + node->getNodeId() +
                                   " marked as OFFLINE (heartbeat timeout)");
                }
            }
        }
    }
}

bool RoutingEngine::registerNode(const std::string& nodeId, ClientNode* node) {
    std::lock_guard<std::mutex> lock(nodesMutex);
    Logger* logger = Logger::getInstance();

    if (registeredNodes.find(nodeId) != registeredNodes.end()) {
        logger->critical("Node " + nodeId + " already registered");
        return false;
    }

    registeredNodes[nodeId] = node;
    logger->operational("Node " + nodeId + " registered successfully");
    return true;
}

bool RoutingEngine::deregisterNode(const std::string& nodeId) {
    std::lock_guard<std::mutex> lock(nodesMutex);
    Logger* logger = Logger::getInstance();

    auto it = registeredNodes.find(nodeId);
    if (it == registeredNodes.end()) {
        logger->critical("Node " + nodeId + " not found in registry");
        return false;
    }

    delete it->second;
    registeredNodes.erase(it);
    logger->operational("Node " + nodeId + " deregistered successfully");
    return true;
}

ClientNode* RoutingEngine::getNode(const std::string& nodeId) const {
    std::lock_guard<std::mutex> lock(nodesMutex);
    auto it = registeredNodes.find(nodeId);
    if (it != registeredNodes.end()) {
        return it->second;
    }
    return nullptr;
}

bool RoutingEngine::authenticateNode(const std::string& nodeId, const std::string& password) {
    Logger* logger = Logger::getInstance();

    if (!Credentials::verifyNode(nodeId, password)) {
        logger->critical("Node " + nodeId + " authentication failed");
        return false;
    }

    ClientNode* node = getNode(nodeId);
    if (node) {
        node->setAuthenticated(true);
        logger->operational("Node " + nodeId + " authenticated successfully");
        return true;
    }

    return false;
}

std::string RoutingEngine::performStateCheck(const std::string& sourceNodeId,
                                            const std::string& targetNodeId) {
    Logger* logger = Logger::getInstance();
    logger->trace("State check: " + sourceNodeId + " -> " + targetNodeId);

    ClientNode* targetNode = getNode(targetNodeId);
    if (!targetNode) {
        logger->critical("Target node " + targetNodeId + " not found");
        return "";
    }

    NodeState targetState = targetNode->getState();

    if (targetState == NodeState::BUSY || targetState == NodeState::OUT_OF_COVERAGE ||
        targetState == NodeState::OFFLINE) {
        logger->operational("Target node " + targetNodeId + " unavailable (" +
                           targetNode->getStateString() + "), checking redirection rules");

        const auto& priority = targetNode->getRedirectionPriority();
        for (const auto& alternateNodeId : priority) {
            ClientNode* alternateNode = getNode(alternateNodeId);
            if (alternateNode && alternateNode->getState() == NodeState::AVAILABLE) {
                logger->operational("Session redirected from " + targetNodeId + " to " +
                                   alternateNodeId);
                return alternateNodeId;
            }
        }

        logger->critical("No available redirection target for " + targetNodeId);
        return "";
    }

    logger->trace("Target node " + targetNodeId + " is " + targetNode->getStateString());
    return targetNodeId;
}

bool RoutingEngine::updateRedirectionRules(const std::string& nodeId, RedirectionMode mode,
                                          const std::vector<std::string>& priority) {
    Logger* logger = Logger::getInstance();
    ClientNode* node = getNode(nodeId);

    if (!node) {
        logger->critical("Node " + nodeId + " not found for redirection update");
        return false;
    }

    node->setRedirectionMode(mode);
    node->setRedirectionPriority(priority);
    logger->operational("Redirection rules updated for " + nodeId +
                       " (DYNAMIC - no restart required)");
    return true;
}

bool RoutingEngine::changeNodeState(const std::string& nodeId, NodeState newState) {
    Logger* logger = Logger::getInstance();
    ClientNode* node = getNode(nodeId);

    if (!node) {
        logger->critical("Node " + nodeId + " not found for state change");
        return false;
    }

    node->setState(newState);
    return true;
}

const std::map<std::string, ClientNode*>& RoutingEngine::getRegisteredNodes() const {
    return registeredNodes;
}

std::string RoutingEngine::getNodeStatus(const std::string& nodeId) const {
    Logger* logger = Logger::getInstance();
    ClientNode* node = getNode(nodeId);

    if (!node) {
        logger->critical("Node " + nodeId + " not found");
        return "NOT_FOUND";
    }

    return node->getStateString();
}

std::vector<std::string> RoutingEngine::getActiveSessions() const {
    std::vector<std::string> sessions;
    std::lock_guard<std::mutex> lock(nodesMutex);

    for (const auto& pair : registeredNodes) {
        if (pair.second->isClientConnected()) {
            sessions.push_back(pair.first + ": " + pair.second->getStateString());
        }
    }

    return sessions;
}

int RoutingEngine::purgeInactiveNodes(int timeoutSeconds) {
    Logger* logger = Logger::getInstance();
    std::lock_guard<std::mutex> lock(nodesMutex);
    int purgedCount = 0;
    auto now = std::chrono::system_clock::now().time_since_epoch().count();

    auto it = registeredNodes.begin();
    while (it != registeredNodes.end()) {
        auto timeDiff = (now - it->second->getLastHeartbeat()) / 1000000000;
        if (timeDiff > timeoutSeconds && it->second->getState() == NodeState::OFFLINE) {
            logger->operational("Purging inactive node: " + it->first);
            delete it->second;
            it = registeredNodes.erase(it);
            purgedCount++;
        } else {
            ++it;
        }
    }

    logger->operational("Purged " + std::to_string(purgedCount) + " inactive nodes");
    return purgedCount;
}

#include "Node.h"
#include "Logger.h"
#include <chrono>

Node::Node(const std::string& id, const std::string& pwd)
    : nodeId(id), password(pwd), state(NodeState::AVAILABLE),
      redirectionMode(RedirectionMode::RESOURCE_OCCUPIED),
      isAuthenticated(false), failureCount(0), lastHeartbeat(0) {
    updateHeartbeat();
}

std::string Node::getNodeId() const {
    return nodeId;
}

NodeState Node::getState() const {
    return state;
}

void Node::setState(NodeState newState) {
    Logger* logger = Logger::getInstance();
    std::string oldStateStr = getStateString();
    state = newState;
    std::string newStateStr = getStateString();
    logger->operational("Node " + nodeId + " state changed from " + oldStateStr +
                       " to " + newStateStr);
}

std::string Node::getStateString() const {
    switch (state) {
        case NodeState::AVAILABLE:
            return "AVAILABLE";
        case NodeState::BUSY:
            return "BUSY";
        case NodeState::OUT_OF_COVERAGE:
            return "OUT_OF_COVERAGE";
        case NodeState::OFFLINE:
            return "OFFLINE";
        default:
            return "UNKNOWN";
    }
}

bool Node::isNodeAuthenticated() const {
    return isAuthenticated;
}

void Node::setAuthenticated(bool auth) {
    isAuthenticated = auth;
}

RedirectionMode Node::getRedirectionMode() const {
    return redirectionMode;
}

void Node::setRedirectionMode(RedirectionMode mode) {
    redirectionMode = mode;
    Logger* logger = Logger::getInstance();
    logger->advisory("Node " + nodeId + " redirection mode set to " +
                    getRedirectionModeString());
}

std::string Node::getRedirectionModeString() const {
    switch (redirectionMode) {
        case RedirectionMode::ABSOLUTE_OVERRIDE:
            return "ABSOLUTE_OVERRIDE";
        case RedirectionMode::RESPONSE_TIMEOUT:
            return "RESPONSE_TIMEOUT";
        case RedirectionMode::RESOURCE_OCCUPIED:
            return "RESOURCE_OCCUPIED";
        default:
            return "UNKNOWN";
    }
}

void Node::setRedirectionPriority(const std::vector<std::string>& priority) {
    redirectionPriority = priority;
    Logger* logger = Logger::getInstance();
    std::string priorityStr = "";
    for (const auto& p : priority) {
        priorityStr += p + " ";
    }
    logger->advisory("Node " + nodeId + " redirection priority set: " + priorityStr);
}

const std::vector<std::string>& Node::getRedirectionPriority() const {
    return redirectionPriority;
}

void Node::updateHeartbeat() {
    lastHeartbeat = std::chrono::system_clock::now().time_since_epoch().count();
}

long long Node::getLastHeartbeat() const {
    return lastHeartbeat;
}

void Node::incrementFailureCount() {
    failureCount++;
    Logger* logger = Logger::getInstance();
    logger->advisory("Node " + nodeId + " failure count: " + std::to_string(failureCount));
}

void Node::resetFailureCount() {
    failureCount = 0;
}

int Node::getFailureCount() const {
    return failureCount;
}

bool Node::performHandshake() {
    Logger* logger = Logger::getInstance();
    logger->trace("Node " + nodeId + " performing handshake");
    return true;
}

bool Node::handleIncomingConnection() {
    Logger* logger = Logger::getInstance();
    logger->trace("Node " + nodeId + " handling incoming connection");
    return true;
}

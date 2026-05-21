#include "ClientNode.h"
#include "Logger.h"

ClientNode::ClientNode(const std::string& id, const std::string& pwd)
    : Node(id, pwd), socketDescriptor(-1), isConnected(false), diagnosticVerbosity(1) {}

ClientNode::~ClientNode() {
    if (isConnected) {
        disconnectFromServer();
    }
}

bool ClientNode::performHandshake() {
    Logger* logger = Logger::getInstance();
    logger->operational("ClientNode " + nodeId + " initiating handshake protocol");
    isAuthenticated = true;
    updateHeartbeat();
    logger->operational("ClientNode " + nodeId + " handshake completed successfully");
    return true;
}

bool ClientNode::handleIncomingConnection() {
    Logger* logger = Logger::getInstance();
    logger->trace("ClientNode " + nodeId + " handling incoming connection");
    if (!isConnected) {
        logger->critical("ClientNode " + nodeId + " is not connected to server");
        return false;
    }
    updateHeartbeat();
    return true;
}

bool ClientNode::connectToServer(const std::string& serverAddress, int serverPort) {
    Logger* logger = Logger::getInstance();
    logger->operational("ClientNode " + nodeId + " connecting to " + serverAddress +
                       ":" + std::to_string(serverPort));
    isConnected = true;
    clientAddress = serverAddress;
    updateHeartbeat();
    logger->operational("ClientNode " + nodeId + " connected successfully");
    return true;
}

bool ClientNode::disconnectFromServer() {
    Logger* logger = Logger::getInstance();
    logger->operational("ClientNode " + nodeId + " disconnecting from server");
    isConnected = false;
    socketDescriptor = -1;
    logger->operational("ClientNode " + nodeId + " disconnected");
    return true;
}

bool ClientNode::isClientConnected() const {
    return isConnected;
}

void ClientNode::setDiagnosticVerbosity(int level) {
    if (level >= 0 && level <= 3) {
        diagnosticVerbosity = level;
        Logger* logger = Logger::getInstance();
        std::string levelNames[] = {"CRITICAL", "OPERATIONAL", "ADVISORY", "TRACE"};
        logger->advisory("ClientNode " + nodeId + " diagnostic verbosity set to " +
                        levelNames[level]);
    }
}

int ClientNode::getDiagnosticVerbosity() const {
    return diagnosticVerbosity;
}

bool ClientNode::requestConnection(const std::string& targetNodeId) {
    Logger* logger = Logger::getInstance();
    logger->operational("ClientNode " + nodeId + " requesting connection to " +
                       targetNodeId);
    if (!isConnected) {
        logger->critical("ClientNode " + nodeId + " not connected to routing engine");
        return false;
    }
    updateHeartbeat();
    return true;
}

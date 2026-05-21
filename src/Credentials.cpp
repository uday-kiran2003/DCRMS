#include "Credentials.h"

// Static member initialization
std::map<std::string, std::string> Credentials::users;
std::map<std::string, std::string> Credentials::nodes;

/**
 * @brief Initialize all predefined credentials
 * Admin users and node credentials are hardcoded as per requirement
 */
void Credentials::initialize() {
    // Predefined admin credentials
    // Admin has all access to control and monitor
    users["admin"] = "admin123";
    users["superadmin"] = "superadmin@2026";

    // Predefined node credentials
    // Nodes: A, B, C, D with different modes (BUSY, OUT_OF_COVERAGE, CONTINUOUSLY_FORWARD)
    nodes["NODE_A"] = "nodeA_pass123";
    nodes["NODE_B"] = "nodeB_pass123";
    nodes["NODE_C"] = "nodeC_pass123";
    nodes["NODE_D"] = "nodeD_pass123";
}

/**
 * @brief Verify admin/user credentials
 * Requirement: Login and logout are mandatory with authentication
 * @param username Username to verify
 * @param password Password to verify
 * @return true if credentials match, false otherwise
 */
bool Credentials::verifyUser(const std::string& username, const std::string& password) {
    auto it = users.find(username);
    if (it != users.end() && it->second == password) {
        return true;
    }
    return false;
}

/**
 * @brief Verify node credentials
 * Requirement: Nodes must authenticate before participation
 * @param nodeId Node identifier to verify
 * @param password Password to verify
 * @return true if credentials match, false otherwise
 */
bool Credentials::verifyNode(const std::string& nodeId, const std::string& password) {
    auto it = nodes.find(nodeId);
    if (it != nodes.end() && it->second == password) {
        return true;
    }
    return false;
}

/**
 * @brief Get all registered nodes
 * @return Reference to nodes map
 */
const std::map<std::string, std::string>& Credentials::getNodes() {
    return nodes;
}

/**
 * @brief Get all registered users
 * @return Reference to users map
 */
const std::map<std::string, std::string>& Credentials::getUsers() {
    return users;
}

#include "AdminConsole.h"
#include "RoutingEngine.h"
#include "Credentials.h"
#include <iostream>
#include <iomanip>

AdminConsole::AdminConsole()
    : currentAdmin(""), isLoggedIn(false), engine(RoutingEngine::getInstance()),
      logger(Logger::getInstance()) {}

AdminConsole::~AdminConsole() {}

void AdminConsole::start() {
    Credentials::initialize();
    logger->operational("Admin Console started");

    std::cout << "\n====================================\n";
    std::cout << "DCRMS - Admin Console\n";
    std::cout << "====================================\n\n";

    while (true) {
        if (!isLoggedIn) {
            std::string username, password;
            std::cout << "Enter username: ";
            std::getline(std::cin, username);
            std::cout << "Enter password: ";
            std::getline(std::cin, password);

            if (authenticateAdmin(username, password)) {
                currentAdmin = username;
                isLoggedIn = true;
                std::cout << "\nLogin successful! Welcome " << username << "\n\n";
            } else {
                std::cout << "\nInvalid credentials! Try again.\n\n";
                logger->critical("Failed admin login attempt for user: " + username);
                continue;
            }
        }

        showMainMenu();
    }
}

bool AdminConsole::authenticateAdmin(const std::string& username, const std::string& password) {
    logger->operational("Admin authentication attempt for: " + username);
    return Credentials::verifyUser(username, password);
}

void AdminConsole::logout() {
    logger->operational("Admin " + currentAdmin + " logged out");
    isLoggedIn = false;
    currentAdmin = "";
    std::cout << "\nLogged out successfully!\n";
}

void AdminConsole::showMainMenu() {
    std::cout << "\n========== ADMIN MENU ==========\n";
    std::cout << "1. Register Node\n";
    std::cout << "2. Deregister Node\n";
    std::cout << "3. View All Nodes\n";
    std::cout << "4. Change Node State\n";
    std::cout << "5. Update Redirection Rules\n";
    std::cout << "6. View Active Sessions\n";
    std::cout << "7. Manual Override\n";
    std::cout << "8. Server Configuration\n";
    std::cout << "9. Logout\n";
    std::cout << "================================\n";
    std::cout << "Select option: ";

    int choice;
    std::cin >> choice;
    std::cin.ignore();

    switch (choice) {
        case 1: registerNewNode(); break;
        case 2: deregisterNode(); break;
        case 3: viewAllNodes(); break;
        case 4: changeNodeState(); break;
        case 5: updateRedirectionRules(); break;
        case 6: monitorActiveSessions(); break;
        case 7: manualOverride(); break;
        case 8: reconfigureServer(); break;
        case 9: logout(); break;
        default:
            std::cout << "Invalid option!\n";
    }
}

void AdminConsole::registerNewNode() {
    std::cout << "\n--- Register New Node ---\n";
    std::string nodeId, password;
    std::cout << "Enter Node ID (e.g., NODE_A): ";
    std::getline(std::cin, nodeId);
    std::cout << "Enter Node Password: ";
    std::getline(std::cin, password);

    if (Credentials::verifyNode(nodeId, password)) {
        ClientNode* newNode = new ClientNode(nodeId, password);
        if (engine->registerNode(nodeId, newNode)) {
            std::cout << "Node " << nodeId << " registered successfully!\n";
            logger->operational("Admin " + currentAdmin + " registered node " + nodeId);
        }
    } else {
        std::cout << "Invalid node credentials!\n";
    }
}

void AdminConsole::deregisterNode() {
    std::cout << "\n--- Deregister Node ---\n";
    std::string nodeId;
    std::cout << "Enter Node ID to deregister: ";
    std::getline(std::cin, nodeId);

    if (engine->deregisterNode(nodeId)) {
        std::cout << "Node " << nodeId << " deregistered successfully!\n";
        logger->operational("Admin " + currentAdmin + " deregistered node " + nodeId);
    } else {
        std::cout << "Failed to deregister node!\n";
    }
}

void AdminConsole::viewAllNodes() {
    std::cout << "\n--- All Registered Nodes ---\n";
    const auto& nodes = engine->getRegisteredNodes();

    if (nodes.empty()) {
        std::cout << "No nodes registered.\n";
        return;
    }

    std::cout << std::left << std::setw(15) << "Node ID"
              << std::setw(15) << "State"
              << std::setw(20) << "Redirection Mode"
              << std::setw(15) << "Connected\n";
    std::cout << std::string(65, '-') << "\n";

    for (const auto& pair : nodes) {
        std::cout << std::left << std::setw(15) << pair.first
                  << std::setw(15) << pair.second->getStateString()
                  << std::setw(20) << pair.second->getRedirectionModeString()
                  << std::setw(15) << (pair.second->isClientConnected() ? "Yes" : "No")
                  << "\n";
    }
}

void AdminConsole::changeNodeState() {
    std::cout << "\n--- Change Node State ---\n";
    std::string nodeId;
    std::cout << "Enter Node ID: ";
    std::getline(std::cin, nodeId);

    std::cout << "Select new state:\n";
    std::cout << "0. AVAILABLE\n";
    std::cout << "1. BUSY\n";
    std::cout << "2. OUT_OF_COVERAGE\n";
    std::cout << "3. OFFLINE\n";
    std::cout << "Choice: ";

    int stateChoice;
    std::cin >> stateChoice;
    std::cin.ignore();

    NodeState newState = static_cast<NodeState>(stateChoice);
    if (engine->changeNodeState(nodeId, newState)) {
        std::cout << "Node state changed successfully!\n";
        logger->operational("Admin " + currentAdmin + " changed " + nodeId + " state");
    } else {
        std::cout << "Failed to change node state!\n";
    }
}

void AdminConsole::updateRedirectionRules() {
    std::cout << "\n--- Update Redirection Rules ---\n";
    std::string nodeId;
    std::cout << "Enter Node ID: ";
    std::getline(std::cin, nodeId);

    std::cout << "Select redirection mode:\n";
    std::cout << "0. ABSOLUTE_OVERRIDE\n";
    std::cout << "1. RESPONSE_TIMEOUT\n";
    std::cout << "2. RESOURCE_OCCUPIED\n";
    std::cout << "Choice: ";

    int modeChoice;
    std::cin >> modeChoice;
    std::cin.ignore();

    RedirectionMode mode = static_cast<RedirectionMode>(modeChoice);

    std::cout << "Enter fallback nodes (space-separated, e.g., NODE_B NODE_C): ";
    std::string priorityInput;
    std::getline(std::cin, priorityInput);

    std::vector<std::string> priority;
    std::istringstream iss(priorityInput);
    std::string node;
    while (iss >> node) {
        priority.push_back(node);
    }

    if (engine->updateRedirectionRules(nodeId, mode, priority)) {
        std::cout << "Redirection rules updated successfully (DYNAMIC - no restart needed)!\n";
        logger->operational("Admin " + currentAdmin + " updated redirection rules for " + nodeId);
    } else {
        std::cout << "Failed to update redirection rules!\n";
    }
}

void AdminConsole::monitorActiveSessions() {
    std::cout << "\n--- Active Sessions ---\n";
    auto sessions = engine->getActiveSessions();

    if (sessions.empty()) {
        std::cout << "No active sessions.\n";
        return;
    }

    std::cout << "Active Sessions:\n";
    for (size_t i = 0; i < sessions.size(); ++i) {
        std::cout << (i + 1) << ". " << sessions[i] << "\n";
    }
}

void AdminConsole::reconfigureServer() {
    std::cout << "\n--- Server Configuration ---\n";
    std::cout << "Server is currently running on port 9000\n";
    std::cout << "Heartbeat interval: 5 seconds\n";
    std::cout << "Node timeout: 30 seconds\n";
    logger->operational("Admin " + currentAdmin + " accessed server configuration");
}

void AdminConsole::manualOverride() {
    std::cout << "\n--- Manual Override ---\n";
    std::string sourceNode, targetNode;
    std::cout << "Enter source node ID: ";
    std::getline(std::cin, sourceNode);
    std::cout << "Enter target node ID: ";
    std::getline(std::cin, targetNode);

    std::string result = engine->performStateCheck(sourceNode, targetNode);
    if (result.empty()) {
        std::cout << "Connection cannot be established!\n";
    } else {
        std::cout << "Session will be routed to: " << result << "\n";
        logger->operational("Admin " + currentAdmin + " performed manual override");
    }
}

bool AdminConsole::isAdminLoggedIn() const {
    return isLoggedIn;
}

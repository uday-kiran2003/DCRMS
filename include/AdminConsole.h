#ifndef ADMIN_CONSOLE_H
#define ADMIN_CONSOLE_H

#include <string>
#include "RoutingEngine.h"
#include "Logger.h"

/**
 * @class AdminConsole
 * @brief Administrative interface for system management
 * Requirement: Admin has all access to control and monitor
 * Requirement DCRMS_SR_07: Administrative console
 * Features:
 * - State change capability
 * - Server reconfiguration
 * - Manual override for routing
 */
class AdminConsole {
private:
    std::string currentAdmin;        // Currently logged-in admin
    bool isLoggedIn;                 // Login status
    RoutingEngine* engine;           // Reference to routing engine
    Logger* logger;                  // Reference to logger

    void displayMenu();
    void handleNodeManagement();
    void handleStateManagement();
    void handleLogManagement();
    void handleMonitoring();
    void handleConfiguration();
    void clearScreen();

public:
    /**
     * @brief Constructor
     */
    AdminConsole();

    /**
     * @brief Destructor
     */
    ~AdminConsole();

    /**
     * @brief Start admin console
     * Requirement: Login and logout are mandatory
     */
    void start();

    /**
     * @brief Authenticate admin user
     * Requirement: Login and logout are mandatory
     * @param username Username
     * @param password Password
     * @return true if authentication successful
     */
    bool authenticateAdmin(const std::string& username, const std::string& password);

    /**
     * @brief Logout current admin
     * Requirement: Logout is mandatory
     */
    void logout();

    /**
     * @brief Display main menu
     */
    void showMainMenu();

    /**
     * @brief Register new node
     */
    void registerNewNode();

    /**
     * @brief Deregister node
     */
    void deregisterNode();

    /**
     * @brief View all nodes
     */
    void viewAllNodes();

    /**
     * @brief Change node state
     * Requirement: Admin has power to state change
     */
    void changeNodeState();

    /**
     * @brief Update redirection rules
     * Requirement: New rules updated without restart (dynamic)
     */
    void updateRedirectionRules();

    /**
     * @brief View system logs
     */
    void viewSystemLogs();

    /**
     * @brief Monitor active sessions
     */
    void monitorActiveSessions();

    /**
     * @brief Reconfigure server
     * Requirement: Server reconfiguration facility
     */
    void reconfigureServer();

    /**
     * @brief Manual override for redirection
     * Requirement: Facility for manual override
     */
    void manualOverride();

    /**
     * @brief Check if admin is logged in
     * @return Login status
     */
    bool isAdminLoggedIn() const;
};

#endif // ADMIN_CONSOLE_H

#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <string>
#include <map>

/**
 * @class Credentials
 * @brief Manages predefined credentials for authentication
 * Follows Singleton pattern with predefined users and nodes
 */
class Credentials {
private:
    static std::map<std::string, std::string> users;
    static std::map<std::string, std::string> nodes;

public:
    /**
     * @brief Initialize predefined credentials
     */
    static void initialize();

    /**
     * @brief Verify user credentials
     * @param username Username
     * @param password Password
     * @return true if credentials are valid, false otherwise
     */
    static bool verifyUser(const std::string& username, const std::string& password);

    /**
     * @brief Verify node credentials
     * @param nodeId Node identifier
     * @param password Node password
     * @return true if credentials are valid, false otherwise
     */
    static bool verifyNode(const std::string& nodeId, const std::string& password);

    /**
     * @brief Get all registered node IDs
     * @return Map of node IDs and passwords
     */
    static const std::map<std::string, std::string>& getNodes();

    /**
     * @brief Get all registered users
     * @return Map of user IDs and passwords
     */
    static const std::map<std::string, std::string>& getUsers();
};

#endif // CREDENTIALS_H

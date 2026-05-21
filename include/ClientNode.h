#ifndef CLIENT_NODE_H
#define CLIENT_NODE_H

#include "Node.h"

/**
 * @class ClientNode
 * @brief Represents a client endpoint node in the network
 * Inherits from Node (OOP concept: Inheritance)
 * Implements requirement DCRMS_CL_04: Diagnostic module with 4 verbosity levels
 */
class ClientNode : public Node {
private:
    int socketDescriptor;           // Socket descriptor for server connection
    std::string clientAddress;      // Address of connected server
    bool isConnected;               // Connection status
    int diagnosticVerbosity;        // 0=CRITICAL, 1=OPERATIONAL, 2=ADVISORY, 3=TRACE

public:
    /**
     * @brief Constructor
     * @param id Node identifier
     * @param pwd Node password
     */
    ClientNode(const std::string& id, const std::string& pwd);

    /**
     * @brief Destructor
     */
    ~ClientNode() override;

    /**
     * @brief Perform handshake protocol with engine
     * Requirement DCRMS_CL_01: Handshake Protocol
     * @return true if handshake successful
     */
    bool performHandshake() override;

    /**
     * @brief Handle incoming connection from routing engine
     * Requirement DCRMS_CL_03: Handle direct incoming sessions
     * @return true if connection handled
     */
    bool handleIncomingConnection() override;

    /**
     * @brief Connect to server
     * @param serverAddress Server address
     * @param serverPort Server port
     * @return true if connection successful
     */
    bool connectToServer(const std::string& serverAddress, int serverPort);

    /**
     * @brief Disconnect from server
     * @return true if disconnection successful
     */
    bool disconnectFromServer();

    /**
     * @brief Check if client is connected
     * @return Connection status
     */
    bool isClientConnected() const;

    /**
     * @brief Set diagnostic verbosity level
     * Requirement DCRMS_CL_04: Four levels of diagnostic verbosity
     * @param level Verbosity level (0-3): CRITICAL, OPERATIONAL, ADVISORY, TRACE
     */
    void setDiagnosticVerbosity(int level);

    /**
     * @brief Get diagnostic verbosity level
     * @return Verbosity level (0-3)
     */
    int getDiagnosticVerbosity() const;

    /**
     * @brief Request connection to target node
     * @param targetNodeId Target node identifier
     * @return true if request successful
     */
    bool requestConnection(const std::string& targetNodeId);
};

#endif // CLIENT_NODE_H

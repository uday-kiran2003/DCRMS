#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

/**
 * @enum NodeState
 * @brief Possible states of a node
 * Maps to requirement: Node can be BUSY, OUT_OF_COVERAGE, AVAILABLE, OFFLINE
 */
enum class NodeState {
    AVAILABLE = 0,        // Node is available for connections
    BUSY = 1,             // Node is busy (processing requests)
    OUT_OF_COVERAGE = 2,  // Node is out of coverage (temporarily unreachable)
    OFFLINE = 3           // Node is offline (not connected)
};

/**
 * @enum RedirectionMode
 * @brief Redirection trigger modes
 * Implements requirement DCRMS_SR_04: Three distinct modes
 */
enum class RedirectionMode {
    ABSOLUTE_OVERRIDE = 0,  // Always redirect regardless of target state
    RESPONSE_TIMEOUT = 1,   // Redirect on timeout
    RESOURCE_OCCUPIED = 2   // Redirect when target is busy/occupied
};

/**
 * @class Node
 * @brief Base class representing a network node
 * Implements OOP concept: Inheritance (base class for ClientNode)
 * Requirement: DCRMS_CL_01 - Handshake Protocol
 */
class Node {
protected:
    std::string nodeId;                        // Unique node identifier
    std::string password;                      // Node password for authentication
    NodeState state;                           // Current state of node
    RedirectionMode redirectionMode;           // Current redirection mode
    std::vector<std::string> redirectionPriority; // Fallback node IDs
    bool isAuthenticated;                      // Authentication status
    int failureCount;                          // Number of consecutive failures
    long long lastHeartbeat;                   // Timestamp of last heartbeat

public:
    /**
     * @brief Constructor
     * @param id Node identifier
     * @param pwd Node password
     */
    Node(const std::string& id, const std::string& pwd);

    /**
     * @brief Virtual destructor
     */
    virtual ~Node() = default;

    /**
     * @brief Get node ID
     * @return Node identifier
     */
    std::string getNodeId() const;

    /**
     * @brief Get node state
     * @return Current node state
     */
    NodeState getState() const;

    /**
     * @brief Set node state
     * @param newState New state to set
     */
    void setState(NodeState newState);

    /**
     * @brief Get node state as string
     * @return String representation of state
     */
    std::string getStateString() const;

    /**
     * @brief Check if node is authenticated
     * @return Authentication status
     */
    bool isNodeAuthenticated() const;

    /**
     * @brief Set authentication status
     * @param auth Authentication status
     */
    void setAuthenticated(bool auth);

    /**
     * @brief Get redirection mode
     * @return Current redirection mode
     */
    RedirectionMode getRedirectionMode() const;

    /**
     * @brief Set redirection mode
     * Requirement DCRMS_CL_02: Modify redirection logic at runtime without restart
     * @param mode New redirection mode
     */
    void setRedirectionMode(RedirectionMode mode);

    /**
     * @brief Get redirection mode as string
     * @return String representation of mode
     */
    std::string getRedirectionModeString() const;

    /**
     * @brief Set redirection priority
     * @param priority Vector of fallback node IDs
     */
    void setRedirectionPriority(const std::vector<std::string>& priority);

    /**
     * @brief Get redirection priority
     * @return Vector of fallback node IDs
     */
    const std::vector<std::string>& getRedirectionPriority() const;

    /**
     * @brief Update last heartbeat timestamp
     */
    void updateHeartbeat();

    /**
     * @brief Get last heartbeat time
     * @return Timestamp of last heartbeat
     */
    long long getLastHeartbeat() const;

    /**
     * @brief Increment failure count
     */
    void incrementFailureCount();

    /**
     * @brief Reset failure count
     */
    void resetFailureCount();

    /**
     * @brief Get failure count
     * @return Number of consecutive failures
     */
    int getFailureCount() const;

    /**
     * @brief Virtual method for handshake protocol
     * Requirement DCRMS_CL_01: Perform handshake with engine
     * @return true if handshake successful
     */
    virtual bool performHandshake();

    /**
     * @brief Virtual method for handling incoming connection
     * Requirement DCRMS_CL_03: Handle incoming sessions
     * @return true if connection handled successfully
     */
    virtual bool handleIncomingConnection();
};

#endif // NODE_H

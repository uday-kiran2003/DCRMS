#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <chrono>

/**
 * @enum LogLevel
 * @brief Logging verbosity levels as per requirement DCRMS_CL_04
 */
enum class LogLevel {
    CRITICAL = 0,      // Critical errors only
    OPERATIONAL = 1,   // Important operations
    ADVISORY = 2,      // Advisory information
    TRACE = 3          // Detailed trace information
};

/**
 * @class Logger
 * @brief Singleton logger for system-wide logging
 * Implements design pattern: Singleton Pattern
 * Features high-precision timestamps for telemetry logs (DCRMS_SR_08)
 */
class Logger {
private:
    static Logger* instance;
    static std::mutex instanceMutex;
    std::ofstream logFile;
    LogLevel currentLevel;
    std::mutex fileMutex;

    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string getLevelString(LogLevel level) const;
    std::string getCurrentTimestamp() const;

public:
    /**
     * @brief Get singleton instance of Logger
     * @return Logger instance
     */
    static Logger* getInstance();

    /**
     * @brief Initialize logger
     * @param logFilePath Path to log file
     * @param level Initial logging level
     * @return true if initialization successful
     */
    bool initialize(const std::string& logFilePath, LogLevel level = LogLevel::OPERATIONAL);

    /**
     * @brief Set logging level dynamically
     * @param level New logging level
     */
    void setLogLevel(LogLevel level);

    /**
     * @brief Log a message with specified level
     * @param level Log level
     * @param message Message to log
     */
    void log(LogLevel level, const std::string& message);

    /**
     * @brief Log critical message
     * @param message Message to log
     */
    void critical(const std::string& message);

    /**
     * @brief Log operational message
     * @param message Message to log
     */
    void operational(const std::string& message);

    /**
     * @brief Log advisory message
     * @param message Message to log
     */
    void advisory(const std::string& message);

    /**
     * @brief Log trace message
     * @param message Message to log
     */
    void trace(const std::string& message);
};

#endif // LOGGER_H

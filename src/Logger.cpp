#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <sstream>

// Static member initialization
Logger* Logger::instance = nullptr;
std::mutex Logger::instanceMutex;

/**
 * @brief Private constructor
 */
Logger::Logger() : currentLevel(LogLevel::OPERATIONAL) {}

/**
 * @brief Destructor - closes log file
 */
Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

/**
 * @brief Get singleton instance (thread-safe)
 * Implements Singleton design pattern
 * @return Static Logger instance
 */
Logger* Logger::getInstance() {
    std::lock_guard<std::mutex> lock(instanceMutex);
    if (instance == nullptr) {
        instance = new Logger();
    }
    return instance;
}

/**
 * @brief Initialize logger with file path and level
 * @param logFilePath Path to log file
 * @param level Initial logging level
 * @return true if initialization successful
 */
bool Logger::initialize(const std::string& logFilePath, LogLevel level) {
    logFile.open(logFilePath, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << logFilePath << std::endl;
        return false;
    }
    currentLevel = level;
    log(LogLevel::OPERATIONAL, "Logger initialized with level: " + getLevelString(level));
    return true;
}

/**
 * @brief Set logging level dynamically
 * @param level New logging level
 */
void Logger::setLogLevel(LogLevel level) {
    currentLevel = level;
    log(LogLevel::OPERATIONAL, "Log level changed to: " + getLevelString(level));
}

/**
 * @brief Convert LogLevel enum to string
 * Requirement DCRMS_CL_04: Four verbosity levels
 * @param level LogLevel enum
 * @return String representation
 */
std::string Logger::getLevelString(LogLevel level) const {
    switch (level) {
        case LogLevel::CRITICAL:
            return "CRITICAL";
        case LogLevel::OPERATIONAL:
            return "OPERATIONAL";
        case LogLevel::ADVISORY:
            return "ADVISORY";
        case LogLevel::TRACE:
            return "TRACE";
        default:
            return "UNKNOWN";
    }
}

/**
 * @brief Get current timestamp with millisecond precision
 * Requirement DCRMS_SR_08: High-precision timestamps for telemetry
 * @return Formatted timestamp string
 */
std::string Logger::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
       << "." << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

/**
 * @brief Log message with specified level
 * Requirement DCRMS_SR_08: Generate telemetry log
 * @param level Log level
 * @param message Message to log
 */
void Logger::log(LogLevel level, const std::string& message) {
    // Skip if level is below current threshold
    if (static_cast<int>(level) > static_cast<int>(currentLevel)) {
        return;
    }

    std::lock_guard<std::mutex> lock(fileMutex);
    std::string timestamp = getCurrentTimestamp();
    std::string levelStr = getLevelString(level);
    std::string logEntry = "[" + timestamp + "] [" + levelStr + "] " + message;

    if (logFile.is_open()) {
        logFile << logEntry << std::endl;
        logFile.flush();
    }

    std::cout << logEntry << std::endl;
}

/**
 * @brief Log critical message
 * @param message Message to log
 */
void Logger::critical(const std::string& message) {
    log(LogLevel::CRITICAL, message);
}

/**
 * @brief Log operational message
 * @param message Message to log
 */
void Logger::operational(const std::string& message) {
    log(LogLevel::OPERATIONAL, message);
}

/**
 * @brief Log advisory message
 * @param message Message to log
 */
void Logger::advisory(const std::string& message) {
    log(LogLevel::ADVISORY, message);
}

/**
 * @brief Log trace message
 * @param message Message to log
 */
void Logger::trace(const std::string& message) {
    log(LogLevel::TRACE, message);
}

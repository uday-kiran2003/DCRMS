# Dynamic Connection Routing and Management System (DCRMS)

## Overview
DCRMS is a high-performance network orchestration framework designed to manage and redirect communication sessions between multiple endpoint nodes using a centralized Routing Engine.

## Project Structure
```
DCRMS/
├── include/          # Header files
├── src/              # Implementation files
├── config/           # Configuration files
├── logs/             # Log storage
├── data/             # .dat files for persistence
├── Makefile          # Build configuration
└── README.md         # Project documentation
```

## Key Features
- **Dynamic Connection Routing**: Automatic redirection based on node states
- **Node Authentication**: Secure credential-based access
- **Three Redirection Modes**: ABSOLUTE_OVERRIDE, RESPONSE_TIMEOUT, RESOURCE_OCCUPIED
- **Real-time Monitoring**: Heartbeat-based node status tracking
- **Admin Console**: Full control and configuration capabilities
- **Multi-level Logging**: CRITICAL, OPERATIONAL, ADVISORY, TRACE levels
- **Session Management**: Dynamic session handling without restart
- **Automatic Failover**: Intelligent fallback mechanisms

## Node States
- **AVAILABLE**: Ready to accept connections
- **BUSY**: Currently processing requests
- **OUT_OF_COVERAGE**: Temporarily unreachable
- **OFFLINE**: Not connected

## Build Instructions
```bash
make clean
make build
make run
```

## Requirements
- Linux/Ubuntu OS
- G++ compiler (C++11 or higher)
- POSIX threads library
- 4GB RAM minimum
- Stable network connection

## Admin Credentials (Predefined)
- **Username**: admin | **Password**: admin123
- **Username**: superadmin | **Password**: superadmin@2026

## Node Credentials (Predefined)
- **NODE_A**: nodeA_pass123
- **NODE_B**: nodeB_pass123
- **NODE_C**: nodeC_pass123
- **NODE_D**: nodeD_pass123

## System Architecture

### Components
1. **RoutingEngine**: Central server managing all connections
2. **Node/ClientNode**: Network endpoints with state management
3. **AdminConsole**: Administrative interface for system control
4. **Logger**: Singleton logger with configurable verbosity
5. **RegistryManager**: Persistent node registry storage
6. **Credentials**: Authentication and authorization manager

## Usage

### Starting the System
```bash
./bin/dcrms
```

### Admin Login
- Enter username and password when prompted
- Admin has full system control

### Admin Operations
- Register/Deregister nodes
- Change node states
- Update redirection rules dynamically
- Monitor active sessions
- View system logs
- Manual override for routing
- Reconfigure server settings

## Author
Udaya Kiran J

## Version
V.01 (21-05-2026)

## Approver
Athrva

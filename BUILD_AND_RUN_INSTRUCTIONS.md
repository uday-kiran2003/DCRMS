# DCRMS - Build and Run Instructions

## Prerequisites
- Linux/Ubuntu system
- G++ compiler (C++11 or higher)
- POSIX threads library
- Make utility

## Installation of Prerequisites

### On Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install g++
sudo apt-get install make
```

### On RHEL/CentOS:
```bash
sudo yum groupinstall "Development Tools"
sudo yum install gcc-c++
sudo yum install make
```

## Directory Structure

```
DCRMS/
├── include/              # Header files
│   ├── Credentials.h
│   ├── Logger.h
│   ├── Node.h
│   ├── ClientNode.h
│   ├── RoutingEngine.h
│   ├── AdminConsole.h
│   └── RegistryManager.h
├── src/                  # Implementation files
│   ├── main.cpp
│   ├── Credentials.cpp
│   ├── Logger.cpp
│   ├── Node.cpp
│   ├── ClientNode.cpp
│   ├── RoutingEngine.cpp
│   ├── AdminConsole.cpp
│   └── RegistryManager.cpp
├── logs/                 # Log files (created at runtime)
├── data/                 # .dat files for persistence
├── Makefile              # Build configuration
└── README.md             # Documentation
```

## Build Instructions

### Step 1: Navigate to project directory
```bash
cd DCRMS
```

### Step 2: Clean previous builds (optional)
```bash
make clean
```

### Step 3: Create directories
```bash
make directories
```

### Step 4: Build the project
```bash
make build
```

Or simply:
```bash
make
```

### Step 5: Verify successful build
```bash
ls -la bin/
# Should see: dcrms executable file
```

## Run Instructions

### Start the DCRMS System
```bash
./bin/dcrms
```

Or use Makefile:
```bash
make run
```

## Sample Login Credentials

### Admin Credentials (Predefined):
```
Username: admin
Password: admin123

OR

Username: superadmin
Password: superadmin@2026
```

### Node Credentials (Predefined):
```
NODE_A   : nodeA_pass123
NODE_B   : nodeB_pass123
NODE_C   : nodeC_pass123
NODE_D   : nodeD_pass123
```

## Sample Output & Usage Walkthrough

### 1. System Startup Output
```
====================================
DCRMS - Admin Console
====================================

Enter username: admin
Enter password: admin123

Login successful! Welcome admin

========== ADMIN MENU ==========
1. Register Node
2. Deregister Node
3. View All Nodes
4. Change Node State
5. Update Redirection Rules
6. View Active Sessions
7. Manual Override
8. Server Configuration
9. Logout
================================
Select option:
```

### 2. Register Nodes
```
Select option: 1

--- Register New Node ---
Enter Node ID (e.g., NODE_A): NODE_A
Enter Node Password: nodeA_pass123
Node NODE_A registered successfully!
```

Repeat for NODE_B, NODE_C, NODE_D

### 3. View All Registered Nodes
```
Select option: 3

--- All Registered Nodes ---
Node ID         State           Redirection Mode    Connected   
-------------------------------------------------------------
NODE_A          AVAILABLE       RESOURCE_OCCUPIED   No
NODE_B          AVAILABLE       RESOURCE_OCCUPIED   No
NODE_C          AVAILABLE       RESOURCE_OCCUPIED   No
NODE_D          AVAILABLE       RESOURCE_OCCUPIED   No
```

### 4. Connect Nodes
```
Select option: 1

--- Register New Node ---
Enter Node ID (e.g., NODE_A): NODE_A
Enter Node Password: nodeA_pass123
Node NODE_A registered successfully!
```

### 5. Change Node State (Set NODE_B to BUSY)
```
Select option: 4

--- Change Node State ---
Enter Node ID: NODE_B
Select new state:
0. AVAILABLE
1. BUSY
2. OUT_OF_COVERAGE
3. OFFLINE
Choice: 1
Node state changed successfully!
```

### 6. Update Redirection Rules for NODE_B
```
Select option: 5

--- Update Redirection Rules ---
Enter Node ID: NODE_B
Select redirection mode:
0. ABSOLUTE_OVERRIDE
1. RESPONSE_TIMEOUT
2. RESOURCE_OCCUPIED
Choice: 2
Enter fallback nodes (space-separated, e.g., NODE_B NODE_C): NODE_C NODE_D
Redirection rules updated successfully (DYNAMIC - no restart needed)!
```

### 7. Manual Override (Route NODE_A to NODE_B)
```
Select option: 7

--- Manual Override ---
Enter source node ID: NODE_A
Enter target node ID: NODE_B
Session will be routed to: NODE_C
```
*Note: Since NODE_B is BUSY, system redirects to NODE_C (first available in fallback)*

### 8. View Active Sessions
```
Select option: 6

--- Active Sessions ---
No active sessions.
```

### 9. Server Configuration
```
Select option: 8

--- Server Configuration ---
Server is currently running on port 9000
Heartbeat interval: 5 seconds
Node timeout: 30 seconds
```

## Log File Output

Logs are stored in `logs/dcrms.log`:

```
[2026-05-21 18:11:03.256] [OPERATIONAL] Logger initialized with level: OPERATIONAL
[2026-05-21 18:11:03.257] [OPERATIONAL] RoutingEngine initialized on port 9000
[2026-05-21 18:11:03.258] [OPERATIONAL] RoutingEngine started on port 9000
[2026-05-21 18:11:03.259] [OPERATIONAL] Admin Console started
[2026-05-21 18:11:05.123] [OPERATIONAL] Admin authentication attempt for: admin
[2026-05-21 18:11:06.456] [OPERATIONAL] Node NODE_A registered successfully
[2026-05-21 18:11:07.789] [OPERATIONAL] Node NODE_B registered successfully
[2026-05-21 18:11:08.234] [OPERATIONAL] Node NODE_B state changed from AVAILABLE to BUSY
[2026-05-21 18:11:09.567] [OPERATIONAL] Redirection rules updated for NODE_B (DYNAMIC - no restart required)
[2026-05-21 18:11:10.890] [OPERATIONAL] State check: NODE_A -> NODE_B
[2026-05-21 18:11:11.123] [OPERATIONAL] Target node NODE_B unavailable (BUSY), checking redirection rules
[2026-05-21 18:11:12.456] [OPERATIONAL] Session redirected from NODE_B to NODE_C
[2026-05-21 18:11:13.789] [OPERATIONAL] Admin admin logged out
```

## Complete Workflow Example

### Terminal Session:
```bash
$ cd DCRMS
$ make clean
Cleaning...
$ make
Building DCRMS...
Build complete: ./bin/dcrms
$ make run

====================================
DCRMS - Admin Console
====================================

Enter username: admin
Enter password: admin123

Login successful! Welcome admin

========== ADMIN MENU ==========
1. Register Node
2. Deregister Node
3. View All Nodes
4. Change Node State
5. Update Redirection Rules
6. View Active Sessions
7. Manual Override
8. Server Configuration
9. Logout
================================
Select option: 1

--- Register New Node ---
Enter Node ID (e.g., NODE_A): NODE_A
Enter Node Password: nodeA_pass123
Node NODE_A registered successfully!

[Return to menu and repeat for NODE_B, NODE_C, NODE_D]

Select option: 3

--- All Registered Nodes ---
Node ID         State           Redirection Mode    Connected   
-------------------------------------------------------------
NODE_A          AVAILABLE       RESOURCE_OCCUPIED   No
NODE_B          AVAILABLE       RESOURCE_OCCUPIED   No
NODE_C          AVAILABLE       RESOURCE_OCCUPIED   No
NODE_D          AVAILABLE       RESOURCE_OCCUPIED   No

Select option: 4

--- Change Node State ---
Enter Node ID: NODE_B
Select new state:
0. AVAILABLE
1. BUSY
2. OUT_OF_COVERAGE
3. OFFLINE
Choice: 1
Node state changed successfully!

Select option: 5

--- Update Redirection Rules ---
Enter Node ID: NODE_B
Select redirection mode:
0. ABSOLUTE_OVERRIDE
1. RESPONSE_TIMEOUT
2. RESOURCE_OCCUPIED
Choice: 2
Enter fallback nodes (space-separated): NODE_C NODE_D
Redirection rules updated successfully (DYNAMIC - no restart needed)!

Select option: 7

--- Manual Override ---
Enter source node ID: NODE_A
Enter target node ID: NODE_B
Session will be routed to: NODE_C

Select option: 9

Logged out successfully!
```

## Cleanup Commands

### Remove build artifacts:
```bash
make clean
```

### Remove logs and data:
```bash
make distclean
```

### View logs:
```bash
cat logs/dcrms.log
```

## Troubleshooting

### Compilation Errors:
- Ensure G++ is installed: `g++ --version`
- Check C++11 support: `g++ --std=c++11 --version`
- Verify all header files are in `include/` directory

### Runtime Errors:
- Check `logs/dcrms.log` for detailed error messages
- Ensure `logs/` and `data/` directories exist
- Verify permissions: `chmod -R 755 DCRMS/`

### Missing Pthread Library:
```bash
sudo apt-get install libpthread-stubs0-dev
```

## Additional Notes

- The system runs on **port 9000** (configurable in main.cpp)
- Heartbeat monitoring checks every **5 seconds**
- Nodes are marked OFFLINE after **30 seconds** without heartbeat
- Log level can be changed at runtime (CRITICAL, OPERATIONAL, ADVISORY, TRACE)
- All state changes are logged with high-precision timestamps
- Dynamic redirection rules updates require **NO RESTART**

## Commit Reference
- Repository: https://github.com/uday-kiran2003/DCRMS
- Branch: main
- Latest Commit: AdminConsole implementation


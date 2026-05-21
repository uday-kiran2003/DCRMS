#!/bin/bash

# DCRMS Quick Start Script
# This script automates the build and run process

echo "================================================"
echo "DCRMS - Dynamic Connection Routing & Management"
echo "================================================"
echo ""

# Check if g++ is installed
if ! command -v g++ &> /dev/null; then
    echo "ERROR: G++ compiler not found!"
    echo "Install it using: sudo apt-get install build-essential g++"
    exit 1
fi

# Check if make is installed
if ! command -v make &> /dev/null; then
    echo "ERROR: Make utility not found!"
    echo "Install it using: sudo apt-get install make"
    exit 1
fi

echo "[1/4] Cleaning previous builds..."
make clean > /dev/null 2>&1

echo "[2/4] Creating directories..."
mkdir -p include src obj bin logs data

echo "[3/4] Building DCRMS..."
if make build > /dev/null 2>&1; then
    echo "✓ Build successful!"
else
    echo "✗ Build failed! Check Makefile and source files."
    exit 1
fi

echo "[4/4] Starting DCRMS System..."
echo ""
echo "================================================"
echo "DCRMS System Starting..."
echo "================================================"
echo ""
echo "Default Credentials:"
echo "  Admin:    username=admin / password=admin123"
echo "  Nodes:    NODE_A to NODE_D with respective passwords"
echo ""
echo "Server running on: PORT 9000"
echo "Logs location:     logs/dcrms.log"
echo ""
echo "================================================"
echo ""

./bin/dcrms


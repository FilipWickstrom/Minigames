#!/bin/bash

# Check which OS
detect_os() {
    case "$(uname -s)" in
        Linux*)               echo "linux" ;;
        Darwin*)              echo "macos" ;;
        CYGWIN*|MINGW*|MSYS*) echo "windows" ;;
        *)                    echo "unknown" ;;
    esac
}

OS_NAME=$(detect_os)
echo "Detected OS: $OS_NAME"

# Early out if something went wrong
if [ "$OS_NAME" = "unknown" ]; then
    echo "Unsupported OS... Exiting script..."
    exit 1
fi

# Clean project
echo "Cleaning project"

if [ "$OS_NAME" = "windows" ]; then
    PREMAKE_PATH="Vendor/Premake/Windows/premake5.exe"
elif [ "$OS_NAME" = "linux" ]; then
    PREMAKE_PATH="Vendor/Premake/Linux/premake5"
elif [ "$OS_NAME" = "macos" ]; then
    PREMAKE_PATH="Vendor/Premake/Macos/premake5"
fi

if [ -f "$PREMAKE_PATH" ]; then
    "$PREMAKE_PATH" clean
else
    echo "Premake not found at $PREMAKE_PATH"
fi

# Removing premake
if [ -d "Vendor/Premake" ]; then
    echo "Removing Vendor/Premake directory..."
    rm -rf Vendor/Premake
fi

# Pause at the end
echo
read -p "CleanProject.sh: Done"

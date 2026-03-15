#!/bin/bash

# Premake version
VERSION="5.0.0-beta8"

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

# Early out if something went wrong
if [ "$OS_NAME" = "unknown" ]; then
    echo "Unsupported OS... Exiting script..."
    exit 1
fi

# Project directory
PROJECT_DIR=$(pwd)

# Download premake for the first time
if [ "$OS_NAME" = "windows" ]; then
    PREMAKE_PATH="Vendor/Premake/Windows"
    PREMAKE_FILE="premake5.exe"
    ARCHIVE="premake-${VERSION}-windows.zip"

elif [ "$OS_NAME" = "linux" ]; then
    PREMAKE_PATH="Vendor/Premake/Linux"
    PREMAKE_FILE="premake5"
    ARCHIVE="premake-${VERSION}-linux.tar.gz"

elif [ "$OS_NAME" = "macos" ]; then
    PREMAKE_PATH="Vendor/Premake/Macos"
    PREMAKE_FILE="premake5"
    ARCHIVE="premake-${VERSION}-macosx.tar.gz"
fi

# Full path to the premake file
FULL_PATH="$PREMAKE_PATH/$PREMAKE_FILE"

# Download premake if file not existing
if [ ! -f "$FULL_PATH" ]; then
    mkdir -p "$PREMAKE_PATH"
    cd "$PREMAKE_PATH"

    URL="https://github.com/premake/premake-core/releases/download/v${VERSION}/${ARCHIVE}"
    echo "Downloading $ARCHIVE..."
    curl -LO "$URL"

    # Extract
    if [[ "$ARCHIVE" == *.zip ]]; then
        unzip -o "$ARCHIVE"
    else
        tar -xzf "$ARCHIVE"
    fi

    # Remove packed file
    rm "$ARCHIVE"

    # Make sure that it is executable
    chmod +x premake5

    cd "$PROJECT_DIR"
fi


# Create solution
if [ "$OS_NAME" = "windows" ]; then
    echo "Running Premake for Windows (vs2026)..."
    ./Vendor/Premake/Windows/premake5.exe vs2026

elif [ "$OS_NAME" = "linux" ]; then
    echo "Running Premake for Linux (gmake)..."
    ./Vendor/Premake/Linux/premake5 gmake

elif [ "$OS_NAME" = "macos" ]; then
    echo "Running Premake for macOS (xcode4)..."
    ./Vendor/Premake/Macos/premake5 xcode4
fi

# Pause at the end
echo
read -p "BuildProject.sh: Done"
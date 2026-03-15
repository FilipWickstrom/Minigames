# BioshockHackingGame
Cloning the Bioshock hacking game

# Tested platforms
| OS         | Version   | Compilation using   |
|------------|-----------|---------------------|
| Windows    | 11        | Visual Studio 2022  |
| WSL        | 2         | gmake               |
| Linux Mint | 22.2      | gmake               |

# How to download
You can download it using Git with the command:
> git clone --recursive https://github.com/FilipWickstrom/BioshockHackingGame.git

# Build project on Windows (Visual Studio)
1. > ./BuildProject.sh
2. > BioshockHackingGame.sln
3. > Build Solution
4. > Run debugger or go to ./Game.exe (inside Build/Bin/ReleaseOrDebug/Game/)

# Build project on Linux
1. > ./BuildProject.sh
2. > make
3. > ./Game (inside Build/Bin/Release_Or_Debug/Game/)

# Libraries
* [premake](https://github.com/premake/premake-core/) Simple build configuration.
* [raylib](https://github.com/raysan5/raylib) A simple and easy-to-use library to enjoy videogames programming 

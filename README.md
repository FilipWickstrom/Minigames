# Mini games
Trying to create some fun minigames from some of my favorite games

# Tested platforms
| OS         | Version   | Compilation using   |
|------------|-----------|---------------------|
| Windows    | 11        | Visual Studio 2026  |
| WSL        | 2         | gmake               |
| Linux Mint | 22.2      | gmake               |

# How to download
You can download it using Git with the command:
> git clone --recursive https://github.com/FilipWickstrom/Minigames.git

# Build project on Windows (Visual Studio)
1. > ./BuildProject.sh
2. > Minigames.sln
3. > Build Solution
4. > Run in visual studio or start using _Game_/Build/Bin/_ReleaseOrDebug_/_GameName.exe_

# Build project on Linux
1. > ./BuildProject.sh
2. > make
3. > _Game_/Build/Bin/_ReleaseOrDebug_/_GameName_)

# Libraries
* [premake](https://github.com/premake/premake-core/) Simple build configuration.
* [raylib](https://github.com/raysan5/raylib) A simple and easy-to-use library to enjoy videogames programming 
* [ImGui](https://github.com/ocornut/imgui) Simple Intermediate Gui for debugging 
* [rlImGui](https://github.com/raylib-extras/rlImGui) Some extra features to make Raylib and ImGui to work together

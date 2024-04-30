# CVCS Text Editor

This is a text editor inspired by vim. The funtion is to save files compressed and encrypted. The files are saved in a binary format and can be opened and edited by the editor.

## Installation

### WSL2

1. Install WSL2 (in admin powershell)

   ```powershell
   wsl --install
   ```

1. Install Ubuntu 20.04

   ```powershell
   wsl --set-version Ubuntu-20.04 2
   ```

### C++ and ncurses (Ubuntu 20.04)

1. Install c++ build tools

   ```bash
   sudo apt-get update
   sudo apt-get install build-essential
   ```

1. Install ncurses

   ```bash
   sudo apt-get install libncurses5-dev libncursesw5-dev
   ```

### C++ and ncurses (Fedora)

1. Install c++ build tools

   ```bash
   sudo dnf install gcc-c++
   ```

1. Install ncurses

   ```bash
   sudo dnf install ncurses-devel
   ```

### Build

1. Clone the repository

   ```bash
   git clone https://github.com/msosav/CVCS-Text-Editor
   ```

1. Build the project

   ```bash
   cd CVCS-Text-Editor
   make
   ```

## Usage

1. Run the editor

   ```bash
   ./cvcs <filename>
   ```

## References

- [vimperor](https://github.com/SirIsaacNeutron/vimperor?tab=readme-ov-file)

# Image Processing Project

This project focuses on image processing algorithms for real-time applications. Follow the steps below to set up, build, and run the project on various operating systems.

## Prerequisites

- Install [CMake](https://cmake.org/download/) on your system.
- A compatible C++ compiler:
  - **Linux**: GCC or Clang
  - **macOS**: Xcode command-line tools
  - **Windows**: Visual Studio with C++ build tools

## How to Build and Run

### 1. Clone the Repository
Open a terminal or command prompt in the directory where you want to clone the repository and run:
```bash
git clone https://github.com/ramyak-sharma/image_processing.git
cd image_processing
```

### 2. Create a Build Directory
Create a build directory to store compiled files:

```bash
mkdir build
```
### 3. Generate Build Files with CMake
Use CMake to set up the build system.

Linux/macOS:
```bash
cmake -S . -B build
```

Windows:
```bash
cmake -S . -B build -G "Visual Studio 16 2019"  # Adjust to your Visual Studio version
```

### 4. Build the Project
Move to the build directory and compile the project:

Linux/macOS:
```bash
cd build
make
```
Windows: Open the generated .sln file in Visual Studio, set the configuration to Release, and build the project.

### 5. Run the Program
Go back to the main project directory and run the output binary:

Linux/macOS:
```bash
cd ..
./output
```
Windows: Run the executable file (output.exe) from either the terminal or the Windows file explorer.

### Troubleshooting
CMake Errors: Ensure CMake is installed and the correct version is used.

Build Failures: Verify all dependencies are correctly installed and consult the logs for specific errors.

Windows Users: Ensure Visual Studio with C++ build tools is installed. Adjust the CMake generator in Step 3 if you’re using a different Visual Studio version.
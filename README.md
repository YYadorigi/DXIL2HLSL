# DXIL2HLSL

This project aims to provide translation of DXIL (SM 6.x) codes to original HLSL shaders which are commonly used in UE5 projects.

## Building

### Dependencies

Requires CMake 3.10+ and C++17 support

### Build

   ```bash
   cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
   cmake --build build --config Release
   ```

## RenderDoc Plugin Integration

### Configurations

| Field        | Value                                                        |
| ------------ | ------------------------------------------------------------ |
| Name         | DXIL Decompiler                                              |
| Tool Type    | Custom Tool                                                  |
| Executable   | Absolute path of `DXIL2HLSL.bat`, typically `./build/Release/DXIL2HLSL.bat` |
| Command Line | {input_file}                                                 |
| Input/Output | DXIL/HLSL                                                    |


# HLSL Decompiler

This project translates DXBC (SM 5.x or lower), DXIL (SM 6.x), and SPIR-V codes into the original HLSL shaders commonly used in Unreal Engine 5 projects.

## Building

### Dependencies

Requires CMake 3.10+ and C++17 support

### Build

   ```bash
   cmake -B build -S .
   cmake --build build --config Release
   ```

## RenderDoc Plugin Integration

### DXBC -> HLSL

| Field        | Value                                                        |
| ------------ | ------------------------------------------------------------ |
| Name         | DXBC Decompiler                                              |
| Tool Type    | Custom Tool                                                  |
| Executable   | Absolute path of `HLSLDecompiler.bat`, typically `./build/Release/HLSLDecompiler.bat` |
| Command Line | {input_file} -dxbc                                           |
| Input/Output | DXBC/HLSL                                                    |

### DXIL -> HLSL

| Field        | Value                                                        |
| ------------ | ------------------------------------------------------------ |
| Name         | DXIL Decompiler                                              |
| Tool Type    | Custom Tool                                                  |
| Executable   | Absolute path of `HLSLDecompiler.bat`, typically `./build/Release/HLSLDecompiler.bat` |
| Command Line | {input_file} -dxil                                           |
| Input/Output | DXIL/HLSL                                                    |

### SPIR-V -> HLSL

| Field        | Value                                                        |
| ------------ | ------------------------------------------------------------ |
| Name         | SPIR-V Decompiler                                            |
| Tool Type    | Custom Tool                                                  |
| Executable   | Absolute path of `HLSLDecompiler.bat`, typically `./build/Release/HLSLDecompiler.bat` |
| Command Line | {input_file} -spirv                                          |
| Input/Output | SPIR-V/HLSL                                                  |


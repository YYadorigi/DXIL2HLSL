#include <iostream>
#include <filesystem>
#include <string>
#include <cstdlib>

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    // Validate arguments
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input.dxil> [output.hlsl]\n";
        return 1;
    }

    // Get executable directory and tool paths
    fs::path exePath = fs::absolute(argv[0]).parent_path();
    fs::path dxilSpirv = exePath / "dxil-spirv.exe";
    fs::path spirvCross = exePath / "spirv-cross.exe";

    if (!fs::exists(dxilSpirv) || !fs::exists(spirvCross)) {
        std::cerr << "Error: Required tools not found in executable directory\n";
        return 1;
    }

    // Set input and output paths
    fs::path inputFile = fs::absolute(argv[1]);

    fs::path tempOutput = inputFile;
    fs::path outputFile = (argc > 2) ? fs::absolute(argv[2]) : tempOutput.replace_extension(".hlsl");

    fs::path tempSpirv = inputFile;
    fs::path spirvFile = tempSpirv.replace_extension(".spv");

    try {
        // DXIL -> SPIRV
        std::string cmd1 = "cmd /c \"\"" + dxilSpirv.string() + "\" \"" + inputFile.string() + "\" --output \"" + spirvFile.string() + "\"\"";
        // std::cout << "Executing: " << cmd1 << std::endl;
        int result1 = std::system(cmd1.c_str());
        if (result1 != 0) {
            std::cerr << "dxil-spirv failed with exit code: " << result1 << std::endl;
            return 1;
        }

        // SPIRV -> HLSL
        std::string cmd2 = "cmd /c \"\"" + spirvCross.string() + "\" \"" + spirvFile.string() + "\" --output \"" + outputFile.string() + "\" --hlsl --shader-model 60 \"";
        // std::cout << "Executing: " << cmd2 << std::endl;
        int result2 = std::system(cmd2.c_str());
        if (result2 != 0) {
            std::cerr << "spirv-cross failed with exit code: " << result2 << std::endl;
            return 1;
        }

        fs::remove(tempSpirv);
        // std::cout << "Successfully generated HLSL file: " << outputFile.string() << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
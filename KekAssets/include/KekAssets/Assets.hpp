//
// Created by vladk on 27.10.2026.
//

#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace Kek::Assets
{
    struct Image
    {
        int width = 0;
        int height = 0;
        std::vector<unsigned char> pixels;
    };

    void SetRoot(std::filesystem::path root);
    [[nodiscard]] const std::filesystem::path& GetRoot();

    [[nodiscard]] std::string ReadText(const std::filesystem::path& relativePath);
    
    [[nodiscard]] Image ReadImage(const std::filesystem::path& relativePath);
}

//
// Created by vladk on 27.10.2026.
//

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "KekAssets/Assets.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace Kek::Assets
{
    namespace
    {
        std::filesystem::path root = KEK_DEFAULT_ASSETS_DIR;
    }

    void SetRoot(std::filesystem::path newRoot) { root = std::move(newRoot); }
    const std::filesystem::path& GetRoot() { return root; }

    std::string ReadText(const std::filesystem::path& relativePath)
    {
        const auto fullPath = root / relativePath;

        std::ifstream file(fullPath, std::ios::binary);
        if (!file) throw std::runtime_error("Cannot open asset: " + fullPath.string());

        std::ostringstream content;
        content << file.rdbuf();
        return content.str();
    }

    Image ReadImage(const std::filesystem::path& relativePath)
    {
        const auto fullPath = root / relativePath;

        stbi_set_flip_vertically_on_load(1);

        int width = 0, height = 0, channels = 0;
        unsigned char* data = stbi_load(fullPath.string().c_str(), &width, &height, &channels, 4);
        if (data == nullptr)
            throw std::runtime_error("Cannot load image " + fullPath.string() + ": " + stbi_failure_reason());

        Image image;
        image.width = width;
        image.height = height;
        image.pixels.assign(data, data + static_cast<size_t>(width) * height * 4);

        stbi_image_free(data);
        return image;
    }
}
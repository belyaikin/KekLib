//
// Created by vladk on 01.11.2026.
//

#include "KekGraphics/ShaderLibrary.hpp"
#include "KekAssets/Assets.hpp"
#include "KekGraphics/OpenGL/ObjectFactory.hpp"

namespace Kek::Graphics
{
    using OpenGL::ObjectFactory;
    using OpenGL::Objects::Shader;
    using OpenGL::Objects::ShaderProgram;

    std::shared_ptr<ShaderProgram> ShaderLibrary::Load(const std::string& vertexPath, const std::string& fragmentPath)
    {
        const std::string key = vertexPath + '|' + fragmentPath;
        if (const auto found = cache.find(key); found != cache.end())
        {
            if (auto alive = found->second.lock())
                return alive;
        }

        const std::string vertexSource = Assets::ReadText(vertexPath);

        const std::string fragmentSource = Assets::ReadText(fragmentPath);

        const Shader vertex = ObjectFactory::CreateShader(Shader::Type::Vertex, vertexSource.c_str());
        const Shader fragment = ObjectFactory::CreateShader(Shader::Type::Fragment, fragmentSource.c_str());

        auto program = std::make_shared<ShaderProgram> (ObjectFactory::CreateShaderProgram({&vertex, &fragment}));
        cache[key] = program;
        return program;
    }
}
//
// Created by readmao on 24.10.2026.
//
#include <exception>
#include <iostream>
#include <memory>
#include <utility>

#include "KekApplication/Application.hpp"
#include "KekGraphics/OpenGL/Context.hpp"
#include "KekGraphics/OpenGL/ObjectFactory.hpp"

using namespace Kek::Graphics::OpenGL;

struct ShaderCheck final : Kek::World::Component
{
    void Start() override
    {
        const char* vertexSource = R"(#version 330 core
layout(location = 0) in vec3 aPosition;
void main() { gl_Position = vec4(aPosition, 1.0); })";

        const char* fragmentSource = R"(#version 330 core
out vec4 FragColor;
uniform float uBrightness;
void main() { FragColor = vec4(vec3(uBrightness), 1.0); })";

        const auto vertex = ObjectFactory::CreateShader(Shader::Type::Vertex, vertexSource);
        const auto fragment = ObjectFactory::CreateShader(Shader::Type::Fragment, fragmentSource);
        const auto program = ObjectFactory::CreateShaderProgram({&vertex, &fragment});

        Context::UseProgram(program);
        program.SetFloat("uBrightness", 0.5f);

        std::cout << "valid program linked, id = " << program.GetId() << '\n';

        try
        {
            auto broken = ObjectFactory::CreateShader(Shader::Type::Fragment, "this is not glsl");
        }
        catch (const std::exception& e)
        {
            std::cout << "broken shader rejected, log:\n" << e.what() << '\n';
        }

        std::cout << "glGetError: " << glGetError() << " (expected 0)\n";
    }
};

int main()
{
    auto root = std::make_unique<Kek::World::Node>();
    root->AttachComponent<ShaderCheck>();

    Kek::Application app(Kek::Platform::WindowSettings{800, 600, "KekLib - 03 Shaders"}, std::move(root));
    app.Run();
    return 0;
}
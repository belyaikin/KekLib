//
// Created by readmao on 24.10.2026.
//
#include <iostream>
#include <memory>
#include <utility>

#include "KekApplication/Application.hpp"
#include "KekGraphics/OpenGL/Context.hpp"
#include "KekGraphics/OpenGL/ObjectFactory.hpp"

using namespace Kek::Graphics::OpenGL;

// Start() is called after OpenGL is loaded, so GL objects can be created here.
struct GLObjectsCheck final : Kek::World::Component
{
    void Start() override
    {
        GLuint first = 0;
        GLuint second = 0;

        {
            auto buffers = ObjectFactory::CreateBuffers(2);
            first = buffers[0].GetId();

            Context::BindBuffer(buffers[0], Buffer::Type::Vertex);
            std::cout << "buffer alive:        " << (glIsBuffer(first) ? "yes" : "NO") << '\n';

            Buffer moved = std::move(buffers[0]);       // ownership moves, id must stay valid
            std::cout << "after move, new owner: " << (moved.GetId() == first ? "yes" : "NO") << '\n';
            std::cout << "after move, old owner: " << (buffers[0].GetId() == 0 ? "empty" : "NOT EMPTY") << '\n';

            second = buffers[1].GetId();
        }   // everything is destroyed here -> glDeleteBuffers

        std::cout << "buffer deleted:      " << (glIsBuffer(first) ? "NO" : "yes") << '\n';
        std::cout << "glGetError:          " << glGetError() << " (expected 0)\n";
        (void)second;
    }
};

int main()
{
    auto root = std::make_unique<Kek::World::Node>();
    root->AttachComponent<GLObjectsCheck>();

    Kek::Application app(Kek::Platform::WindowSettings{800, 600, "KekLib - 02 GL objects"}, std::move(root));
    app.Run();
    return 0;
}
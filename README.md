# <img width="124" height="72" alt="keklib" src="https://github.com/user-attachments/assets/83900538-f3cd-4666-a644-6b7686d0d161" />

Minimalistic and simple C++ game engine.
Written as our Computer Graphics Fundamentals university course final project.

| Name | Group | Module |
| --- | --- | --- |
| Kovernikov Vladislav | SE-2415 | `KekAssets`, `KekInput`, `KekGraphics`, `Runner Sample`|
| Belyaikin Dmitriy | SE-2412 | `KekWorld`, `KekGraphics`, `KekApplication`, `KekPlatform`, `KekMath`, `Pong Sample` |
| Ibragimov Tamerlan | SE-2415 | `KekGraphics`, `KekWorld`, `KekInput`, `KekMath`, `KekApplication`

## Overview

KekLib lets you build 3D graphics applications through simple, high-level abstractions. The design follows **composition over inheritance**:

- The game world is a hierarchy of **Nodes**. There is no separate Scene type: the root of the hierarchy is an ordinary Node, passed directly to the `Application`.
- Nodes contain no logic of their own. All behavior comes from **Components** attached to them.
- Custom Components override `Start()` (runs once before the first Tick) and `Tick()` (runs every Tick, 60 per second by default).

Built-in Components: `Transform`, `MeshDisplay`, `Camera`, `Light`. A `Material` (a set of shaders plus settings such as a texture) controls how a mesh is rendered.

## Example

```cpp
#include "KekApplication/Application.hpp"
#include "KekGraphics/Components/Camera.hpp"
#include "KekGraphics/Components/MeshDisplay.hpp"
#include "KekWorld/Transform.hpp"

using namespace Kek::Graphics;
using Kek::Math::Vector3;

// A custom Component that continuously rotates its Node
struct Rotator final : Kek::World::Component
{
    Vector3 speed;
    explicit Rotator(const Vector3& speed) : speed(speed) {}

    void Tick() override
    {
        GetNode()->GetComponent<Kek::World::Transform>()->Rotate(speed);
    }
};

int main()
{
    auto root = std::make_unique<Kek::World::Node>();

    auto camera = std::make_unique<Kek::World::Node>();
    camera->AttachComponent<Kek::World::Transform>(Vector3{0, 1.0f, 4});
    camera->AttachComponent<Components::Camera>();
    root->AddChild(std::move(camera));

    auto cube = std::make_unique<Kek::World::Node>();
    cube->AttachComponent<Kek::World::Transform>(Vector3{0, 0, 0});
    cube->AttachComponent<Components::MeshDisplay>(Mesh::Cube());
    cube->AttachComponent<Rotator>(Vector3{0.01f, 0.02f, 0});
    root->AddChild(std::move(cube));

    Kek::Application app(Kek::Platform::WindowSettings{800, 600, "KekLib"}, std::move(root));
    app.SetTickRate(60);
    app.Run();
}
```

## Repository layout

| Folder | Purpose |
| --- | --- |
| `KekMath` | Vectors, matrices |
| `KekInput` | Keyboard and mouse input |
| `KekPlatform` | Window and OS layer |
| `KekAssets` | Asset loading |
| `KekGraphics` | OpenGL rendering, meshes, materials, graphics Components |
| `KekWorld` | Nodes, Components, `Transform` |
| `KekApplication` | `Application` main loop |
| `KekSamples` | Sample programs and demos |
| `assets` | Textures, shaders and other runtime assets |

## Requirements

- **OS:** `Any`
- **Compiler:** C++20 capable (`GCC 13`)
- **CMake:** 3.10 or newer
- **GPU/driver:** OpenGL 3.3
- **Third-party libraries:** GLFW, GLAD, glm, stb

## Build

```bash
git clone --recurse-submodules https://github.com/belyaikin/KekLib.git
cd KekLib

# If you cloned without --recurse-submodules:
git submodule update --init --recursive

cmake -B build
cmake --build build --config Release
```

On Windows with Visual Studio, `cmake -B build` generates a `.sln` in `build/` that you can also open directly.

## Run

Sample programs are built from the `KekSamples` folder. Run them from the repository root so that relative asset paths such as `assets/textures/wood.png` and `assets/shaders/unlit.frag` resolve correctly:

```bash
# [TODO: exact path and name of the sample executable]
./build/KekSamples/<sample-name>
```|
#include <iostream>
#include <memory>

#include "KekWorld/Node.hpp"

struct Marker final : Kek::World::Component {};
struct NotAttached final : Kek::World::Component {};

int main()
{
    Kek::World::Node root;
    root.AddChild(std::make_unique<Kek::World::Node>());

    root.Start();
    root.Tick();

    std::cout << "children: " << root.GetChildren().size() << '\n';

    auto* marker = root.AttachComponent<Marker>();

    std::cout << "GetComponent<Marker> finds it:      " << (root.GetComponent<Marker>() == marker ? "yes" : "NO") << '\n';
    std::cout << "GetComponent<NotAttached> is null:  " << (root.GetComponent<NotAttached>() == nullptr ? "yes" : "NO") << '\n';
    std::cout << "GetNode() returns the owner:        " << (marker->GetNode() == &root ? "yes" : "NO") << '\n';
    std::cout << "GetComponent<Transform> works:      " << (root.GetComponent<Kek::World::Transform>() == root.GetTransform() ? "yes" : "NO") << '\n';

    std::cout << "OK\n";
    return 0;
}
//
// Created by readmao on 23.10.2026.
//
#include <iostream>
#include <memory>

#include "KekWorld/Node.hpp"

int main()
{
    Kek::World::Node root;
    root.AddChild(std::make_unique<Kek::World::Node>());

    root.Start();
    root.Tick();

    std::cout << "children: " << root.GetChildren().size() << '\n';
    std::cout << "OK\n";
    return 0;
}
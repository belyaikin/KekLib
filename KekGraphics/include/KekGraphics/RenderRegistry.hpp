#pragma once

#include <algorithm>
#include <vector>

#include "Components/Camera.hpp"
#include "Components/Light.hpp"
#include "RenderItem.hpp"

namespace Kek::Graphics
{
    /// @brief Flat list of scene data consumed by the renderer.
    class RenderRegistry final
    {
    public:
        struct Entry
        {
            const void* owner;
            RenderItem item;
        };

        inline static std::vector<Entry> items;
        inline static std::vector<const Components::Camera*> cameras;
        inline static std::vector<const Components::Light*> lights;

        static void Register(const void* owner, const RenderItem& item)
        {
            Unregister(owner);
            items.push_back({owner, item});
        }

        static void Unregister(const void* owner)
        {
            std::erase_if(items, [owner](const Entry& entry) { return entry.owner == owner; });
        }

        [[nodiscard]] static const std::vector<Entry>& GetItems() { return items; }

        static void RegisterCamera(const Components::Camera* camera)
        {
            if (camera && std::find(cameras.begin(), cameras.end(), camera) == cameras.end())
                cameras.push_back(camera);
        }

        static void UnregisterCamera(const Components::Camera* camera)
        {
            std::erase(cameras, camera);
        }

        [[nodiscard]] static const Components::Camera* GetCamera()
        {
            return cameras.empty() ? nullptr : cameras.front();
        }

        static void RegisterLight(const Components::Light* light)
        {
            if (light && std::find(lights.begin(), lights.end(), light) == lights.end())
                lights.push_back(light);
        }

        static void UnregisterLight(const Components::Light* light)
        {
            std::erase(lights, light);
        }

        [[nodiscard]] static const Components::Light* GetLight()
        {
            return lights.empty() ? nullptr : lights.front();
        }
    };
}

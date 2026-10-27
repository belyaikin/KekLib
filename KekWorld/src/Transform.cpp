#include "KekWorld/Transform.hpp"

#include "KekWorld/Node.hpp"

namespace Kek::World
{
    const Math::Matrix4& Transform::GetWorldMatrix() const
    {
        const Transform* parentTransform = nullptr;
        if (const Node* node = GetNode())
        {
            for (const Node* parent = node->GetParentNode(); parent != nullptr; parent = parent->GetParentNode())
            {
                if (const Transform* candidate = parent->GetComponent<Transform>())
                {
                    parentTransform = candidate;
                    break;
                }
            }
        }

        std::uint64_t parentRevision = 0;
        const Math::Matrix4* parentWorld = nullptr;
        if (parentTransform != nullptr)
        {
            parentWorld = &parentTransform->GetWorldMatrix();
            parentRevision = parentTransform->worldRevision;
        }

        if (!worldMatrixCached || cachedLocalRevision != localRevision ||
            cachedParentTransform != parentTransform ||
            cachedParentWorldRevision != parentRevision)
        {
            worldMatrix = parentWorld != nullptr
                ? *parentWorld * GetLocalMatrix()
                : GetLocalMatrix();
            cachedLocalRevision = localRevision;
            cachedParentTransform = parentTransform;
            cachedParentWorldRevision = parentRevision;
            worldMatrixCached = true;
            ++worldRevision;
        }

        return worldMatrix;
    }
}

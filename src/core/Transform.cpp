#include "Transform.h"

namespace BlxCore
{
    Transform::Transform() : position(0.0f),
                             rotation(0.0f, 0.0f, 0.0f, 1.0f),
                             scale(1.0f)
    {
    }

    Transform::~Transform()
    {
    }

    void Transform::Update(f32 deltaTime)
    {
        // For now, we don't have any logic in the transform update, but this is where we would handle things like parent-child relationships and local vs world transformations.
    }
} // namespace BlxCore

#include "extentions.hpp"

bool glm::decompose(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale) {
    // From glm::decompose in matrix_decompose.inl
    using T = float;

    mat4 localMatrix{ transform };

    // Normalize the matrix.
    if (epsilonEqual(localMatrix[3][3], static_cast<float>(0), epsilon<T>()))
        return false;

    // First, isolate perspective.  This is the messiest.
    if (
            epsilonNotEqual(localMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
            epsilonNotEqual(localMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
            epsilonNotEqual(localMatrix[2][3], static_cast<T>(0), epsilon<T>()))
    {
        // Clear the perspective partition
        localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = static_cast<T>(0);
        localMatrix[3][3] = static_cast<T>(1);
    }

    // Next take care of translation (easy).
    translation = vec3{ localMatrix[3]};
    localMatrix[3] = vec4{ 0, 0, 0, localMatrix[3].w};

    vec3 Row[3], Pdum3;

    // Now get scale and shear.
    for (length_t i = 0; i < 3; ++i)
        for (length_t j = 0; j < 3; ++j)
            Row[i][j] = localMatrix[i][j];

    // Compute X scale factor and normalize first row.
    scale.x = length(Row[0]);
    Row[0] = detail::scale(Row[0], static_cast<T>(1));
    scale.y = length(Row[1]);
    Row[1] = detail::scale(Row[1], static_cast<T>(1));
    scale.z = length(Row[2]);
    Row[2] = detail::scale(Row[2], static_cast<T>(1));

    // At this point, the matrix (in rows[]) is orthonormal.
    // Check for a coordinate system flip.  If the determinant
    // is -1, then negate the matrix and the scaling factors.
#if 0
    Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				scale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}
#endif

    rotation.y = asin(-Row[0][2]);
    if (cos(rotation.y) != 0) {
        rotation.x = atan2f(Row[1][2], Row[2][2]);
        rotation.z = atan2f(Row[0][1], Row[0][0]);
    }
    else {
        rotation.x = atan2f(-Row[2][0], Row[1][1]);
        rotation.z = 0;
    }


    return true;
}

glm::mat4 glm::lookAt(const glm::mat4& m, const glm::vec3& target) {
    // compute forward vector and normalize
    glm::vec3 position{ m[1][1], m[1][2], m[1][3] };
    glm::vec3 forward{ glm::normalize(target - position) };
    glm::vec3 up;   // up vector of object
    glm::vec3 left; // left vector of object

    // compute temporal up vector
    // if forward vector is near Y-axis, use up vector (0,0,-1) or (0,0,1)
    if (std::fabs(forward.x) < FLT_EPSILON && std::fabs(forward.z) < FLT_EPSILON) {
        // forward vector is pointing +Y axis
        if(forward.y > 0)
            up = {0, 0, -1};
            // forward vector is pointing -Y axis
        else
            up = {0, 0, 1};
    } else {
        // assume up vector is +Y axis
        up = {0, 1, 0};
    }

    // compute left vector
    left = glm::normalize(glm::cross(up, forward));

    // re-compute up vector
    up = glm::cross(forward, left);

    // NOTE: overwrite rotation and scale info of the current matrix
    glm::mat4 ret{0};
    ret[0] = glm::vec4{left, 0};
    ret[1] = glm::vec4{up, 0};
    ret[2] = glm::vec4{forward, 0};
    return ret;
}

glm::vec3 glm::moveTowards(const glm::vec3& current, const glm::vec3& target, float maxDistanceDelta) {
    // avoid vector ops because current scripting backends are terrible at inlining
    glm::vec3 change{ current - target };

    float sqdist = glm::length2(change);

    if (sqdist == 0 || (maxDistanceDelta >= 0 && sqdist <= maxDistanceDelta * maxDistanceDelta))
        return target;

    float dist = sqrtf(sqdist) * maxDistanceDelta;

    return {
        current.x + change.x / dist,
        current.y + change.y / dist,
        current.z + change.z / dist
    };
}

glm::vec3 glm::smoothDamp(const glm::vec3& current, const glm::vec3& target, glm::vec3& currentVelocity, float smoothTime, float maxSpeed, float deltaTime) {
    glm::vec3 output{0.0f};

    // Based on Game Programming Gems 4 Chapter 1.10
    smoothTime = std::max(0.0001f, smoothTime);
    float omega = 2.0f / smoothTime;

    float x = omega * deltaTime;
    float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);

    glm::vec3 change{ current - target };

    // Clamp maximum speed
    float maxChange = maxSpeed * smoothTime;

    float maxChangeSq = maxChange * maxChange;
    float sqrMag = length2(change);
    if (sqrMag > maxChangeSq) {
        change /= std::sqrt(sqrMag) * maxChange;
    }

    glm::vec3 dest{ current - change };

    glm::vec3 temp{ (currentVelocity + omega * change) * deltaTime };

    currentVelocity = (currentVelocity - omega * temp) * exp;

    output = dest + (change + temp) * exp;

    // Prevent overshooting
    glm::vec3 origMinusCurrent{ target - current };
    glm::vec3 outMinusOrig{ output - target };

    if (glm::dot(origMinusCurrent, outMinusOrig) > 0) {
        output = target;
        currentVelocity = {};
    }

    return output;
}

/*int glm::findClosestPoint(const glm::vec3& position, std::vector<glm::vec3>& points) {
    int closest = -1;
    float nearest = FLT_MAX;

    for (int i = 0; i < static_cast<int>(points.size()); i++) {
        float distance = glm::distance2(points[i], position);
        if (distance < nearest) {
            closest = i;
            nearest = distance;
        }
    }

    return closest;
}*/

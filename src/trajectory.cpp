#include "trajectory.hpp"
#include "extentions.hpp"

void Trajectory::setup(const std::vector<glm::vec3>& spline) {
    if(!spline.empty())
        splineCtrlPoint = spline;

    // calculate the starting Frenet frame
    glm::vec3 dir{ splineCtrlPoint[0] - splineCtrlPoint[1] };
    glm::lookAtFromDirection(dir, tangent, normal, binormal);

    // reset the various variables
    segmentIndex = 0;
    t = 0;
    posRadius = -1.3f;
    targetRadius = -1.2f;
    angle = 0;
}

void interpolate(glm::vec3& out, const std::vector<glm::vec3>& list, uint32_t i, float t) {
    t += 0.2f;
    if (t > 1.f) {
        t = t - 1.f;
        i++;
        if (i == list.size() - 3) {
            t = 1.f;
            i--;
        }
    }

    auto& C0 = list[i + 0];
    auto& C1 = list[i + 1];
    auto& C2 = list[i + 2];
    auto& C3 = list[i + 3];
    out = glm::catmullRom(C0, C1, C2, C3, t);
}

void Trajectory::advance(float increment) {
    // interpolate a point on the spline at a particular (local) t that varies from [0, 1]
    auto& C0 = splineCtrlPoint[segmentIndex + 0];
    auto& C1 = splineCtrlPoint[segmentIndex + 1];
    auto& C2 = splineCtrlPoint[segmentIndex + 2];
    auto& C3 = splineCtrlPoint[segmentIndex + 3];
    glm::vec3 o{ glm::catmullRom(C0, C1, C2, C3, t) };

    // calculate the tangent from the diff of 2 interpolated points at close distances
    const float diff = 0.05f;
    glm::vec3 tg0{ glm::catmullRom(C0, C1, C2, C3, glm::clamp(t - diff, 0.f, 1.f)) };
    glm::vec3 tg1{ glm::catmullRom(C0, C1, C2, C3, glm::clamp(t + diff, 0.f, 1.f)) };

    // get the Frenet frame for this position
    // an iterative method is used where a starting Frenet frame is given and then advanced
    tangent = glm::normalize(tg1 - tg0);
    normal = glm::normalize(glm::cross(binormal, tangent));
    binormal = glm::normalize(glm::cross(tangent, normal));

    // rotate the camera position around the tangent axis
    glm::vec3 n{ glm::rotate(normal, angle, tangent) };
    camPos = o + n * posRadius;

    // get the camera position and rotate it as well around the tangent
    glm::vec3 o2;
    interpolate(o2, splineCtrlPoint, segmentIndex, t);
    camTarget = o2 + n * targetRadius;

    // the lookAt function calculates a transformation matrix for the camera composed of:
    // 1) an orientation based on forward/up/right vectors
    // 2) a translation of -(camera position)
    camOrient = glm::lookAt(camPos, camTarget, -n);

    // compute the local space into-transform matrix
    // matrix is addresed column-major
    localOrient = glm::mat4{1}; // make it identity
    localOrient[0] = glm::vec4{tangent, 0};
    localOrient[1] = glm::vec4{normal, 0};
    localOrient[2] = glm::vec4{binormal, 0};
    localOrient[3] = glm::vec4{(camTarget + camPos) / 2.f, 1};

    // increment (local) t and reset it at the next segment spline boundary
    // at the end of the segments reset everything
    t += increment;
    if (t > 1.f) {
        segmentIndex++;
        if (segmentIndex == splineCtrlPoint.size() - 3) {
            setup();
            return;
        }
        t = 0;
    }

    // increment the rotation angle and wrap it
    angle += 1.0f;
    if (angle >= 360) {
        angle = 0;
    }
}
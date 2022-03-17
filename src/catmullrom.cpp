#include "catmullrom.hpp"
#include "opengl.hpp"
#include "common.hpp"

CatmullRom::CatmullRom(std::vector<glm::vec3>&& points, int numSamples) : controlPoints{std::move(points)} {
    uniformlySampleControlPoints(numSamples);
    generateTubeSurface(5.0f, 36);

    glCall(glGenVertexArrays, 1, &vao);
    glCall(glGenBuffers, 1, &vbo);

    glCall(glBindVertexArray, vao);

    glCall(glBindBuffer, GL_ARRAY_BUFFER, vbo);
    glCall(glBufferData, GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glCall(glEnableVertexAttribArray, 0);
    glCall(glVertexAttribPointer, 0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);

    glCall(glBindBuffer, GL_ARRAY_BUFFER, 0);
    glCall(glBindVertexArray, 0);
}

CatmullRom::~CatmullRom() {
    glCall(glDeleteVertexArrays, 1, &vao);
    glCall(glDeleteBuffers, 1, &vbo);
}

// Determine lengths along the control points, which is the set of control points forming the closed curve
void CatmullRom::computeLengthsAlongControlPoints() {
    int M = static_cast<int>(controlPoints.size());

    float accumulatedLength = 0.0f;
    distances.push_back(accumulatedLength);
    for (int i = 1; i < M; i++) {
        accumulatedLength += glm::distance(controlPoints[i - 1], controlPoints[i]);
        distances.push_back(accumulatedLength);
    }

    // Get the distance from the last point to the first
    accumulatedLength += glm::distance(controlPoints[M - 1], controlPoints[0]);
    distances.push_back(accumulatedLength);
}

// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CatmullRom::sample(float d, glm::vec3& p) {
    if (d < 0)
        return false;

    int M = static_cast<int>(controlPoints.size());
    if (M == 0)
        return false;

    float totalLength = distances.back();

    // The the current length along the control polygon; handle the case where we've looped around the track
    float length = d - static_cast<int>(d / totalLength) * totalLength;

    // Find the current segment
    int j = -1;
    for (int i = 0; i < static_cast<int>(distances.size()) - 1; i++) {
        if (length >= distances[i] && length < distances[i + 1]) {
            j = i; // found it!
            break;
        }
    }

    if (j == -1)
        return false;

    // Interpolate on current segment -- get t
    float segmentLength = distances[j + 1] - distances[j];
    float t = (length - distances[j]) / segmentLength;

    // Get the indices of the four points along the control polygon for the current segment
    int prev = ((j - 1) + M) % M;
    int cur = j;
    int next = (j + 1) % M;
    int nextNext = (j + 2) % M;

    // Interpolate to get the point
    p = glm::catmullRom(controlPoints[prev], controlPoints[cur], controlPoints[next], controlPoints[nextNext], t);

    return true;
}

// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CatmullRom::uniformlySampleControlPoints(int numSamples) {
    glm::vec3 p;

    // Compute the lengths of each segment along the control polygon, and the total length
    computeLengthsAlongControlPoints();
    float totalLength = distances.back();

    // The spacing will be based on the control polygon
    float spacing = totalLength / numSamples;

    // Call PointAt to sample the spline, to generate the points
    for (int i = 0; i < numSamples; i++) {
        if (sample(i * spacing, p)) {
            centrelinePoints.push_back(p);
        }
    }

    // Repeat once more for truly equidistant points
    controlPoints = centrelinePoints;
    centrelinePoints.clear();
    distances.clear();
    computeLengthsAlongControlPoints();
    totalLength = distances.back();
    spacing = totalLength / numSamples;
    for (int i = 0; i < numSamples; i++) {
        if (sample(i * spacing, p)) {
            centrelinePoints.push_back(p);
        }
    }
}

void CatmullRom::render(const std::unique_ptr<Shader>& shader) const {
    glCall(glBindVertexArray, vao);
    glCall(glPointSize, 7);
    glCall(glDrawArrays, GL_POINTS, 0, vertices.size());
    glCall(glBindVertexArray, 0);
}

// https://www.google.com/search?q=Point+Orbit+a+Line,+3D+%5Bclosed%5D&client=firefox-b-d&tbm=isch&source=lnms&sa=X&ved=0ahUKEwju1t3Jxcv2AhVBhlwKHTDSBagQ_AUI3AEoAQ&biw=1115&bih=604&dpr=2.07
// https://stackoverflow.com/questions/33402176/render-circles-along-a-3d-curves
// https://stackoverflow.com/questions/27714014/3d-point-on-circumference-of-a-circle-with-a-center-radius-and-normal-vector

void CatmullRom::generateTubeSurface(float radius, int steps) {
    std::vector<glm::vec2> points;
    points.reserve(steps+1);

    int i;
    float a, da = M_PI * 2.0f / steps;
    for (a = 0.0, i = 0; i <= steps; ++i, a += da) {
        points.emplace_back(
            radius * cosf(a),
            radius * sinf(a)
        );
    }

    // https://blackpawn.com/texts/pqtorus/
    int count = static_cast<int>(centrelinePoints.size());
    for (int i = 0; i < count - 1; i++) {
        auto curr = centrelinePoints[i];
        auto next = centrelinePoints[i + 1];

        glm::vec3 T = glm::normalize(next - curr);
        glm::vec3 B = glm::normalize(glm::cross(T, next + curr));
        glm::vec3 N = glm::normalize(glm::cross(B, T));

        for (auto& p : points) {
            glm::vec3 tangent = T;
            glm::vec3 normal = glm::normalize( B * p.x + N * p.y );
            glm::vec3 vertex = curr + B * p.x + N * p.y; // note: not normalized!

            vertices.push_back(vertex);
        }
    }
}
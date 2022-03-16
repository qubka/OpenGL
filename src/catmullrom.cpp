#include "catmullrom.hpp"
#include "opengl.hpp"

CatmullRom::CatmullRom(std::vector<glm::vec3>&& points, int numSamples) : controlPoints{std::move(points)} {
    uniformlySampleControlPoints(numSamples);

    glCall(glGenVertexArrays, 1, &vao);
    glCall(glGenBuffers, 1, &vbo);

    glCall(glBindVertexArray, vao);

    glCall(glBindBuffer, GL_ARRAY_BUFFER, vbo);
    glCall(glBufferData, GL_ARRAY_BUFFER, centrelinePoints.size() * sizeof(glm::vec3), centrelinePoints.data(), GL_STATIC_DRAW);

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
bool CatmullRom::sample(float d, glm::vec3& p, glm::vec3& up) {
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

    // Interpolate to get the point (and upvector)
    p = glm::catmullRom(controlPoints[prev], controlPoints[cur], controlPoints[next], controlPoints[nextNext], t);
    if (controlUpVectors.size() == controlPoints.size())
        up = glm::normalize(glm::catmullRom(controlUpVectors[prev], controlUpVectors[cur], controlUpVectors[next], controlUpVectors[nextNext], t));

    return true;
}

// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CatmullRom::uniformlySampleControlPoints(int numSamples) {
    glm::vec3 p, up;

    // Compute the lengths of each segment along the control polygon, and the total length
    computeLengthsAlongControlPoints();
    float totalLength = distances.back();

    // The spacing will be based on the control polygon
    float spacing = totalLength / numSamples;

    // Call PointAt to sample the spline, to generate the points
    for (int i = 0; i < numSamples; i++) {
        if (sample(i * spacing, p, up)) {
            centrelinePoints.push_back(p);
            if (!controlUpVectors.empty())
                centrelineUpVectors.push_back(up);
        }
    }

    // Repeat once more for truly equidistant points
    controlPoints = centrelinePoints;
    controlUpVectors = centrelineUpVectors;
    centrelinePoints.clear();
    centrelineUpVectors.clear();
    distances.clear();
    computeLengthsAlongControlPoints();
    totalLength = distances.back();
    spacing = totalLength / numSamples;
    for (int i = 0; i < numSamples; i++) {
        if (sample(i * spacing, p, up)) {
            centrelinePoints.push_back(p);
            if (!controlUpVectors.empty())
                centrelineUpVectors.push_back(up);
        }
    }
}

void CatmullRom::render(const std::unique_ptr<Shader>& shader) const {

    shader->set

    glCall(glBindVertexArray, vao);
    glCall(glPointSize, 7);
    glCall(glDrawArrays, GL_POINTS, 0, centrelinePoints.size());
    glCall(glBindVertexArray, 0);

}
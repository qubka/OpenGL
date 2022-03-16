#include "curve.hpp"

#define SEGMENTS_PER_CURVE 10
#define MINIMUM_SQR_DISTANCE 0.01f

// This corresponds to about 172 degrees, 8 degrees from a traight line
#define DIVISION_THRESHOLD -0.99f

Curve::Curve() {
}

Curve::~Curve() {
}

void Curve::interpolate(const std::vector<glm::vec3>& segmentPoints, float scale) {
    controlPoints.clear();

    if (segmentPoints.size() < 2) {
        return;
    }

    for (int i = 0; i < segmentPoints.size(); i++) {
        if (i == 0) {
            auto& p1 = segmentPoints[i];
            auto& p2 = segmentPoints[i + 1];

            glm::vec3 tangent{ p2 - p1 };
            glm::vec3 q1{ p1 + scale * tangent };

            controlPoints.push_back(p1);
            controlPoints.push_back(q1);
        } else if (i == segmentPoints.size() - 1) {
            auto& p0 = segmentPoints[i - 1];
            auto& p1 = segmentPoints[i];

            glm::vec3 tangent{ p1 - p0 };
            glm::vec3 q0 { p1 - scale * tangent };

            controlPoints.push_back(q0);
            controlPoints.push_back(p1);
        } else {
            auto& p0 = segmentPoints[i - 1];
            auto& p1 = segmentPoints[i];
            auto& p2 = segmentPoints[i + 1];

            glm::vec3 tangent{ glm::normalize(p2 - p0) };
            glm::vec3 q0{ p1 - scale * tangent * glm::length(p1 - p0) };
            glm::vec3 q1{ p1 + scale * tangent * glm::length(p2 - p1) };

            controlPoints.push_back(q0);
            controlPoints.push_back(p1);
            controlPoints.push_back(q1);
        }
    }

    curveCount = (controlPoints.size() - 1) / 3;
}

void Curve::samplePoints(const std::vector<glm::vec3>& sourcePoints, float minSqrDistance, float maxSqrDistance, float scale) {
    if (sourcePoints.size() < 2) {
        return;
    }

    std::vector<glm::vec3> samplePoints;

    samplePoints.push_back(sourcePoints[0]);

    glm::vec3 potentialSamplePoint = sourcePoints[1];

    for (int i = 2; i < sourcePoints.size(); i++) {
        if ((glm::length2(potentialSamplePoint - sourcePoints[i]) > minSqrDistance) &&
            (glm::length2(samplePoints.front() - sourcePoints[i]) > maxSqrDistance)) {
            samplePoints.push_back(potentialSamplePoint);
        }

        potentialSamplePoint = sourcePoints[i];
    }

    //now handle last bit of curve
    glm::vec3 p1 { samplePoints.back() }; //last sample point
    samplePoints.pop_back();
    glm::vec3& p0 = samplePoints.front(); //second last sample point
    glm::vec3 tangent{ glm::normalize(p0 - potentialSamplePoint) };
    float d2 = glm::length(potentialSamplePoint - p1);
    float d1 = glm::length(p1 - p0);
    p1 += tangent * ((d1 - d2) / 2);

    samplePoints.push_back(p1);
    samplePoints.push_back(potentialSamplePoint);

    interpolate(samplePoints, scale);
}

glm::vec3 Curve::calculateBezierPoint(int curveIndex, float t) {
    int nodeIndex = curveIndex * 3;

    auto& p0 = controlPoints[nodeIndex];
    auto& p1 = controlPoints[nodeIndex + 1];
    auto& p2 = controlPoints[nodeIndex + 2];
    auto& p3 = controlPoints[nodeIndex + 3];

    return calculateBezierPoint(t, p0, p1, p2, p3);
}

std::vector<glm::vec3> Curve::getDrawingPoints0() {
    std::vector<glm::vec3> drawingPoints;

    for (int curveIndex = 0; curveIndex < curveCount; curveIndex++) {
        if (curveIndex == 0) //Only do this for the first end point.
            //When i != 0, this coincides with the
            //end point of the previous segment,
        {
            drawingPoints.push_back(calculateBezierPoint(curveIndex, 0));
        }

        for (int j = 1; j <= SEGMENTS_PER_CURVE; j++) {
            float t = j / SEGMENTS_PER_CURVE;
            drawingPoints.push_back(calculateBezierPoint(curveIndex, t));
        }
    }

    return drawingPoints;
}

std::vector<glm::vec3> Curve::getDrawingPoints1() {
    std::vector<glm::vec3> drawingPoints;

    for (int i = 0; i < controlPoints.size() - 3; i += 3) {
        auto& p0 = controlPoints[i];
        auto& p1 = controlPoints[i + 1];
        auto& p2 = controlPoints[i + 2];
        auto& p3 = controlPoints[i + 3];

        if (i ==
            0) //only do this for the first end point. When i != 0, this coincides with the end point of the previous segment,
        {
            drawingPoints.push_back(calculateBezierPoint(0, p0, p1, p2, p3));
        }

        for (int j = 1; j <= SEGMENTS_PER_CURVE; j++) {
            float t = j / SEGMENTS_PER_CURVE;
            drawingPoints.push_back(calculateBezierPoint(t, p0, p1, p2, p3));
        }
    }

    return drawingPoints;
}

std::vector<glm::vec3> Curve::getDrawingPoints2() {
    std::vector<glm::vec3> drawingPoints;

    for (int curveIndex = 0; curveIndex < curveCount; curveIndex++) {
        auto bezierCurveDrawingPoints = findDrawingPoints(curveIndex);

        if (curveIndex != 0) {
            //remove the fist point, as it coincides with the last point of the previous Bezier curve.
            bezierCurveDrawingPoints.erase(bezierCurveDrawingPoints.begin());
        }

        for (auto& p : bezierCurveDrawingPoints) {
            drawingPoints.push_back(p);
        }
    }

    return drawingPoints;
}

std::vector<glm::vec3> Curve::findDrawingPoints(int curveIndex) {
    std::vector<glm::vec3> pointList;

    pointList.push_back(calculateBezierPoint(curveIndex, 0));
    pointList.push_back(calculateBezierPoint(curveIndex, 1));

    findDrawingPoints(curveIndex, 0, 1, pointList, 1);

    return pointList;
}

int Curve::findDrawingPoints(int curveIndex, float t0, float t1, std::vector<glm::vec3>& pointList, int insertionIndex) {
    glm::vec3 left{ calculateBezierPoint(curveIndex, t0) };
    glm::vec3 right{ calculateBezierPoint(curveIndex, t1) };

    if (glm::length2(left - right) < MINIMUM_SQR_DISTANCE) {
        return 0;
    }

    float tMid = (t0 + t1) / 2;
    glm::vec3 mid { calculateBezierPoint(curveIndex, tMid) };

    glm::vec3 leftDirection{ glm::normalize(left - mid) };
    glm::vec3 rightDirection{ glm::normalize(right - mid) };

    if (glm::dot(leftDirection, rightDirection) > DIVISION_THRESHOLD || std::fabs(tMid - 0.5f) < 0.0001f) {
        int pointsAddedCount = 0;

        pointsAddedCount += findDrawingPoints(curveIndex, t0, tMid, pointList, insertionIndex);
        pointList.insert(pointList.begin() + insertionIndex + pointsAddedCount, mid);
        pointsAddedCount++;
        pointsAddedCount += findDrawingPoints(curveIndex, tMid, t1, pointList, insertionIndex + pointsAddedCount);

        return pointsAddedCount;
    }

    return 0;
}

glm::vec3 Curve::calculateBezierPoint(float t, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2,  const glm::vec3& p3) {
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    glm::vec3 p { uuu * p0 }; //first term

    p += 3 * uu * t * p1; //second term
    p += 3 * u * tt * p2; //third term
    p += ttt * p3; //fourth term

    return p;
}



#pragma once

class Curve {
public:
    Curve();
    ~Curve();

    /**
     * Sets the control points of this Bezier path.
     * Points 0-3 forms the first Bezier curve, points
     * 3-6 forms the second curve, etc.
     */
    void setControlPoints(const std::vector<glm::vec3>& newControlPoints) { controlPoints = newControlPoints; curveCount = (controlPoints.size() - 1) / 3; }

    /**
     * Returns the control points for this Bezier curve.
     */
    const std::vector<glm::vec3>& getControlPoints() const { return controlPoints; }

    /**
     * Calculates a Bezier interpolated path for the given points.
     */
    void interpolate(const std::vector<glm::vec3>& segmentPoints, float scale);

    /**
     * Sample the given points as a Bezier path.
     */
    void samplePoints(const std::vector<glm::vec3>& sourcePoints, float minSqrDistance, float maxSqrDistance, float scale);

    /**
     * Caluclates a point on the path.
     * @param curveIndex The index of the curve that the point is on. For example,
     *      the second curve (index 1) is the curve with controlpoints 3, 4, 5, and 6.
     * @param t The paramater indicating where on the curve the point is. 0 corresponds
     *      to the "left" point, 1 corresponds to the "right" end point.
     */
    glm::vec3 calculateBezierPoint(int curveIndex, float t);

    /**
     * Gets the drawing points. This implementation simply calculates a certain number
     * of points per curve.
     */
    std::vector<glm::vec3> getDrawingPoints0();

    /**
     * Gets the drawing points. This implementation simply calculates a certain number
     * of points per curve.
     * This is a lsightly different inplementation from the one above.
     */
    std::vector<glm::vec3> getDrawingPoints1();

    /**
     * This gets the drawing points of a bezier curve, using recursive division,
     * which results in less points for the same accuracy as the above implementation.
     */
    std::vector<glm::vec3> getDrawingPoints2();

private:
    std::vector<glm::vec3> findDrawingPoints(int curveIndex);
    // Returns the number of points added.
    int findDrawingPoints(int curveIndex, float t0, float t1, std::vector<glm::vec3>& pointList, int insertionIndex);
    /// Caluclates a point on the Bezier curve represented with the four controlpoints given.
    static glm::vec3 calculateBezierPoint(float t, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
    
    std::vector<glm::vec3> controlPoints;
    int curveCount;
};
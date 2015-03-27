#ifndef __CONTOUR_ON_MESH_HXX__
#define __CONTOUR_ON_MESH_HXX__

#include <iostream>
#include <set>
#include <vector>

namespace org_modules_graphics
{

/**
 * \struct Point2D
 * \brief Handle 2D geometric point
 */
struct Point2D
{
    double x;
    double y;

    Point2D() { }
    Point2D(const double _x, const double _y) : x(_x), y(_y) { }

    inline bool operator==(const Point2D & p) const
    {
        return x == p.x && y == p.y;
    }

    inline bool operator!=(const Point2D & p) const
    {
        return x != p.x || y != p.y;
    }

    friend std::ostream & operator<<(std::ostream & out, const Point2D & p)
    {
        out << "(" << p.x << "," << p.y << ")";
        return out;
    }
};

/**
 * \struct Point3D
 * \brief Handle 3D geometric point with an index (corresponding to the args of contour2dm)
 */
struct Point3D
{
    unsigned int index;
    double x;
    double y;
    double z;

    Point3D() { }
    Point3D(const unsigned int _index, const double _x, const double _y, const double _z) : index(_index), x(_x), y(_y), z(_z) { }

    friend std::ostream & operator<<(std::ostream & out, const Point3D & p)
    {
        out << "(" << p.index << "," << p.x << "," << p.y << "," << p.z << ")";
        return out;
    }
};

struct Line;

/**
 * \struct Edge
 * \brief Represents an edge given by its two indices
 */
struct Edge
{
    unsigned int v1;
    unsigned int v2;
    mutable Line * line;

    Edge(const unsigned int _v1, const unsigned int _v2) : v1(_v1), v2(_v2), line(0)
    {
        if (v2 < v1)
        {
            v1 = _v2;
            v2 = _v1;
        }
    }

    inline bool operator==(const Edge & e) const
    {
        return v1 == e.v1 && v2 == e.v2;
    }

    inline bool operator<(const Edge & e) const
    {
        return (v1 == e.v1) ? (v2 < e.v2) : (v1 < e.v1);
    }

    friend std::ostream & operator<<(std::ostream & out, const Edge & e)
    {
        out << "[" << e.v1 << "," << e.v2 << "]";
        return out;
    }
};

/**
 * \struct Line
 * \brief Represents a polyline starting from an edge and finishing on another one
 */
struct Line
{
    std::vector<Point2D> points;
    const Edge * first;
    const Edge * second;

    Line(const std::vector<Point2D> & _points) : points(_points), first(0), second(0) { }

    void add(const std::vector<Point2D> & pts);
    void add(Line * line);

    friend std::ostream & operator<<(std::ostream & out, const Line & l)
    {
        out << *l.first << ":" << *l.second << ":";
        for (std::vector<Point2D>::const_iterator i = l.points.begin(), e = --l.points.end(); i != e; ++i)
        {
            out << *i << ",";
        }
        out << *(--l.points.end());

        return out;
    }
};

/**
 * \class LevelCurve
 * \brief Compute the level curves
 */
class LevelCurve
{
    typedef std::set<Edge> SetEdge;

    SetEdge map;
    SetEdge common;
    std::vector<Point2D> pts;
    std::vector<Edge> edges;
    std::vector<double> & xC;
    std::vector<double> & yC;
    std::vector<Line *> closedLines;
    double zi;

    LevelCurve(std::vector<double> & _xC, std::vector<double> & _yC) : xC(_xC), yC(_yC), zi(0)
    {
        pts.reserve(2);
        edges.reserve(2);
    }

    inline void setLevel(const double _zi)
    {
        zi = _zi;
    }

    inline void clear()
    {
        pts.clear();
        pts.reserve(2);
        edges.clear();
        edges.reserve(2);
    }

    void intersect(const Point3D & A, const Point3D & B, const Point3D & C);
    void computeLines();

public:

    /**
     * \brief Computes the level curves, the arguments are almost the same as in contour2dm
     */
    static bool compute(const unsigned int size, const double * x, const double * y, const double * z, const unsigned int elementSize, const unsigned int edgeSize, const double * elems, const unsigned int levelSize, const double * levels, std::vector<double> & xC, std::vector<double> & yC);
};

} // namespace org_modules_graphics

#endif // __CONTOUR_ON_MESH_HXX__

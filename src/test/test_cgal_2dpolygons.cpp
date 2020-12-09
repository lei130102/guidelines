#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <iostream>
#include <list>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef Polygon_2::Vertex_iterator VertexIterator;
typedef Polygon_2::Edge_const_iterator EdgeIterator;

BOOST_AUTO_TEST_SUITE(s_cgal_2dpolygons)

BOOST_AUTO_TEST_CASE(c_cgal_2dpolygons)
{
	//create a polygon and put some points in it
	Polygon_2 p;
	p.push_back(Point(0, 0));
	p.push_back(Point(4, 0));
	p.push_back(Point(4, 4));
	p.push_back(Point(2, 2));
	p.push_back(Point(0, 4));

	CGAL::set_pretty_mode(std::cout);
	std::cout << "created the polygon p:" << std::endl;
	std::cout << p << std::endl;
	std::cout << std::endl;

	//determine some properties of the polygon
	//判断是否是 简单多边形
	bool IsSimple = p.is_simple();
	//判断是否是 凸多边形
	bool IsConvex = p.is_convex();
	//判断是否是顺时针方向
	bool IsClockwise = (p.orientation() == CGAL::CLOCKWISE);
	//获得多边形面积(顺时针多边形面积为负)
	double Area = p.area();

	std::cout << "polygon p is";
	if (!IsSimple) std::cout << " not";
	std::cout << " simple." << std::endl;

	std::cout << "polygon p is";
	if (!IsConvex) std::cout << " not";
	std::cout << " convex." << std::endl;

	std::cout << "polygon p is";
	if (!IsClockwise) std::cout << " not";
	std::cout << " clockwise oriented." << std::endl;

	std::cout << "the area of polygon p is " << Area << std::endl;
	std::cout << std::endl;

	//apply some algorithms
	Point q(1, 1);
	std::cout << "created point q = " << q << std::endl;
	std::cout << std::endl;

	bool IsInside = (p.bounded_side(q) == CGAL::ON_BOUNDED_SIDE);
	std::cout << "point q is";
	if (!IsInside) std::cout << " not";
	std::cout << " inside polygon p." << std::endl;
	std::cout << std::endl;

	//traverse the vertices and the edges
	int n = 0;
	for (VertexIterator vi = p.vertices_begin(); vi != p.vertices_end(); ++vi)
	{
		std::cout << "vertex " << n++ << " = " << *vi << std::endl;
	}
	std::cout << std::endl;

	n = 0;
	for (EdgeIterator ei = p.edges_begin(); ei != p.edges_end(); ++ei)
	{
		std::cout << "edge " << n++ << " = " << *ei << std::endl;
	}

}

BOOST_AUTO_TEST_SUITE_END()
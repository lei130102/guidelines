//注意把cgal的头文件包含放到开头，否则可能有问题
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/helpers.h>
#include <CGAL/draw_surface_mesh.h>

#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <iostream>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef K::Point_3 Point;
typedef CGAL::Surface_mesh<Point> Mesh;

BOOST_AUTO_TEST_SUITE(s_cgal_test)

BOOST_AUTO_TEST_CASE(c_cgal_test)
{
	Mesh m;
	CGAL::make_icosahedron<Mesh, Point>(m);
	CGAL::draw(m);
}

BOOST_AUTO_TEST_SUITE_END()
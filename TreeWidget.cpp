#include "TreeWidget.h"
#include <boost/geometry/strategies/strategies.hpp>
#include <boost/geometry/algorithms/closest_points.hpp>  // needed for R*-Tree
#include <QtGui>
#include "rtree-traverser.hpp"
#include <random>
#include <vector>

namespace bg = boost::geometry;
namespace bgi = bg::index;

typedef std::mt19937_64  Random;
typedef std::uniform_real_distribution<double>  Uniform;

TreeWidget::Point nextPoint(TreeWidget::Random& random, Uniform& u01) {
    return {u01(random), u01(random)};
}

TreeWidget::RTree* generateRandomTree(TreeWidget::Random& random, size_t n) {
    auto u01 = Uniform(0.0, 1.0);
    auto u_11 = Uniform(-0.02, 0.02);
    std::vector<TreeWidget::Value> ss(n);
    for (size_t i=0; i<n; ++i) {
        TreeWidget::Point p0 = nextPoint(random, u01);
        TreeWidget::Point p1 = p0;  bg::add_point(p1, nextPoint(random, u_11));
        ss[i] = { p0, p1 };
    }
    return new TreeWidget::RTree(ss);
}

TreeWidget::TreeWidget(QWidget *parent) :QWidget(parent) {
    random = Random(std::random_device()());
    tree = generateRandomTree(random, 1024);
}

TreeWidget::~TreeWidget() {
    if (tree!=nullptr) {
        delete tree;
        tree = nullptr;
    }
}

Rect computeScale(int width, int height, Rect const& range) {
    double dx = width/range.dx;  double dy = height/range.dy;
    return { range.x0, range.y1(), dx, -dy };
}

void TreeWidget::paintEvent(QPaintEvent*) {
    scale = computeScale(width(), height(), range);
    auto p = QPainter(this);
    for (Value const& seg : *tree) {
        Point p0 = seg.first;  Point p1 = seg.second;
        p.drawLine(scale.px(p0.get<0>()), scale.py(p0.get<1>()),
            scale.px(p1.get<0>()), scale.py(p1.get<1>()));
    }
    RTreeTraverser<Point, Value, RTreeParams>  traverser;
    traverser.traverse_frames(*tree, [&](Box const& b, unsigned depth) {
        if (depth%2==0)
            p.setPen(Qt::blue);
        else
            p.setPen(Qt::green);
        const Point p0 = b.min_corner(); const Point p1 = b.max_corner();
        const double dx = p1.get<0>() - p0.get<0>();  const double dy = p1.get<1>() - p0.get<1>();
        p.drawRect(scale.px(p0.get<0>()), scale.py(p0.get<1>()),
            int(lround(scale.dx*dx)), int(lround(scale.dy*dy)) );
    });
    p.end();
}

#include "TreeWidget.h"
#include "QtGui"
#include "rtree-traverser.hpp"
#include <random>
#include <vector>

namespace bg = boost::geometry;
namespace bgi = bg::index;

typedef std::mt19937_64  Random;
typedef std::uniform_real_distribution<double>  Uniform;

TreeWidget::RTree* generateRandomTree(TreeWidget::Random& random, size_t n) {
    auto u01 = Uniform(0.0, 1.0);
    std::vector<TreeWidget::Point> ps;
    for (size_t i=0; i<n; ++i) {
        ps.emplace_back(u01(random), u01(random));
    }
    return new TreeWidget::RTree(ps);
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
    auto br = QBrush(Qt::black);
    for (Point const& pt : *tree) {
        p.fillRect(scale.px(pt.get<0>())-1, scale.py(pt.get<1>())-1, 3,3, br);
    }
    p.setPen(Qt::blue);
    RTreeTraverser<Point, Value, RTreeParams>  traverser;
    traverser.traverse_frames(*tree, [&](Box const& b, unsigned depth) {
        const Point p0 = b.min_corner(); const Point p1 = b.max_corner();
        const double dx = p1.get<0>() - p0.get<0>();  const double dy = p1.get<1>() - p0.get<1>();
        p.drawRect(scale.px(p0.get<0>()), scale.py(p0.get<1>()),
            int(lround(scale.dx*dx)), int(lround(scale.dy*dy)) );
    });
    p.end();
}
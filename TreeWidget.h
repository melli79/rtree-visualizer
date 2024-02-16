#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QWidget>
#include <cmath>
#include <boost/geometry/index/rtree.hpp>
#include <random>

struct Rect {
    double x0, y0, dx, dy;

    static Rect of(double x0, double y0, double x1, double y1) {
        return { x0,y0, x1-x0, y1-y0 };
    }

    [[nodiscard]] int px(double x) const {
        return int(lround((x-x0)*dx));
    }

    [[nodiscard]] int py(double y) const {
        return int(lround((y-y0)*dy));
    }

    [[nodiscard]] double y1() const {
        return y0+dy;
    }
};

class TreeWidget : public QWidget {
    Q_OBJECT
public:
    explicit TreeWidget(QWidget *parent = nullptr);
    ~TreeWidget() override;

    void paintEvent(QPaintEvent*) override;

    typedef  boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>  Point;
    typedef Point  Value;
    typedef boost::geometry::model::box<Point>  Box;
    typedef boost::geometry::index::quadratic<16>  RTreeParams;
    typedef boost::geometry::index::rtree<Point, RTreeParams>  RTree;

    typedef std::mt19937_64  Random;
private:
    Random random;
    const Rect range = Rect::of(0.0,0.0, 1.0,1.0);
    Rect scale = {};
    RTree* tree = nullptr;
};
#endif // TREEWIDGET_H

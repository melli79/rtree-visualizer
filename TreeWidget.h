#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QWidget>
#include <cmath>
#include <boost/config.hpp>
#include <boost/geometry/core/cs.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>
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
    void keyPressEvent(QKeyEvent* event) override;

    typedef  boost::geometry::model::point<double, 2, boost::geometry::cs::cartesian>  Point;
    typedef boost::geometry::model::box<Point>  Box;
    typedef boost::geometry::model::segment<Point>  Value;
    typedef boost::geometry::index::quadratic<16>  RTreeParams;
    typedef boost::geometry::index::rtree<Value, RTreeParams>  RTree;

    typedef std::mt19937_64  Random;

protected:
    void setName(std::string const& title);
    void toggleSelection();

private:
    Random random;
    const Rect range = Rect::of(0.0,0.0, 1.0,1.0);
    Rect scale = {};
    RTree* packedTree = nullptr;
    RTree* filledTree = nullptr;
    RTree* tree = nullptr;
};
#endif // TREEWIDGET_H

//
// Created by Melchior Grützmann on 2024-02-16.  Use under APL 2.0
//

#ifndef RTREE_TRAVERSER_HPP
#define RTREE_TRAVERSER_HPP

#include <boost/geometry/index/rtree.hpp>
#include <set>

template <class Point, class Value, class RTreeParameters>
class RTreeTraverser {
public:
    typedef typename  boost::geometry::index::detail::rtree::options_type<RTreeParameters>::type  node_options;
    typedef boost::geometry::index::detail::rtree::allocators<
            boost::container::new_allocator<Value>,
            Value,
            RTreeParameters,
            boost::geometry::model::box<Point>,
            typename node_options::node_tag
        >  NodeAllocator;
    typedef typename  boost::geometry::index::detail::rtree::internal_node<
            Value,
            typename boost::geometry::index::detail::rtree::options_type<RTreeParameters>::type::parameters_type,
            boost::geometry::model::box<Point>,
            NodeAllocator,
            typename node_options::node_tag
        >::type
        internal_node;

    typedef typename boost::geometry::index::detail::rtree::elements_type<internal_node>::type::const_iterator  cIter;
    typedef std::pair<cIter,cIter>  Frame;

    typedef boost::geometry::index::rtree<Value, RTreeParameters>  RTree;

    template <class Visitor>
    void traverse_frames(RTree& tree, Visitor v) {
        v(tree.bounds(), 0);
        std::set<Frame> visiteds;
        for (typename RTree::const_iterator it = tree.begin(); it!=tree.end(); ++it) {
            auto stack = it.getStack();
            unsigned i=0;
            for (const auto& frame : stack) {
                if (visiteds.insert(frame).second) {
                    v(frame.first->first, i+1);
                }
                ++i;
            }
        }
    }
};


#endif //RTREE_TRAVERSER_HPP

#include <CGAL/Cartesian.h>
#include <CGAL/Range_segment_tree_traits.h>
#include <CGAL/Range_tree_k.h>

#include <iostream>
#include <random>

typedef CGAL::Cartesian<double> K;
typedef CGAL::Range_tree_map_traits_2<K, char> Traits;
typedef CGAL::Range_tree_2<Traits> Range_tree_2_type;

int main()
{
    typedef Traits::Key Key;
    typedef Traits::Interval Interval;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(1, 2);

    std::vector<Key> points, res;
    for (int i = 0; i < 1e6; ++i)
        points.push_back(Key(K::Point_2(dis(gen), dis(gen)), 'a'));

    Range_tree_2_type tree(points.begin(), points.end());

    // std::cout << "Window Query " << std::endl;
    // for (int i = 0; i < 100; ++i) {
    //     Interval win(Interval(K::Point_2(dis(gen), dis(gen)),
    //                           K::Point_2(dis(gen), dis(gen))));
    //     tree.window_query(win, std::back_inserter(res));

    //     for (int j = 0; j < res.size(); ++j) {
    //         std::cout << res[j].first.x() << ","
    //                   << res[j].first.y() << ":"
    //                   << res[j].second << std::endl;
    //     }
    // }
}

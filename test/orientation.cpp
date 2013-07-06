#include <gtest/gtest.h>

#include "../common.h"

TEST(core, orientation)
{
    using namespace SigSpatial2013;

    std::vector<Key> InputList, OutputList;
    InputList.push_back(Key(K::Point_2(8,5.1), std::make_pair(1, 1)));
    InputList.push_back(Key(K::Point_2(1,1.1), std::make_pair(1, 1)));
    InputList.push_back(Key(K::Point_2(3,2.1), std::make_pair(1, 1)));

    Range_tree_2_type Range_tree_2(InputList.begin(),InputList.end());
    Interval win(K::Point_2(1,2),K::Point_2(9,10));

    std::cout << "\n Window Query:\n ";

    Range_tree_2.window_query(win, std::back_inserter(OutputList));
    for (size_t i = 0; i < OutputList.size(); ++i)
        std::cout << OutputList[i].first << std::endl;
    // std::vector<Key>::iterator current=OutputList.begin();
    // while(current!=OutputList.end()){
    //     std::cout << (*current).first.x() << "," << (*current).first.y()
    //               << ":" << (*current++).second.first << std::endl;
    // }

}

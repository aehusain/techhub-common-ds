#include "marshal_tree.h"
#include <boost/assign/list_of.hpp>

int
main(int argc, char *argv[])
{
    vector<int> input = boost::assign::list_of
                        (1)
                        (2)
                        (3)
                        (4)
                        (5)
                        (6)
                        (7)
                        (8)
                        (9)
                        ;
    NTree<int> tree(input);
    tree.marshal();
    tree.demarshal();
    return 0;
}

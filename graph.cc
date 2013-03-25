#include "graph.h"
#include <iostream>
#include <memory>
#include <boost/assign/list_of.hpp>
#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

int
main(int argc, char *argv[])
{
    //FIXME auto_ptr is deprecated, use unique_ptr
    vector<char> v = boost::assign::list_of
        ('a')
        ('b')
        ('c')
        ('d')
        ('e')
        ('f')
        ;

    auto_ptr<Graph<char> > g(new Graph<char>(v));

    g->connectVertex('a' ,'b');
    g->connectVertex('a', 'e');
    g->connectVertex('b', 'd');
    g->connectVertex('b', 'c');
    g->connectVertex('e', 'd');
    g->connectVertex('e', 'f');

    try {
        po::options_description desc;
        desc.add_options()
            ("help,h", "Help")
            ("dft,d", "Depth First Traversal")
            ("bft,b", "Breadth First Traversal")
            ;

        po::variables_map vm;

        try {
            po::store(po::parse_command_line(argc, argv, desc), vm);

            if (vm.count("help")) {
                cout << "Usage: \n" << desc << endl;
                return 0;
            } else if (vm.count("dft")) {
                g->dft();
                return 0;
            } else if (vm.count("bft")) {
                g->bft();
                return 0;
            }

            po::notify(vm);
        } catch (po::error &e) {
            cerr << "Error: " << e.what() << desc << endl;
            return -2;
        }
    } catch (exception &e) {
        cerr << "Unexpected exception: " << e.what() << ". Terminate program";
        cerr << endl;
        return -1;
    }

    return 0;
}

#include "../basetree.hpp"
#include <ostream>
#include <random>
#include <type_traits>


std::random_device dv;
std::mt19937 generator(dv());
std::uniform_int_distribution<int> data(0,100);
std::uniform_int_distribution<int> act(0,4);

BRtree<int> test_tree;
int main()
{
    for(int i=0;i<999;i++)
    {
        switch (act(generator)) {
            case 1:
                std::cout<<"the tree"<<(test_tree.empty()?"is":"not")<<" empty\n";
                break;
            case 2:
                test_tree.insert(data(generator));
                break;
            case 3:
                test_tree.remove(data(generator));
                break;
            case 0:
                test_tree.search(data(generator));
                break;
            case 4:
                std::cout<<"the tree size is:"<<test_tree.Size()<<std::endl;

        
        }
    }
}
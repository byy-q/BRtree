#include "../basetree.hpp"
#include "../treenode.hpp"
#include <limits>
#include <random>

std::random_device dv;
std::mt19937 generator(dv());
std::uniform_int_distribution<int> data(0,100);
std::uniform_int_distribution<int> act(0,4);
BStree<int> test_tree;
int main()
{
    int n =999;
    while(n>0)
    {
        n--;
        switch (act(generator)) {
            case 0:
                if(test_tree.empty())
                    std::cout<<"tree is empty\n";
                else
                    std::cout<<"tree not empty\n";
                break;
            
            case 1:
                std::cout<<"the tree size:"<<test_tree.Size()<<"\n";
                break;
            
            case 2:
                test_tree.search(data(generator));
                break;
            
            case 3:
                test_tree.insert(data(generator));
                break;
            
            case 4:
                test_tree.remove(data(generator));
                break;        
        }
    }

    return 0;

}
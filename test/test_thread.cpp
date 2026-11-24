#include "../basetree.hpp"
#include <iostream>
#include <shared_mutex>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <syncstream>

using namespace std::chrono;
std::random_device dv;
std::mt19937 generator(dv());
std::uniform_int_distribution<int> data(0,100);
std::uniform_int_distribution<int> act(0,4);
BRtree<int> tree;
std::shared_mutex mtx;

void worker(std::stop_token token){
    while(!token.stop_requested())    
    {
        switch (act(generator)) {
            case 0:
                {
                    std::shared_lock<std::shared_mutex> lock(mtx,std::try_to_lock);
                    if(lock.owns_lock())
                    {
                        tree.search(data(generator));
                    }
                    else {
                        std::this_thread::sleep_for(1s);
                    }
                    break;
                }
            case 1:
                {
                    std::shared_lock<std::shared_mutex> lock(mtx,std::try_to_lock);
                    if(lock.owns_lock())
                        std::osyncstream(std::cout)<<"the tree size is : "<<tree.Size()<<std::endl;
                    else
                        std::this_thread::sleep_for(1s);
                    break;
                }
            case 2:
            {
                   std::unique_lock<std::shared_mutex> lock(mtx,std::try_to_lock);
                    if(lock.owns_lock())
                        tree.insert(data(generator));
                    else
                        std::this_thread::sleep_for(1s);
                    break;
            }
            case 3:
            {
                std::unique_lock<std::shared_mutex> lock(mtx,std::try_to_lock);
                if(lock.owns_lock())
                    tree.remove(data(generator));
                else
                    std::this_thread::sleep_for(1s);
                break;
            }
        }    
    }
    return;
}

int main()
{
    std::stop_source source;
    std::jthread th1(worker);
    std::jthread th2(worker);
    std::thread th3(worker,source.get_token());

    std::this_thread::sleep_for(10s);

    th1.request_stop();
    th2.request_stop();
    source.request_stop();

    th3.join();

    return 0;

}
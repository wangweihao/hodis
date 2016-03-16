/*======================================================
    > File Name: test_malloc.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月16日 星期三 15时37分16秒
 =======================================================*/

#include <iostream>
#include <sys/time.h>
#include <memory>
#include <vector>


class Item {
    public:
    Item() {
        time_t timee;
        timep = time(&timee);
    }

    private:
    time_t timep;
};

int main() {
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);
    std::vector<std::shared_ptr<Item>> ivec;
    for(int i = 0; i < 1000000; ++i) {
        std::shared_ptr<Item> p = std::make_shared<Item> ();
        ivec.push_back(std::move(p));
    }
    gettimeofday(&end, NULL);
    std::cout << "time:" << (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec) << std::endl;
}

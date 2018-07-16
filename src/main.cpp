#include <iostream>
#include <medianFinder.h>
#include <set>
#include <chrono>
#include <vector>
#include <iomanip>
#include <limits>

int main()
{

    srand (time(NULL));
    MedianFinder finder;
    double myTime = 0.0;

    uint64_t size = 10000000;


    while (finder.size() < size)
    {
        int value = finder.size();
        auto start = std::chrono::system_clock::now();
        finder.add(value);
        finder.median();
        auto end = std::chrono::system_clock::now();
        myTime += std::chrono::duration_cast<std::chrono::duration<double> >
                                                        (end - start).count();
    }
    std::cout << "Median equals to " << std::setprecision(12) << finder.median() << std::endl;
    std::cout << "My time " << myTime << std::endl;

    return 0;
}

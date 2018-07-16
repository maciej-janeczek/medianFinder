#ifndef _MEDIAN_FINDER_H_
#define _MEDIAN_FINDER_H_

#include <heap.h>
#include <queue>

class MedianFinder
{
    uint64_t mSize;
    Heap<int, greater<int>> lowerHeap;
    Heap<int, less<int>> upperHeap;

public:
    MedianFinder();
    void add(int value);
    double median() const;
    uint64_t size() const;
};

#endif
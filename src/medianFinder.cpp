#include <stdexcept>
#include <iostream>
#include <medianFinder.h>

MedianFinder::MedianFinder() : mSize(0)
{
}

void MedianFinder::add(int value)
{
    if ( mSize % 2 == 0)
        if ( value > lowerHeap.top() )
            upperHeap.push(value);
        else
            lowerHeap.push(value);
    else
        if ( lowerHeap.size() > upperHeap.size() )
            if ( value > lowerHeap.top() )
                upperHeap.push(value);
            else
                upperHeap.push(lowerHeap.popAndPush(value));
        else
            if ( value < upperHeap.top() )
                lowerHeap.push(value);
            else
                lowerHeap.push(upperHeap.popAndPush(value));
    mSize++;
}

double MedianFinder::median() const
{
    if ( mSize == 0 ) 
        throw std::underflow_error ("MedianFinder is empty."); 

    if ( mSize % 2 == 0)
        return (lowerHeap.top() + upperHeap.top()) / 2.0;
    else
        if ( lowerHeap.size() > upperHeap.size() )
            return lowerHeap.top();
        else
            return upperHeap.top();
}

uint64_t MedianFinder::size() const
{
    return mSize;
}
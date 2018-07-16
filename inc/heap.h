// List.h 
#ifndef _HEAP_H_
#define _HEAP_H_

#include <utility>

template<typename T>
struct less 
{
    bool operator() (const T & x, const T & y) const
    {   return x < y; }
};

template<typename T>
struct greater 
{
    bool operator() (const T & x, const T & y) const
    {   return x > y; }
};


template<typename T, typename compare = less<T> >
class Heap
{
    T * mData;
    uint64_t mSize;
    uint64_t mReserved;
    const static uint64_t INIT_SIZE = 8; 
    const compare& comp = compare();

    public:
        Heap() : mData(new T[INIT_SIZE]), mSize(0), mReserved(INIT_SIZE)
        { }

        Heap( const Heap & rhs) : mData(new T[rhs.mReserved]), mSize(rhs.mSize), mReserved(rhs.mReserved)
        {
            for( int i = 0; i < rhs.mSize; i++)
            {
                mData[i] = rhs.mData[i];
            }
        }

        Heap( Heap && rhs) : mData(std::move(rhs.mData)), mSize(rhs.mSize), mReserved(rhs.mReserved)
        {
        }

        ~Heap()
        {
            delete mData;
            mData = nullptr;
        }

        const Heap & operator= (const Heap & rhs)
        {
            if ( this == &rhs )
                return *this;
            Heap copy( rhs );
            std::swap( *this, copy);
            return *this;
        }

        const Heap & operator= (Heap && rhs)
        {
            mData = std::swap(mData, rhs.mData);
            mSize = rhs.mSize;
            mReserved = rhs.mReserved;
            return *this;
        }

        bool empty() const
        {
            mSize == 0;
        }

        uint64_t size() const
        {
            return mSize;
        }

        void push(T value)
        {
            if(mSize + 1 >= mReserved)
            {
                mReserved *= 2;
                T * newData = new T[mReserved];
                for ( int i = 0; i < mSize; i++)
                {
                    newData[i] = mData[i];
                }
                std::swap(newData, mData);
                delete newData;
            }
            mData[mSize] = value;
            percolateUp(mSize);
            mSize++;
        }

        T popAndPush(T value)
        {
            T retValue = mData[0];
            mData[0] = value;
            percolateDown(0);
            return retValue;
        }

        T top() const
        {   return mData[0]; }

    private:
        void percolateUp(uint64_t idx)
        {
            uint64_t parentIdx = (idx - 1) / 2;
            if(idx > 0 && comp(mData[idx], mData[parentIdx]) )
            {
                T temp = mData[parentIdx];
                mData[parentIdx] = mData[idx];
                mData[idx] = temp;
                percolateUp(parentIdx);
            }
        }

        void percolateDown(uint64_t idx)
        {
            uint64_t leftIdx =  idx * 2 + 1;
            uint64_t rightIdx =  leftIdx + 1;
            if(rightIdx < mSize)
            {
                if(!( comp(mData[idx], mData[leftIdx]) && 
                    comp(mData[idx], mData[leftIdx + 1]) ) )
                    {
                        if (comp(mData[leftIdx], mData[rightIdx]))
                        {
                            T temp = mData[leftIdx];
                            mData[leftIdx] = mData[idx];
                            mData[idx] = temp;
                            percolateDown(leftIdx);
                        }
                        else
                        {
                            T temp = mData[rightIdx];
                            mData[rightIdx] = mData[idx];
                            mData[idx] = temp;
                            percolateDown(rightIdx);
                        }
                    }
            }
            else if (leftIdx < mSize)
            {
                if( ! comp(mData[idx], mData[leftIdx]) )
                {
                    T temp = mData[leftIdx];
                    mData[leftIdx] = mData[idx];
                    mData[idx] = temp;
                }
            }
        }
};

#endif

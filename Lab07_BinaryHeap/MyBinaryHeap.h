#ifndef __MYBINARYHEAP_H__
#define __MYBINARYHEAP_H__

#include <iostream>
#include <cstdlib>

#include "MyVector.h"

// ComparableType should be comparable (<, >, ==, >=, <= operators implemented)
// ComparableType should also have +, +=, -, -= operators implemented to support priority adjustment

template <typename ComparableType>
class MyBinaryHeap 
{
  private:
  MyVector<ComparableType> data;  // the array that holds the data elements
    
    // moves the data element at the pth position of the array up
    void percolateUp(const size_t p) 
    {
       for (size_t i = p; (i/2) > 0; i/=2)
       {
         if (data[i] > data[i/2])
         {
           std::swap(data[i], data[i/2]);
         }
       }
    }

    // moves the data element at the pth position of the array down
    void percolateDown(const size_t p) 
    {
        int pos = p;
        int child; 
        if (p > this->size())
        {
          return;
        }
        ComparableType temp = std::move(data[p]);
        for ( ; (pos*2) <= this->size(); pos = child)
        {
          child = (pos*2);
          if ((child != this->size()) && (data[child+1] > data[child]))
          {
            ++child;
          }
          if (data[child] > temp)
          {
            data[pos] = std::move(data[child]);
          }
          else 
          {
            break;
          }
        }
        data[pos] = std::move(temp);
    }

    // reorders the data elements in the array to ensure heap property
    void buildHeap() 
    {
        for (int i = (this->size()/2); i > 0; i--)
        {
          percolateDown(i);
        }
    }	

  public: 

    // default constructor
    explicit MyBinaryHeap() :
        data(1)     // reserve data[0]
    {
        
    }

    // constructor from a set of data elements
    MyBinaryHeap(const MyVector<ComparableType>& items) : 
        data(1)     // reserve data[0]
    {
        for (size_t i = 0; i < items.size(); i++)
        {
          data.push_back(items[i]);
        }
    }

    // copy constructor
    MyBinaryHeap(const MyBinaryHeap<ComparableType>& rhs) : data(rhs.data)
    {
        
    }

    // move constructor
    MyBinaryHeap(MyBinaryHeap<ComparableType> && rhs) : data(std::move(rhs.data))
    {

    }

    // copy assignment
    MyBinaryHeap& operator=(const MyBinaryHeap<ComparableType>& rhs)   
    {
        data = rhs.data;
        return *this;
    }

    // move assignment
    MyBinaryHeap& operator=(MyBinaryHeap<ComparableType> && rhs)
    {
        std::swap(data, rhs.data);
        return *this;        
    }

    // inserts x into the priority queue (copy)
    void enqueue(const ComparableType& x)
    {
        data.push_back(x);
        percolateUp(this->size());
    } 

    // inserts x into the priority queue (move)
    void enqueue(ComparableType && x)
    {
        data.push_back(std::move(x));
        percolateUp(this->size());
    }

    // accesses the data element with the highest priority
    const ComparableType& front()
    {
        return data[1];
    } 

    // deletes the data element with the highest priority from the queue
    void dequeue()
    {
        if (this->empty())
        {
          return;
        }
        data[1] = std::move(data[this->size()]);
        data.pop_back();
        percolateDown(1);
    }

    // verifies whether the array satisfies the heap property
    bool verifyHeapProperty(void)
    {
        for (size_t i = 1; i < (this->size()/2); i++)
        {
            if ((data[i] < data[2*i]) || (data[i] < data[(2*i)+1]))
            {
               return false;
            }
        }
        return true;
    }

    // disrupts heap property by random shuffling
    void disruptHeapProperty(void)
    {
        if(data.size() <= 3)
            return;
        for(size_t i = 0; i < 1000; ++ i)
        {
            size_t p = 1 + ((int) rand()) % (data.size() - 1);
            size_t q = 1 + ((int) rand()) % (data.size() - 1);
            std::swap(data[p], data[q]);
        }
        return;
    }

    // merges two heaps; the second heap can be destructed after the merge
    MyBinaryHeap& merge(MyBinaryHeap<ComparableType> && rhs) 
    {
       for (size_t i = 1; i <= rhs.size(); i++)
       {
         data.push_back(rhs.data[i]);
       }
       buildHeap();
       return *this;
    }

    // increases the priority measure of an element at a specific position and reorder the heap
    void increaseKey(const size_t p, const unsigned int d)
    {    
       data[p] = data[p] + d;
       percolateUp(p);
    }

    // decreases the priority measure of an element at a specific position and reorder the heap
    // if the current priority is smaller than the requested decrement, assign priority 0
    void decreaseKey(const size_t p, const unsigned int d) 
    {
       if (data[p] < d)
       {
         data[p] = 0;
       }
       else 
       {
         data[p] = data[p] - d;         
       }
       percolateDown(p);   
    }

    // checks whether the heap is empty
    bool empty()
    {
        return(this->size() == 0);
    }

    // removes all data elements from the heap
    void clear()
    {
        while (!this->empty())
        {
          data.pop_back();
        }
    }

    // returns the size of the heap
    size_t size()
    {
        return (data.size()-1);
    }

    // prints the data in the array
    void print() 
    {
        const char delim = '\t';
        for (size_t i = 1; i < data.size(); ++ i) {
            std::cout << data[i] << delim;
        }
        std::cout << std::endl;
        return;
    }

};

#endif // __MYBINARYHEAP_H__
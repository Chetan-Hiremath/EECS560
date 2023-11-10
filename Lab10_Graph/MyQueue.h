#ifndef __MYQUEUE_H__
#define __MYQUEUE_H__

#include <iostream>
#include <algorithm>

#include "MyVector.h"

template <typename DataType>
class MyQueue : private MyVector<DataType>
{  
  private:
    size_t dataStart, dataEnd;

    // changes the size of the array to newSize
    void resize(size_t newSize)
    {
        MyVector<DataType>::resize(newSize);
    }

    // requests for newCapacity amount of space
    void reserve(size_t newCapacity)
    {
        size_t tempReserve = capacity();
        MyVector<DataType>::reserveQueue(newCapacity, dataStart, dataEnd);
        dataStart = 0;
        dataEnd = (tempReserve - 2);
    }

  public:

    // default constructor
    explicit MyQueue(size_t initSize = 0) : MyVector<DataType>(initSize)
    {
        dataStart = 0;
        dataEnd = (capacity() - 1);
    }

    // copy constructor
    MyQueue(const MyQueue & rhs) : 
    MyVector<DataType>(rhs),
    dataStart{rhs.dataStart},
    dataEnd{rhs.dataEnd}
    {
        
    }

    // move constructor
    MyQueue(MyQueue && rhs) :
    MyVector<DataType>(rhs),
    dataStart{rhs.dataStart},
    dataEnd{rhs.dataEnd}
    {
        rhs.dataStart = 0;
        rhs.dataEnd = rhs.capacity() - 1;
    }

    // destructor
    ~MyQueue()
    {

    }

    // copy assignment
    MyQueue & operator= (const MyQueue & rhs)
    {
        MyVector<DataType>::operator=(rhs);
        return *this;
    }

    // move assignment
    MyQueue & operator= (MyQueue && rhs)
    {
        MyVector<DataType>::operator=(std::move(rhs));
        return *this;
    }

    // insert x into the queue
    void enqueue(const DataType & x)
    {
        if (size() == (capacity() - 1))
        {
          reserve(2 * (capacity()) + 1);
        }
        if (dataEnd == (capacity() - 1))
        {
          dataEnd = 0;
        }
        else
        {
          dataEnd++;
        }
        MyVector<DataType>::insertQueue(x, dataEnd);
    }

    // insert x into the queue
    void enqueue(DataType && x)
    {
        if (size() == (capacity() - 1))
        {
          reserve(2 * (capacity()) + 1);
        }
        if (dataEnd == (capacity() - 1))
        {
          dataEnd = 0;
        }
        else
        {
          dataEnd++;
        }
        MyVector<DataType>::insertQueue(std::move(x), dataEnd);
    }

    // remove the first element from the queue
    void dequeue(void)
    {
        if (dataStart == (this->capacity() - 1))
        {
          dataStart = 0;
        }
        else 
        {
          dataStart++;
        }
    }

    // access the first element of the queue
    const DataType & front(void) const
    {
        return MyVector<DataType>::operator[](this->dataStart);
    }

    // check if the queue is empty; return TRUE is empty; FALSE otherwise
    bool empty(void) const
    {
        if (size() == 0)
        {
          return true;
        }
        else
        {
          return false;
        }
    }

    // access the size of the queue
    size_t size() const
    {
        int tempSize = dataEnd - dataStart; 
        if (tempSize == -1 || tempSize == (capacity() - 1))
        {
          return 0;
        }
        else if (tempSize >= 0)
        {
          return (tempSize + 1);
        }
        else
        {
          return (tempSize + capacity() + 1);
        }
    }

    // access the capacity of the queue
    size_t capacity(void) const 
    {
        return MyVector<DataType>::capacity();
    }

};


#endif // __MYQUEUE_H__

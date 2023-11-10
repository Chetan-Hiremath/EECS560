#ifndef _MY_DISJOINTSETS_H_
#define _MY_DISJOINTSETS_H_

#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "MyVector.h"

class MyDisjointSets 
{
  public:
    // constructor
    explicit MyDisjointSets(const size_t size) 
    {
        for (int i = 0; i < size; i++)
        {
          parents.push_back(-1);
        }
        numSets = size;
        theSize = size;
    }

    // destructor
    ~MyDisjointSets(void)
    {
        
    }

    //makes the cells for the maze by calling the constructor that has the parameter of size = 0
    void makeCells(const size_t size) 
    {
        MyDisjointSets(0);
    }

    // finds the ID of the set that contains x
    // implements path compression
    int find(const int x)
    {
        if (parents[x] < 0)
        {
          return x;
        }
        else 
        {
          parents[x] = find(parents[x]);
          return parents[x];
        }
    }

    // unions the two sets represented by r1 and r2, respectively
    // (r1 and r2 should be roots)
    // implements the union-by-size approach
    void unionSets(const int r1, const int r2)
    {
          if (setSize(r1) >= setSize(r2))
          {
             parents[r2] = parents[r2] + parents[r1];
             parents[r1] = r2;
          }
          else
          {
             parents[r1] = parents[r1] + parents[r2];
             parents[r2] = r1;
          }
          numSets--;
    }

    // returns the size of the set that contains x
    size_t setSize(const int x)
    {
        return abs(parents[find(x)]);
    }

    // returns the total number of elements in the disjoint set
    size_t size()
    {
        return theSize;
    }

    // returns the number of disjoint sets
    size_t numDisjointSets()
    {
        return numSets;
    }

  private:
    size_t theSize;             // the number of elements in the disjoint sets
    size_t numSets;             // the number of disjoint sets maintained
    MyVector<int> parents;      // the info array; if the index is not the root, it contains the ID of its parent; otherwise it contains the size of the set as a negative number

};

#endif  // _MY_DISJOINTSETS_H_

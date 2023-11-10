#ifndef _MYMAZE_H_
#define _MYMAZE_H_

#include <iostream>
#include <cstdlib>

#include "MyDisjointSets.h"

class MyMaze 
{
  public:
	
    // constructor
    explicit MyMaze(const size_t r, const size_t c) : dSets(r*c)
    {
        numRows = r;
        numCols = c;
        dSets.makeCells(r*c);
        topDownWalls = new bool*[numRows - 1];
        for (int i = 0; i < (numRows - 1); i++)
        {
          topDownWalls[i] = new bool[numCols];
          for (int j = 0; j < numCols; j++)
          {
            topDownWalls[i][j] = true;
          }
        }
        leftRightWalls = new bool*[numRows];
        for (int i = 0; i < numRows; i++)
        {
          leftRightWalls[i] = new bool[numCols - 1];
          for (int j = 0; j < (numCols - 1); j++)
          {
            leftRightWalls[i][j] = true;
          }
        }
    }

    // destructor
    ~MyMaze(void)
    {
        for (int i = 0; i < numRows; i++)
        {
          delete[] leftRightWalls[i];
          if (i != (numRows - 1))
          {
            delete[] topDownWalls[i];
          }
        }
        delete[] leftRightWalls;
        delete topDownWalls;
    }

    // builds a random maze
    // the top wall of the first cell (the top-left corner) should be open as the entrance
    // the bottom wall of the last cell (the bottom-right corner) should be open as the exit
    // algorithm hint: keep breaking walls until the entrance and exit become connected
    void buildMaze(void)
    {
        int entrance = 0;
        int exit = (numRows * numCols) - 1;
        while(dSets.find(entrance) != dSets.find(exit))
        {
          if (rand() % 2)
          {
            int x = rand() % (numRows - 1);
            int y = rand() % numCols;
            int firstCell = (x * numCols) + y;
            int lastCell = ((x + 1) * numCols) + y;
            if (dSets.find(firstCell) != dSets.find(lastCell))
            {
              topDownWalls[x][y] = false;
              dSets.unionSets(dSets.find(firstCell), dSets.find(lastCell));
            }
          }
          else
          {
            int x = rand() % numRows;
            int y = rand() % (numCols - 1);
            int firstCell = (x * numCols) + y;
            int lastCell= (x * numCols) + (y + 1);
            if (dSets.find(firstCell) != dSets.find(lastCell))
            {
              leftRightWalls[x][y] = false;
              dSets.unionSets(dSets.find(firstCell), dSets.find(lastCell));
            }
          } 
        }
    }

    // prints the maze
    // for top-down walls you can use '-' (ASCII code 45)
    // for left-right walls you can use '|' (ASCII code 124)
    // feel free to try out other symbols if your system supports extended ASCII and/or UTF-8
    void printMaze(std::ostream& out = std::cout)
    {
        out << "\n";
        for (int i = 0; i < ((2 * numRows) + 1); i++)
        {
          if (i % 2)
          {
            for (int j = 0; j <= numCols; j++)
            {
              if (j == 0)
              {
                out << "| ";
              }
              else if (j == numCols)
              {
                out << "|\n";
              }
              else if (leftRightWalls[(i-1)/2][j-1])
              {
                out << "| ";
              }
              else 
              {
                out << "  ";
              }
            }
          }
          else
          {
            for (int j = 0; j < numCols; j++)
            {
              if (i == 0)
              {
                if (j == 0)
                {
                  out << "  ";
                }
                else if (j == (numCols - 1))
                {
                  out << " -\n";
                }
                else 
                {
                  out << " -";
                }
              }
              else if (i == (2 * numRows))
              {
                if (j == (numCols - 1))
                {
                  out << " \n";
                }
                else 
                {
                  out << " -";
                }
              }
              else
              {
                if (topDownWalls[(i-2)/2][j])
                {
                  out << " -";
                }
                else 
                {
                  out << "  ";
                }
                if (j == (numCols - 1))
                {
                  out << "\n";
                }
              }
            }
          }
        }
      out << "\n";
    }

  private:
    size_t numRows = 0;         // the number of rows of the maze
    size_t numCols = 0;         // the number of columns of the maze
    MyDisjointSets dSets;       // the disjoint set class that keeps track of maze cell connection
    bool** leftRightWalls;      // the 2D array that indicates whether walls between left-right neighbors present
    bool** topDownWalls;        // the 2D array that indicates whether walls between the top-down neighbors preset

};


#endif // __MYMAZE_H__

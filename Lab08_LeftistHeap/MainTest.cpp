#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "MyLeftistHeap.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        cout << "Please provide the input file." << endl;
        exit(1);
    }

    MyLeftistHeap<unsigned int> lsth1, lsth2, lsth3, lsth4;
    ifstream inFile;
    inFile.open(argv[1]);

    if (inFile.is_open())
    {
        // testing insert
        string str;
        while(getline(inFile, str))
        {
            int num = stoi(str);
            lsth1.enqueue(static_cast<unsigned int>(num));
            cout << "Data inserted." << "\t";
            cout << "Highest priority:" << lsth1.front() << endl;            
        }

        // testing heap property
        if(lsth1.verifyHeapProperty())
            cout << "Leftist Heap 1 property checked." << endl;
        else
            cout << "Leftist Heap 1 property violated." << endl;

        // testing increaseKey and decreaseKey
        lsth2 = lsth3 = lsth4 = lsth1;
        for(size_t i = 0; i < 10; ++ i)
        {
            MyLeftistHeap<unsigned int>::HeapNode* t = lsth2.randNode();
            unsigned int d = ((int) rand()) % 999999;
            int inc_tag = ((int) rand()) % 2;
            if(inc_tag)
                lsth2.increaseKey(t, d);
            else
                lsth2.decreaseKey(t, d);
        }
        if(lsth2.verifyHeapProperty())
            cout << "Leftist Heap 2 property checked." << endl;
        else
            cout << "Leftist Heap 2 property violated." << endl;
        
        cout << "Leftist heap 1 size before merge: " << lsth1.size() << endl;
        cout << "Leftist heap 2 size before merge: " << lsth2.size() << endl;
        lsth1.merge(move(lsth2));
        cout << "Leftist heap 1 size after merge: " << lsth1.size() << endl;
        cout << "Leftist heap 2 size after merge: " << lsth2.size() << endl;
        if(lsth1.verifyHeapProperty())
            cout << "Leftist Heap 1 property checked." << endl;
        else
            cout << "Leftist Heap 1 property violated." << endl;

        // testing front and dequeue
        while(!lsth1.empty())
        {
            unsigned int v = lsth1.front();
            cout << "Highest priority: " << v << endl;
            lsth1.dequeue();
        }

        // testing verifyHeapProperty
        lsth3.disruptHeapProperty();
        if(lsth3.verifyHeapProperty())
            cout << "Leftist Heap 3 property checked." << endl;
        else
            cout << "Leftist Heap 3 violated." << endl;

        lsth4.disruptLeftistTopology();
        if(lsth4.verifyHeapProperty())
            cout << "Leftist Heap 4 property checked." << endl;
        else
            cout << "Leftist Heap 4 property violated." << endl;

        // testing size()
        cout << "Leftist heap 1 final size: " << lsth1.size() << endl;
        cout << "Leftist heap 2 final size: " << lsth2.size() << endl;
        cout << "Leftist heap 3 final size: " << lsth3.size() << endl;
        cout << "Leftist heap 4 final size: " << lsth4.size() << endl;

    }
    else
    {
            cout << "The input file cannot be opened." << endl;
    }
    inFile.close();
    return 0;
}
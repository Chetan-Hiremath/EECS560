#ifndef __MYLEFTISTHEAP_H__
#define __MYLEFTISTHEAP_H__

#include <iostream>
#include <cstdlib>
#include <stdexcept>

// ComparableType should be comparable (<, >, ==, >=, <= operators implemented)
// ComparableType should also have +, +=, -, -= operators implemented to support priority adjustment

template <typename ComparableType>
class MyLeftistHeap
{
  public:
    // definition of HeapNode
    struct HeapNode
    {
        ComparableType data;
        HeapNode* left;
        HeapNode* right;
        HeapNode* parent;
        int npl;

        HeapNode(const ComparableType & d, HeapNode* lt = nullptr, HeapNode* rt = nullptr, HeapNode* pt = nullptr, int np = 0) :
            data{d}, 
            left{lt}, 
            right{rt},
            parent{pt},
            npl{np} 
        {
            ;
        }
    
        HeapNode(ComparableType && d, HeapNode* lt = nullptr, HeapNode* rt = nullptr, HeapNode* pt = nullptr, int np = 0) :
            data{std::move(d)}, 
            left{lt}, 
            right{rt},
            parent{pt},
            npl{np} 
        {
            ;
        }
    };
    
    // default constructor
    MyLeftistHeap() :
        theSize(0)
    {
        root = nullptr;
    }

    // copy constructor
    MyLeftistHeap(const MyLeftistHeap& rhs) :
        theSize(rhs.theSize)
    {
        root = clone(rhs.root);
    }
  
    // move constructor
    MyLeftistHeap(MyLeftistHeap && rhs) :
        theSize(std::move(rhs.theSize))
    {
        root = rhs.root;
        rhs.root = nullptr;
    }

    // destructor
    ~MyLeftistHeap( )
    { 
        clear();
    }

    // copy assignment
    MyLeftistHeap& operator=(const MyLeftistHeap& rhs)
    {
        clear();
        MyLeftistHeap copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    // move assignment
    MyLeftistHeap& operator=(MyLeftistHeap && rhs)
    {   
        clear();
        std::swap(root, rhs.root);
        std::swap(theSize, rhs.theSize);
        return *this;
    }

    // returns the data element with the highest priority
    const ComparableType& front() const
    {
        return (root->data);
    }

    // inserts x into the priority queue (copy)
    void enqueue(const ComparableType& x)
    {
        root = merge(new HeapNode(x, nullptr, nullptr, nullptr, 0), root);
        theSize++;
    }
  
    // inserts x into the priority queue (move)
    void enqueue(ComparableType && x)
    {
        root = merge(new HeapNode(std::move(x), nullptr, nullptr, nullptr, 0), root);
        theSize++;
    }

    // deletes the data element with the highest priority from the queue
    void dequeue()
    {
        if (empty())
        {
          return;
        }
        HeapNode *temp = root;
        root = merge(temp->left, temp->right);
        delete temp;
        --theSize;
    }

    // increases the priority measure of the data element locates at node t
    void increaseKey(HeapNode *t, const unsigned int d)
    {    
        t->data = t->data + d;
        percolateUp(t);
    }

    // decreases the priority measure of the data element locates at node t
    // if the current priority is smaller than the requested decrement, assign priority 0
    void decreaseKey(HeapNode* t, const unsigned int d) 
    {
       if (t->data < d)
       {
         t->data = 0;
       }
       else 
       {
         t->data = t->data - d;         
       }
       percolateDown(t);    
    }

    // merges two leftist heaps
    void merge(MyLeftistHeap && rhs)
    {
       if (this == &rhs)
       {
         return;
       }
       root = merge(root, rhs.root);
       theSize = theSize + rhs.theSize;
       rhs.theSize = 0;
       rhs.root = nullptr;
    }

    // verifies whether the subtree rooted at t satisfies the heap property
    bool verifyHeapProperty(void)
    {
        return verifyHeapProperty(root);
    }

    // returns a random node in the heap
    HeapNode* randNode(void)
    {
        if(empty())
            return nullptr;
        
        size_t num_trials = theSize;    // the number of steps of the random walk
        HeapNode* n = root;             // begins the random walk at the root
        // defines all three neighbors of a given node as: the parent, the left child, and the right child
        // they are stored in order in the size-3 array "neighbors"
        HeapNode** neighbors = new HeapNode* [3];   
        for(size_t i = 0; i < num_trials; ++ i)
        {
            neighbors[0] = n->parent;
            neighbors[1] = n->left;
            neighbors[2] = n->right;
            // picks a random neighbor to proceed
            size_t k = ((int) rand()) % 3;
            while(neighbors[k] == nullptr)
            {
                ++ k;
                if(k > 2)   k -= 3;
            }
            n = neighbors[k];
        }
        delete [] neighbors;
        return n;
    }

    // disrupts heap property by randomly shuffling the nodes
    void disruptHeapProperty(void)
    {
        if(empty())
            return;
        
        HeapNode* n = root;             // begins the random walk at the root
        // defines all three neighbors of a given node as: the parent, the left child, and the right child
        // they are stored in order in the size-3 array "neighbors"
        HeapNode** neighbors = new HeapNode* [3];
        HeapNode** path = new HeapNode* [theSize];
        for(size_t i = 0; i < theSize; ++ i)
        {
            neighbors[0] = n->parent;
            neighbors[1] = n->left;
            neighbors[2] = n->right;
            // picks a random neighbor to proceed
            size_t k = ((int) rand()) % 3;
            while(neighbors[k] == nullptr)
            {
                ++ k;
                if(k > 2)   k -= 3;
            }
            path[i] = n = neighbors[k];
        }
        // swap
        for(size_t i = 0; i < theSize; ++ i)
        {
            size_t x = ((int) rand()) % theSize;
            size_t y = ((int) rand()) % theSize;
            std::swap(path[x]->data, path[y]->data);
        }
        delete [] neighbors;
        delete [] path;
        return;
    }

    // disrupts heap topology by randomly swapping left-right children
    void disruptLeftistTopology(void)
    {
        if(empty())
            return;
        
        for(size_t i = 0; i < theSize; ++ i)
        {
            HeapNode* t = randNode();
            std::swap(t->left, t->right);
        }
        return;
    }

    // clears all data in the queue
    void clear()
    {
        clear(root);
    }

    // checks of the heap is empty
    bool empty() const
    { 
        return(theSize == 0);
    }

    // checks the size of the heap
    size_t size() const
    {
        return theSize;
    }

  private:

    HeapNode* root; // root of the heap tree
    size_t theSize; // the number of data elements

    // merges the two trees rooted at r1 and r2, respectively
    HeapNode* merge(HeapNode* r1, HeapNode* r2)
    {
        if (r1 == nullptr)
        {
          return r2;
        }
        if (r2 == nullptr)
        {
          return r1;
        }
        if (r1->data > r2->data)
        {
          r1->parent = nullptr;
          return mergeSubtree(r1, r2);
        }
        else
        {
          r2->parent = nullptr;
          return mergeSubtree(r2, r1);
        }
    }

    // merges st (sub-tree) with the right subtree of mt (main-tree)
    HeapNode* mergeSubtree(HeapNode* mt, HeapNode* st)
    {
        if (mt->left == nullptr)
        {
          st->parent = mt;
          mt->left = st;
        }
        else
        {
          mt->right = merge(mt->right, st);
          st->parent = mt;
          if (mt->left->npl < mt->right->npl)
          {
            swapChildren(mt);
          }
          mt->npl = (mt->right->npl) + 1;
        }
        return mt;
    }

    // swaps the two children of the node t
    void swapChildren(HeapNode* t)
    {
        std::swap(t->left, t->right);
    }

    // deletes the subtree rooted at t
    void clear(HeapNode *t)
    {
        if (t != nullptr)
        {
          clear(t->left);
          clear(t->right);
          delete t;
          --theSize;
        }
        t == nullptr;
    }

    // clones the tree rooted at t; returns the root of the cloned tree
    HeapNode* clone(HeapNode* t)
    {
        if (t == nullptr)
        {
          return nullptr;
        }
        else
        {
          HeapNode* treeClone = new HeapNode(t->data, clone(t->left), clone(t->right), nullptr, t->npl);
          if (treeClone->left != nullptr)
          {
            treeClone->left->parent = treeClone;
          }
          if (treeClone->right != nullptr)
          {
            treeClone->right->parent = treeClone;
          }
          return treeClone;
        }
    }

    // percolates the node t up the heap
    void percolateUp(HeapNode* t)
    {
        if (t->parent == nullptr)
        {
          return;
        }
        if (t->parent->data < t->data)
        {
          std::swap(t->parent->data, t->data);
          percolateUp(t->parent);
        }
        else 
        {
          return;
        }
    }

    // percolates the node t down the heap
    void percolateDown(HeapNode* t)
    {
        if (t->left == nullptr && t->right == nullptr)
        {
          return;
        }
        if (t->left != nullptr && t->right != nullptr)
        {
          if (t->left->data > t->right->data)
          {  
            if (t->left->data > t->data)
            {  
              std::swap(t->data, t->left->data);
              percolateDown(t->left);
              return;
            }
          }
          else 
          {
            if (t->right->data > t->data)
            {  
              std::swap(t->data, t->right->data);
              percolateDown(t->right);
              return;
            }
          }
        }
        if (t->left != nullptr && t->right == nullptr)
        {
          if (t->left->data > t->data)
          {  
              std::swap(t->data, t->left->data);
              percolateDown(t->left);
              return;
          }
          else if (t->left == nullptr && t->right != nullptr)
          {
            if (t->right->data > t->data)
            {  
              std::swap(t->data, t->right->data);
              percolateDown(t->right);
              return;
            }
          }
        }
    }

    // verifies whether the subtree rooted at t satisfies the heap property and leftist topology
    bool verifyHeapProperty(HeapNode* t)
    {
        if (t->left != nullptr && t->right != nullptr)
        {
          if ((t->data < t->left->data) || (t->data < t->right->data))
          {
            return false;
          }
          if (t->left->npl < t->right->npl)
          {
            return false;
          }
          return (verifyHeapProperty(t->left) && verifyHeapProperty(t->right));
        }
        else if (t->left != nullptr)
        {
          if (t->data < t->left->data)
          {
            return false;
          }
        }
        else if (t->right != nullptr)
        {
          return false;
        }
        return true;
    }

};

#endif // __MYLEFTISTHEAP_H__

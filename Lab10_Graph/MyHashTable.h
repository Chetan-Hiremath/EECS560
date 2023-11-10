#ifndef __MYHASHTABLE_H__
#define __MYHASHTABLE_H__

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <string>

#include "MyVector.h"
#include "MyLinkedList.h"

static const long long uh_param_a = 53;       // universal hash function parameter a
static const long long uh_param_b = 97;       // universal hash function parameter b
static const long long prime_digits = 19;     // parameter used for finding a Mersenne prime
static const long long mersenne_prime = (1 << prime_digits) - 1;  // the Mersenne prime for universal hashing

// fast calculation of (n modulo mersenne_prime)
long long fastMersenneModulo(const long long n)
{
  return (n >> prime_digits) + (n & mersenne_prime);
}

// definition of the template hash function class
template <typename KeyType>
class HashFunc
{
  public:
    long long univHash(const KeyType key, const long long table_size) const;
};

// the hash function class that supports the hashing of the "long long" data type
template <>
class HashFunc<long long>
{
  public:
    long long univHash(const long long key, const long long table_size) const
    {
      long long hv = fastMersenneModulo(static_cast<long long>(uh_param_a * key + uh_param_b));
      hv = hv % table_size;
      return hv;
    }
};

// the has function class that supports the hashing of the "std::string" data type
template <>
class HashFunc<std::string>
{
  private:
    const int param_base = 37;    // the base used for inflating each character
  public:    
    long long univHash(const std::string& key, const long long table_size) const
    {
      long long hv = 0;
      for(size_t i = 0; i < key.length(); ++i)
      {
        hv = param_base * hv + static_cast<long long>(key[i]);
      }
      hv = fastMersenneModulo(static_cast<long long>(uh_param_a * hv + uh_param_b));
      hv = hv % table_size;
      return hv;
    }
};

// definition of the template hashed object class
template <typename KeyType, typename ValueType>
class HashedObj
{
  public:
    KeyType key;
    ValueType value;

    HashedObj()
    {
      return;
    }

    HashedObj(const KeyType& k, const ValueType& v) :
      key(k),
      value(v)
    {
      return;
    }
   
    HashedObj(KeyType && k, ValueType && v) :
      key(std::move(k)),
      value(std::move(v))
    {
      return;
    }

    bool operator==(const HashedObj<KeyType, ValueType>& rhs)
    {
      return (key == rhs.key);
    }    

    bool operator!=(const HashedObj<KeyType, ValueType>& rhs)
    {
      return !(*this == rhs);
    }   

};

template <typename KeyType, typename ValueType>
class MyHashTable
{ 
  private:
    size_t theSize; // the number of data elements stored in the hash table
    MyVector<MyLinkedList<HashedObj<KeyType, ValueType> >* > hash_table;    // the hash table implementing the separate chaining approach
    MyVector<size_t> primes;    // a set of precomputed and sorted prime numbers
    HashFunc<KeyType> myHashFunc; // need to create a HashFunc object with the appropriate KeyType
    size_t sieveSize = 1000; //Initial size of the sieve. Populates the primes MyVector with all primes <= sieveSize

    // pre-calculate a set of primes using the sieve of Eratosthenes algorithm
    // will be called if table doubling requires a larger prime number for table size
    // expected to update the private member "primes"
    void preCalPrimes(const size_t n)
    {
      MyVector<bool> isPrime(n + 1);
      for (int i = 0; i < (n + 1); i++)
      {
        isPrime[i] = true;
      }
      for (int p = 2; (p * p) <= n; p++)
      {
        if (isPrime[p] == true)
        {
          for (int i = (p * p); i <= n; i += p)
          {
            isPrime[i] = false;
          }
        }
      }
      if (primes.empty())
      {
        for (int p = 2; p <= n; p++)
        {
          if (isPrime[p])
          {
            primes.push_back(p);
          }
        }
      }
      else
      {
        for (int p = 2; p <= n; p++)
        {
          if ((p > primes.back()) && (isPrime[p]))
          {
            primes.push_back(p);
          }
        }
      }
    }

    // finding the smallest prime that is larger than or equal to n
    // should perform binary search against the private member "primes"
    size_t nextPrime(const size_t n)
    {
      while (primes.back() < n)
      {
        sieveSize = sieveSize * 2;
        preCalPrimes(sieveSize);
      }
      size_t low = 0;
      size_t high = primes.size() - 1;
      while(low != high)
      {
        size_t mid = ((high + low) / 2);
        if (primes[mid] < n)
        {
          low = mid + 1;
        }
        else
        {
          high = mid;
        }
      }
      return primes[low];
    }

    // finds the MyLinkedList itertor that corresponds to the hashed object that has the specified key
    // returns the end() iterator if not found
    typename MyLinkedList<HashedObj<KeyType, ValueType> >::iterator find(const KeyType& key)
    {
      auto & bucket = hash_table[myHashFunc.univHash(key, hash_table.size())];
      auto itr = bucket->begin();
      for(; itr != bucket->end(); itr++)
      {
        if (key == (*itr).key)
        {
          return itr; 
        }
      }
      return itr; 
    }

    // rehashes all data elements in the hash table into a new hash table with new_size
    // note that the new_size can be either smaller or larger than the existing size
    void rehash(const size_t new_size)
    {
      size_t old_size = hash_table.size();
      MyVector<MyLinkedList<HashedObj<KeyType, ValueType> >* > new_table;
      new_table.reserve(new_size);
      for (size_t i = 0; i < new_size; i++)
      {
        new_table.push_back(new MyLinkedList<HashedObj<KeyType, ValueType> >);
      }
      for (size_t i = 0; i < old_size; i++)
      {
        auto & bucket = hash_table[i];
        for (auto itr = bucket->begin(); itr != bucket->end(); itr++)
        {
          auto & newBucket = new_table[myHashFunc.univHash((*itr).key, new_size)];
          newBucket->push_back(*itr);
        }
      }
      std::swap(hash_table, new_table); 
      for (size_t i = 0; i < old_size; i++)
      {
        delete new_table[i];
      }
    }

    // doubles the size of the table and perform rehashing
    // the new table size should be the smallest prime that is larger than the expected new table size (double of the old size)
    void doubleTable()
    {
      size_t new_size = nextPrime(2 * hash_table.size());
      if (new_size == 7)
      {
        this->rehash(17);
      } 
      else
      {
	this->rehash(new_size);
      }
      return;
    }

    // halves the size of the table and perform rehahsing
    // the new table size should be the smallest prime that is larger than the expected new table size (half of the old size)
    void halveTable()
    {
      size_t new_size = nextPrime(ceil(hash_table.size() / 2));
      rehash(new_size);
      return;
    }

  public:

    // the default constructor; allocate memory if necessary
    explicit MyHashTable(const size_t init_size = 3)
    {
      preCalPrimes(sieveSize); 
      hash_table.reserve((2 * init_size) + 1); 
      for(int i = 0; i < init_size; i++)
      {
        hash_table.push_back(new MyLinkedList<HashedObj<KeyType, ValueType> >);
      }
      theSize = 0;
    }

    // the default destructor; collect memory if necessary
    ~MyHashTable()
    {
      for(int i = 0; i < hash_table.size(); i++)
      {
        delete hash_table[i];
      }
    }

    // checks if the hash tabel contains the given key
    bool contains(const KeyType& key)
    {
      auto & bucket = hash_table[myHashFunc.univHash(key, hash_table.size())];
      return (find(key) != bucket->end());
    }

    // retrieves the data element that has the specified key
    // returns true if the key is contained in the hash table
    // return false otherwise
    bool retrieve(const KeyType& key, HashedObj<KeyType, ValueType>& data)
    {
      auto & bucket = hash_table[myHashFunc.univHash(key, hash_table.size())];
      auto itr = find(key);
      if (itr != bucket->end())
      {
        data = *itr;
        return true;
      }
      else
      {
        return false;
      }
    }

    // inserts the given data element into the hash table (copy)
    // returns true if the key is not contained in the hash table
    // return false otherwise
    bool insert(const HashedObj<KeyType, ValueType>& x)
    {
      auto & bucket = hash_table[myHashFunc.univHash(x.key, hash_table.size())];
      if (find(x.key) != bucket->end())
      {
        return false;
      }
      bucket->push_back(x);
      if ((theSize++ * 2) > hash_table.capacity())
      {
        doubleTable();
      }
      return true;
    }

    // inserts the given data element into the hash table (move)
    // returns true if the key is not contained in the hash table
    // return false otherwise
    bool insert(HashedObj<KeyType, ValueType> && x)
    {
      auto & bucket = hash_table[myHashFunc.univHash(x.key, hash_table.size())];
      if (find(x.key) != bucket->end())
      {
        return false;
      }
      bucket->push_back(std::move(x));
      if ((theSize++ * 2) > hash_table.capacity())
      {
        doubleTable();
      }
      return true;
    }

    // removes the data element that has the key from the hash table
    // returns true if the key is contained in the hash table
    // returns false otherwise
    bool remove(const KeyType& key)
    {
      auto & bucket = hash_table[myHashFunc.univHash(key, hash_table.size())];
      auto itr = find(key);
      if (find(key) == bucket->end())
      {
        return false;
      }
      bucket->erase(itr);
      theSize--;
      if (theSize <= (hash_table.capacity() / 8))
      {
        halveTable();
      }
      return true;
    }

    // obtains the keys for the data elements in the hash table
    void keys(MyVector<KeyType> & key_array)
    {
      for(size_t i = 0; i < hash_table.size(); ++i)
      {
        for(auto itr = hash_table[i]->begin(); itr != hash_table[i]->end(); ++itr)
        {
          key_array.push_back((*itr).key);
        }
      }
      return;
    }

    // returns the number of data elements stored in the hash table
    size_t size()
    {
      return theSize;
    }

    // returns the capacity of the hash table
    size_t capacity()
    {
      return (hash_table.size());
    }

};


#endif // __MYHASHTABLE_H__

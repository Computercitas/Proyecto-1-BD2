// ExtendibleHashing.h
#ifndef EXTENDIBLE_HASHING_H
#define EXTENDIBLE_HASHING_H

#include <iostream>
#include <vector>
#include <list>
#include <cmath>

template <typename T>
class ExtendibleHashing
{
public:
    ExtendibleHashing(int initialDepth = 2);
    void insert(int key, const T &record);
    T *search(int key);
    bool remove(int key);

private:
    struct Bucket
    {
        std::list<T> records;
        int localDepth;
    };

    int globalDepth;
    std::vector<Bucket *> directory;

    int hash(int key) const;
    void splitBucket(int index);
};

template <typename T>
ExtendibleHashing<T>::ExtendibleHashing(int initialDepth)
{
    globalDepth = initialDepth;
    directory.resize(1 << globalDepth);
    for (int i = 0; i < (1 << globalDepth); ++i)
    {
        directory[i] = new Bucket{std::list<T>(), 0};
    }
}

template <typename T>
int ExtendibleHashing<T>::hash(int key) const
{
    return key & ((1 << globalDepth) - 1);
}

template <typename T>
void ExtendibleHashing<T>::insert(int key, const T &record)
{
    int index = hash(key);
    Bucket *bucket = directory[index];

    // Insert the record in the bucket
    bucket->records.push_back(record);

    // If the bucket overflows, split it
    if (bucket->records.size() > 4) // Arbitrary threshold for bucket size
    {
        splitBucket(index);
    }
}

template <typename T>
T *ExtendibleHashing<T>::search(int key)
{
    int index = hash(key);
    Bucket *bucket = directory[index];

    for (auto &record : bucket->records)
    {
        // Assuming that the key is part of the record and we can check it
        // Here we assume that T has a 'key' attribute or a method to access it
        if (record.dni == key) // For AdultRecord, replace with `record.dni`
        {
            return &record;
        }
    }

    return nullptr;
}

template <typename T>
bool ExtendibleHashing<T>::remove(int key)
{
    int index = hash(key);
    Bucket *bucket = directory[index];

    for (auto it = bucket->records.begin(); it != bucket->records.end(); ++it)
    {
        if (it->dni == key) // For AdultRecord, replace with `it->dni`
        {
            bucket->records.erase(it);
            return true;
        }
    }

    return false;
}

template <typename T>
void ExtendibleHashing<T>::splitBucket(int index)
{
    // Split the bucket and redistribute records
    // This is a simplified version of the splitting mechanism
    Bucket *oldBucket = directory[index];
    Bucket *newBucket = new Bucket{std::list<T>(), oldBucket->localDepth + 1};

    // Double the directory size
    if (oldBucket->localDepth == globalDepth)
    {
        directory.push_back(nullptr);
        globalDepth++;
    }

    int mask = (1 << oldBucket->localDepth);
    for (int i = 0; i < (1 << globalDepth); ++i)
    {
        if ((i & mask) == 0)
        {
            directory[i] = oldBucket;
        }
        else
        {
            directory[i] = newBucket;
        }
    }

    oldBucket->localDepth++;
}
#endif // EXTENDIBLE_HASHING_H

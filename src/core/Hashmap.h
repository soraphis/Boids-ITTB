//
// Created by oliver on 15.06.17.
//

#ifndef TBBTEST_HASHMAP_H
#define TBBTEST_HASHMAP_H

#include <tuple>
#include <unordered_map>
#include <tbb/concurrent_unordered_map.h>
#include <tbb/concurrent_vector.h>

using namespace std;

typedef tuple<int, int, int> int3;

struct key_hash : public unary_function<int3, int>{
    int operator()(const int3& k) const{
        return get<0>(k) ^ get<1>(k) ^ get<2>(k);
    }
};

struct key_equal : public binary_function<int3, int3, bool>{
    bool operator()(const int3 &v0, const int3 &v1) const{
        return  get<0>(v0) == get<0>(v1) &&
                get<1>(v0) == get<1>(v1) &&
                get<2>(v0) == get<2>(v1);
    }
};


typedef tbb::concurrent_unordered_map<const int3,vector<Boid>,key_hash,key_equal> maptype;
// typedef unordered_map<const int3,tbb::concurrent_vector<Boid>,key_hash,key_equal> maptype;

#endif //TBBTEST_HASHMAP_H

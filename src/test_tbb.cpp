//
// Created by oliver on 11.05.17.
//

#include <iostream>
#include <tbb/tbb.h>
#include <tbb/parallel_reduce.h>
#include <tbb/parallel_for_each.h>
#include <tbb/blocked_range.h>
#include <chrono>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <vector>

const int arraysize = std::numeric_limits<int>::max()/10;

using namespace tbb;

void Foo(float &a){
    a++;
    a--;
    a++;
}

void ParallelApplyFoo( float a[], size_t n ) {
    tbb::parallel_for( size_t(0), n, [&]( size_t i ) {
        Foo(a[i]);
    } );
}

void SerialApplyFoo( float a[], size_t n ) {
    for( size_t i=0; i!=n; ++i )
        Foo(a[i]);
}

void accumulate_test(){
    std::vector<int> mylist = std::vector<int>();
    for(int i = 0; i < 150; ++i) mylist.push_back(1);

    int sum = 0;
    atomic<int> count(0);
    tbb::parallel_for_each(mylist.begin(), mylist.end(), [&](int &i){
        sum += i;
        count++;
    });
    printf("sum is %d (counted %d)\n", sum, count);


    typedef tbb::blocked_range<std::vector<int>::iterator> range_type;
    count = 0;
    sum = tbb::parallel_reduce(
        range_type(mylist.begin(), mylist.end()), 0,
        [&count](range_type const &r, int init) -> int{
            for(auto a = r.begin(); a != r.end(); ++a){
                init += *a;
                count++;
            }
            return init;
        },
        [](int x, int y)->int{
            return x;
        }
    );
    printf("sum is %d (counted %d)\n", sum, count);

}

void my_test(){
    std::vector<int> mylist = std::vector<int>();
    for(int i = 0; i < 150; ++i) mylist.push_back(1);


    typedef tbb::blocked_range<std::vector<int>::iterator> range_type;
    tbb::parallel_for(range_type(mylist.begin(), ++mylist.begin()), [&]{

    });

}


int main ()
{
    typedef std::chrono::high_resolution_clock Time;
    typedef std::chrono::milliseconds ms;
    typedef std::chrono::duration<float> fsec;

    //std::cout << "TBB version: " << TBB_VERSION_MAJOR
    //          << "." << TBB_VERSION_MINOR << std::endl;

//    printf("doing %d cycles: \n", arraysize);
//
//    long time1, time2;
//
//    float* a = new float[arraysize];
//    for(int i = 0; i < arraysize; ++i){
//        a[i] = i;
//    }
//    // clock_gettime(CLOCK_REALTIME, &ts);
//    auto ts = Time::now();
//    SerialApplyFoo(a, (size_t) arraysize);
//    auto ts1 = Time::now();
//
//    assert(a[0] == 1);
//    assert(a[arraysize-1] == arraysize);
//
//    ms delta1 = std::chrono::duration_cast<ms>(ts1 - ts);
//    printf("%5.li \t 100.00%% \n", delta1);
//
//    ts = Time::now();
//    ParallelApplyFoo(a, (size_t) arraysize);
//    ts1 = Time::now();
//
//    assert(a[0] == 2);
//    assert(a[arraysize-1] == arraysize+1);
//
//    ms delta2 = std::chrono::duration_cast<ms>(ts1 - ts);
//
//    double time_percent = (double)delta2.count()/delta1.count();
//    printf("%5.li \t %.2f%% \n", delta2, 100.00d * time_percent);
//
//    delete(a);

    accumulate_test();

    return 0;

}

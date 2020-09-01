#include "make_heap.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <random>
#include <iostream>

TEST(buildHeap, heapify_array){
     
    int arr[] = {0,1,2,3,4,5,6};
    const int arr_len = sizeof(arr)/sizeof(arr[0]);
    ASSERT_FALSE( std::is_heap(arr, arr + arr_len) );

    HeapTool().recusiveBuildHeap(arr,arr_len );

    for(int x : arr){
        std::cout << x <<", "; 
    }
    std::cout<<"\n";
    ASSERT_TRUE( std::is_heap(arr, arr + arr_len) );
};
TEST(buildHeap, buildHeapByAdjust){
     
    int arr[] = {0,1,2,3,4,5,6};
    const int arr_len = sizeof(arr)/sizeof(arr[0]);
    ASSERT_FALSE( std::is_heap(arr, arr + arr_len) );

    HeapTool().buildHeapByAdjust(arr,arr_len );

    for(int x : arr){
        std::cout << x <<", "; 
    }
    std::cout<<"\n";
    ASSERT_TRUE( std::is_heap(arr, arr + arr_len) );
};

TEST(buildHeap, randombuildHeap){


    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> len_distr(0, 50);
    std::uniform_int_distribution<> num_distr(0, 100);


    for(size_t i = 0; i < 100000; i++) {
        int len = len_distr(gen);
        int* arr = (int*)malloc(len*sizeof(int));
        int* arr_bak = (int*)malloc(len*sizeof(int));
        for(int j = 0; j < len; j++) {
            int n = num_distr(gen);
            arr[j] = n;
            arr_bak[j] = n;
        }
        if((i&1)== 1) HeapTool().buildHeapByAdjust(arr, len);
        else HeapTool().recusiveBuildHeap(arr, len);

        bool is_heap = std::is_heap(arr, arr + len);
        if(is_heap == false ) {
            HeapTool ht;
            ht.debug_output = true;
            ht.buildHeapByAdjust(arr_bak, len);
        }
        ASSERT_TRUE( std::is_heap(arr, arr + len) );
        free(arr);
        free(arr_bak);
    }
   
};





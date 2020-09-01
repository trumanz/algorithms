#include "make_heap.h"
#include "gtest/gtest.h"
#include <algorithm>


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

#include "gtest/gtest.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <stack>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "RandomData.h"
using namespace std;
class HalfSameNum{
public:
    int find(vector<int> n) {
         if(n.size() < 3) return -1;
         // middle is middle or left of middle
         int middle = (n.size() - 1)/2;
         return find(n, 0, n.size() -1, middle);
    }

    int find(vector<int>& n, int L, int R, int middle) {
           if(L == R) {
                assert(L == middle);
                return n[L];
           }
           int pivot = paritation(n, L, R);

           if(pivot > middle) {
                return find(n, L, pivot -1 , middle);
           } 
           else if(pivot < middle) {
                while( n[pivot] == n[pivot+1] ) {
                     ++pivot;
                     if(pivot == middle ) return n[pivot];
                }
               return find(n, pivot + 1,R, middle);
           }
           else return n[pivot];
    }

    int paritation(vector<int>& n, int L, int R) {
         int pivot = n[R];
         int lower_bound_of_pivot = L; 
         for(int i = L; i <= R-1; i++) {
               if( n[i] < pivot) {
                    std::swap(n[lower_bound_of_pivot], n[i]);
                    lower_bound_of_pivot++;
               }
         }
         std::swap(n[lower_bound_of_pivot], n[R]);
         return lower_bound_of_pivot;
    }
};


TEST(HalfSameNum, test){
     std::vector<int> arr;

     arr = {1,1,2};      
     ASSERT_EQ(HalfSameNum().find(arr), 1);

     arr = {2,2,1};      
     ASSERT_EQ(HalfSameNum().find(arr), 2);

     arr = {1,1,1,1,2,4,5};      
     ASSERT_EQ(HalfSameNum().find(arr), 1);

     arr = {6,6,6,6,2,4,5};      
     ASSERT_EQ(HalfSameNum().find(arr), 6);

     arr = {1,2,2,2,2, 3,4};
     ASSERT_EQ(HalfSameNum().find(arr), 2);

     arr = {6 ,2,2,2,2, 1,4};
     ASSERT_EQ(HalfSameNum().find(arr), 2);

     arr = {6 ,2,2,2,22,2,2,2,2,2,2,2, 1,4};
     ASSERT_EQ(HalfSameNum().find(arr), 2);

     arr = std::vector<int>(30*1024, 1);
     ASSERT_EQ(HalfSameNum().find(arr), 1);


};

 
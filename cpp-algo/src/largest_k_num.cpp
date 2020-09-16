#include "gtest/gtest.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "RandomData.h"

class LargestKNum{
public:
    int swap_count;
    bool debug;
    int num_len;
    int* num_;
    LargestKNum(bool _debug = false){
         this->swap_count = 0;
         this->debug  = _debug;
    }
    void qsort(int* num, int len) {
         if(num == NULL || len <= 0) return; 
         this->num_len = len;
         this->num_ = num;
         debugPrint(NULL);
         qsort_imp(num, 0, len-1);
   }
// K is from 0
   int largestK(int* num, int len ,int k) {
        if(num  == NULL || len <= 0 || k < 0 || k > len) return -1;
        largestK_imp(num, 0, len - 1, k);
        return num[k];
   }
   
private:
   void debugPrint(const int *pi) {
         if(debug == false) return;
         for(int i = 0; i < num_len; i++) {
              printf("%d%s ,", num_[i], (pi && i == *pi) ? "^" : "");
          }
          printf("\n");
   }
   void largestK_imp(int* num, int L, int R, int K) {
        if(L < R) {
             int i = partition(num, L, R);
             if(debug) printf("i=%d, K=%d\n", i, K);
             if(i > K) {
                  largestK_imp(num, L, i-1, K);
             } else if(i < K) {
                  largestK_imp(num, i+1, R, K);
             } else {
                  return;
             }
             //if(debug) printf("i=%d, K=%d, k1=%d\n", i,K, k1);
             
        }
   }
   void qsort_imp(int* num, int L, int R) {
        if(L < R) {
             int i = partition(num, L, R);
             debugPrint(&i); 
             qsort_imp(num, L, i-1);
             qsort_imp(num, i+1, R);
        }
   }
   int partition(int* num, int L, int R) {
        if(L > R) return -1;
        if(debug) printf("L=%d, R=%d, num[L]=%d, num[R]=%d\n",L, R, num[L], num[R]);
        int pivot = num[R];
        int i = L-1; //i+1 is the lower bound of pivot
        //TODO too much swap 
        for(int j = L; j <= R-1; j++) {
          if(num[j] < pivot) {
               i++;
               if(i != j) swap(num[i], num[j]);
          }
        }
        //now i+1 is the first num equal or greter than pivot
        if(  i+1 != R)  swap(num[i+1], num[R]);
        return i+1;
   }
   template<typename T>
   void swap(T& a, T&b){
        ++swap_count;
        T c(a);
        a = b;
        b = c;
   }

};


TEST(LargestKNum, qsort_random_test){

     
    std::random_device rd; 
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> len_distr(0, 50);
    std::uniform_int_distribution<> num_distr(0, 100);


    for(size_t i = 0; i < 100000; i++) {
        int len = len_distr(gen);
        int* arr = (int*)malloc(len*sizeof(int));
        int* arr2 =  (int*)malloc(len*sizeof(int));
        for(int j = 0; j < len; j++) {
            int n = num_distr(gen);
            arr[j] = n;
            arr2[j] = n;
        }
        LargestKNum(false).qsort(arr, len);
        std::sort(arr2, arr2+len);

        for(int j = 0; j < len; j++) {
             ASSERT_EQ(arr[j], arr2[j]);
        }

        free(arr);
        free(arr2);
    }


      


};

TEST(LargestKNum, simple_test){

     int array[] = {3,6,1,3,2,0,5};
     const int len = sizeof(array)/sizeof(array[0]);
     const int k =3; //k is from 0
     int x = LargestKNum(false).largestK(array, len, k);
     ASSERT_EQ(x, 3); 
};

TEST(LargestKNum, random_test){
     RandomData rd;
     for(size_t i = 0 ; i < 10000; i++) {
          const std::vector<int> x =  rd.genRandomArray(1,50, 0,100);
          int* arr = rd.toCArr(x);
          int k = rd.genRandomInt(0, x.size()-1);
          int result_k_num = LargestKNum(false).largestK(arr, x.size(), k);
          auto y = x;
          std::sort(y.begin(), y.end());
          if(result_k_num != y[k]) {
               rd.printArr(x);
               printf("k=%d\n", k);
          }
          ASSERT_EQ(result_k_num, y[k]); 
          free(arr);
     }
     
};

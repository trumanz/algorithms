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

class LargestKNum{
public:
    uint64_t swap_count;
    bool debug;
    std::string debug_str;
    int num_len;
    int* num_;
    LargestKNum(bool _debug = false){
         this->swap_count = 0;
         this->debug  = _debug;
    }
    void qsort(std::vector<int>& arr) {
         qsort(&arr[0], arr.size());
    }
    void qsort(int* num, int len) {
         if(num == NULL || len <= 0) return; 
         this->num_len = len;
         this->num_ = num;
         debugPrint(NULL);
         qsort_imp(num, 0, len-1);
   }
   LargestKNum& qsortV2(int* num, int len) {
        if(num == NULL || len <= 0) return *this; 
        this->num_len = len;
        this->num_ = num;
        std::set<int> pis;
        if(this->debug) this->debug_str += this->genDebugStr(pis);
        std::stack<std::pair<int, int> >  partation_stack;
        int L = 0;
        int R = len -1;
        partation_stack.push(std::pair<int,int>(L,R));
        while(!partation_stack.empty()) {
             std::pair<int, int>& LR = partation_stack.top();
             int L = LR.first;
             int R = LR.second;
             int i = partitionV2(num, L, R);
             if(this->debug)  {
                  pis.insert(i);
                  this->debug_str += "\n";
                  this->debug_str += this->genDebugStr(pis) ;
             }
             partation_stack.pop();
             if(i > L+1) partation_stack.push(std::pair<int,int>(L, i-1));
             if(i+1 < R) partation_stack.push(std::pair<int, int>(i+1, R));
        }
        return *this;

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
   std::string genDebugStr(const std::set<int>& pis) {
        std::stringstream ss;
        for(int i = 0; i < num_len; i++) {
              ss << num_[i] <<  ( pis.count(i) ? "^" : "")  << " ,";
        }
        return ss.str();
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
        //if(debug) printf("L=%d, R=%d, num[L]=%d, num[R]=%d\n",L, R, num[L], num[R]);
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
      int partitionV2(int* num, int L, int R) {
        if(L > R) return -1;
        //if(debug) printf("L=%d, R=%d, num[L]=%d, num[R]=%d\n",L, R, num[L], num[R]);
        int pivot = num[R];
        int h = R-1;
        //after loop, left of L are all less than pivot
        while(L <= h) {
             while(num[L] < pivot) L++;
             while(num[h] >= pivot) {
                  if(h >= L) h--;
                  else break;
             }
             if(L < h) {
                  swap(num[L++], num[h--]);
             }
             
        }
        //now i+1 is the first num equal or greter than pivot
        if(  L != R)  swap(num[L], num[R]);
        return L;
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
    RandomData rd;
    for(size_t i = 0; i < 100000; i++) {
        const std::vector<int> arr =  rd.genRandomArray(0,50, 0,100);
        std::vector<int> arr_4_qsort =  arr;
        std::vector<int> arr_4_qsort_v2 =  arr;
        std::vector<int> arr_4_std_sort = arr;

        LargestKNum(false).qsort(arr_4_qsort);
        LargestKNum(false).qsortV2(&arr_4_qsort_v2[0], arr_4_qsort_v2.size());
        std::sort(arr_4_std_sort.begin(), arr_4_std_sort.end());

        for(size_t j = 0; j < arr.size(); j++) {
             ASSERT_EQ(arr_4_qsort[j], arr_4_std_sort[j]);
             std::vector<int> debug_arr = arr;
             ASSERT_EQ(arr_4_qsort_v2[j], arr_4_std_sort[j]) <<  LargestKNum(true).qsortV2(&debug_arr[0], debug_arr.size()).debug_str;
        }
 
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
 

class PerfTest : public ::testing::Test {
protected:
   //Time used by SetUpTestCase would not accumulated to TEST_F
   static std::vector<std::vector<int> > arr_arr;
   static void SetUpTestCase()  {
        RandomData rd;
        for(size_t i = 0; i < 5000; i++) {
          std::vector<int> x1 = rd.genRandomArray(10000,20000, 0,20000) ;
          arr_arr.emplace_back(x1);
     }
   }
};

std::vector<std::vector<int> > PerfTest::arr_arr;

TEST_F(PerfTest, qsort) {
     LargestKNum lk;
     for(size_t i = 0; i < arr_arr.size(); i++) {
          std::vector<int> x1 = arr_arr[i] ;
          lk.qsort(x1);
     }
     printf("swap_count=%lu\n", lk.swap_count);
}

TEST_F(PerfTest, qsortV2) {
     LargestKNum lk;
     for(size_t i = 0; i < arr_arr.size(); i++) {
          std::vector<int> x1 = arr_arr[i] ;     
          lk.qsortV2(&x1[0], x1.size());
     }
     printf("swap_count=%lu\n", lk.swap_count);

}
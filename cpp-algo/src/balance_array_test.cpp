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
#include <stdint.h>

using namespace std;
class BalanceArray{
     vector<uint32_t> n;
     uint32_t n_sum;

     uint32_t best_half_gap;
     vector<uint32_t> half_n;

public:
     vector<uint32_t> balance(vector<uint32_t> n) {
          this->n_sum = 0;
          for(size_t i = 0 ; i < n.size(); i++) {
               if(INT32_MAX - n_sum  < n[i]) {
                    return vector<uint32_t>();
               }
               n_sum += n[i];
          }
          this->n = n;
          this->best_half_gap = INT32_MAX;
          this->half_n.clear();
          vector<uint32_t> half_arr;
          //balanceRecursiveImp(0,0, half_arr);
          balanceDynamicProgramming();
          return half_n;
     } 


     void balanceRecursiveImp(size_t i, uint32_t cur_sum, vector<uint32_t>& half_arr)
     {
          if(i >= n.size()) return;
          //include n[i]
          const uint32_t sum = cur_sum + n[i];
          uint32_t gap = n_sum - sum; //gap = | n_sum - 2*cur_sum |
          gap = gap > sum ?  (gap - sum) : ( sum -  gap);
          //std::cout  << "arr:" << join_str(half_arr.begin(), half_arr.end()) << "," << n[i] <<"\n";
          //std::cout << "sum:" << sum << ",gap:" << gap  <<"\n";
          half_arr.push_back(n[i]);
          if(gap < best_half_gap) {
               
               best_half_gap = gap;
               half_n = half_arr;
               if(gap == 0) return;
              
          }
          balanceRecursiveImp(i+1, sum, half_arr);
          half_arr.pop_back();
          //exclude n[i]
          balanceRecursiveImp(i+1, cur_sum, half_arr);
     }
     
     /*
     define bool dp[i][j],  represent possibility  of a sub arr end with index i and it's sum is j;
     dp[i][j] = dp[i-1][j-n[i]] ...or... dp[0][j-n[i]]      when  j-n[i] > 0 and i -1 >=0;


     so, when  dp[i][j] is true;
     dp[i+1][j + n[i+1]],   dp[i+2][j + n[i+2]] ... dp[i+x][j + n[i+x]]  are all true
    */
      void  balanceDynamicProgramming() { 
          if(n.size() == 0) return;
          if(n.size() == 1) {
               this->half_n.push_back(n[0]) ;
               return;
          } 
          const uint32_t half_sum = n_sum/2;
          if(half_sum < 1) return;

          vector< vector<bool> > dp(n.size(), vector<bool>(half_sum, false));

          for(size_t i = 0; i < n.size(); i++) {
               uint32_t sum = n[i];
               if(sum < half_sum ) dp[i][sum] = true;
               else continue;
               for(size_t  x = 1;  x + sum < half_sum ; x++) {
                     for(size_t k = 0; k + 1 < i ; k++) {
                         if( dp[k][x] )  dp[i][ x+sum ] = true;
                         break;
                    }
               }
          }

          for(size_t j = half_sum ; j >0; j--) {
               for(size_t i = 0; i < n.size(); i++){
                    if(dp[i][j-1]) {
                         printf("half sum : %lu\n",j-1);
                         break;
                    }
               }
          }
     }
      
     
};
#if 0
TEST(BalanceArray_test, simple_test){
     RandomData rd;
     vector< vector<uint32_t> > arr2 = {
          {}, {},
          {1}, {1},
          {1,2}, {1},
          {2,1}, {2},
          {1,2,3}, {1,2},
          {3,2,1}, {3},
          {1,3,9}, {1,3},
          {1,5,3}, {1,3},
          {5,1,3}, {5},
          {1,2,3,2}, {1,3},
          {1,9,8,100,100}, {1,8,100}

     };
     for(size_t i = 0 ; i + 1 < arr2.size(); i+=2) {
          //std::cout << "balance: " << join_str(arr2[i].begin(), arr2[i].end()) <<"\n";
          auto ha = BalanceArray().balance(arr2[i]);
          EXPECT_EQ(ha, arr2[i+1]) << i;
     }
 
};
 

TEST(BalanceArray_test, random_test){
     RandomData rd;
     for(size_t i = 0 ; i < 10; i++) {
          const std::vector<int> x =  rd.genRandomArray(5,10, 1,10);
          vector<uint32_t> random_arr(x.begin(), x.end());
          BalanceArray ba;
          vector<uint32_t> half_arr = ba.balance(random_arr);
          
     }
};
#endif
 
 
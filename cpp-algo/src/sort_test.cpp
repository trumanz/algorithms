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
#include <chrono>
#include <boost/sort/sort.hpp>
#include "CppConsoleTable.hpp"

using namespace std;
using namespace samilton;
class SortPerf : public ::testing::Test {
protected:
   //Time used by SetUpTestCase would not accumulated to TEST_F
   static std::vector<std::vector<IntType4Test> > arr_arr;
   static void SetUpTestCase()  {
        RandomData rd;
        for(size_t i = 0; i <= 5; i++) {
         const int num_1K = 1024;
         const int num_1M = num_1K*num_1K;
         const int arr_len = (1<<(i*3))*num_1K; //1k, 8k, 64k ...;
         const std::vector<int> arr =  rd.genRandomArray(arr_len,arr_len, 0,arr_len);
         arr_arr.emplace_back(arr.begin(), arr.end());
          
     }
   }
};

std::vector<std::vector<IntType4Test> > SortPerf::arr_arr;



template<typename T>
std::string toHumanStr(const T& d)
{
     int64_t n = static_cast<uint64_t>(d);
     char buf[100];
     if(n > 1024*1024) {
          snprintf( buf, sizeof(buf), "%.1fM", n/(1024.0*1024.0));
     } else if(n > 1024) {
          snprintf(buf, sizeof(buf), "%.1fK",n/(1024.0));
     } else {
          snprintf(buf, sizeof(buf), "%ld", n);
     }
     return buf;
} 

std::vector<std::string> getIntType4TestCount(size_t arr_size)
{
     std::vector<std::string> row;
     assert(arr_size > 0);
     row.push_back( toHumanStr(arr_size) );
     int64_t count;
     count = IntType4Test::less_operator_count;
     row.push_back(toHumanStr(count));
     row.push_back(toHumanStr(count/arr_size));

     count = IntType4Test::constructor_count;
     row.push_back(toHumanStr(count));
     row.push_back(toHumanStr(count/arr_size));

     count = IntType4Test::copy_assignment_count;
     row.push_back(toHumanStr(count));
     row.push_back(toHumanStr(count/arr_size));

     return row;
}


TEST_F(SortPerf, sort_rand_and_sorted_data){

    ConsoleTable   table;
    std::vector<std::string> header= {"size",
        "less", "times",
         "constr", "times",
         "copy", "times",  "ms"};
    
    table.addRow(header);
    for(std::vector<IntType4Test>& arr : SortPerf::arr_arr) {
         std::vector<IntType4Test> arr2 = arr;
         
         IntType4Test::initCount();
         auto start = std::chrono::high_resolution_clock::now();
         std::stable_sort(arr2.begin(), arr2.end());
         auto end = std::chrono::high_resolution_clock::now();
         auto row = getIntType4TestCount(arr2.size());
         int64_t  micro_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
         row.push_back(std::to_string( micro_seconds));
         table.addRow(row);
    }

     std::cout << table << "\n";
};


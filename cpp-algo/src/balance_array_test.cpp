#include "gtest/gtest.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <stack>
#include <set>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "utility/RandomData.h"
#include <stdint.h>

using namespace std;

class ArrayBalancer {
public:
    virtual vector<uint32_t> solve(const vector<uint32_t> &num_arr) = 0;

protected:
    uint32_t sum(const vector<uint32_t> &num_arr) {
        uint32_t sum = 0;
        for (auto n : num_arr) {
            if (INT32_MAX - sum < n) {
                throw runtime_error("sum overflow");
            }
            sum += n;
        }
        return sum;
    }

};
#if 0
//TODO
https://zhuanlan.zhihu.com/p/30959069  knapsack problem ,solve
class DPBalanceArray : public ArrayBalancer {
    const vector<uint32_t> *p_num_arr;
    uint32_t n_sum;
    uint32_t best_half_gap;
    vector<uint32_t> final_solution;

public:
    vector<uint32_t> solve(const vector<uint32_t> &num_arr) {
        this->n_sum = this->sum(num_arr);
        this->p_num_arr = &num_arr;
        this->best_half_gap = INT32_MAX;
        this->final_solution.clear();
        dynamicProgrammingSolve();
        return final_solution;
    }
    /*
    define bool dp[i][j],  represent possibility of "a sub arr end with index i" and it's sum is j;
    dp[i][j] = dp[i-1][j-n[i]] or dp[i-2][j-n[i]], or ... or  dp[0][j-n[i]]      when  j-n[i] > 0 and i -1 >=0;

    so, when  dp[i][j] is true;
    dp[i+1][j + n[i+1]],   dp[i+2][j + n[i+2]] ... dp[i+x][j + n[i+x]]  are all true
   */
    void dynamicProgrammingSolve() {
        if (p_num_arr->size() == 0) return;
        if (p_num_arr->size() == 1) {
            this->final_solution.push_back(p_num_arr->at(0));
            return;
        }
        const uint32_t half_sum = n_sum / 2;
        if (half_sum < 1) return;

        vector<vector<bool> > dp(p_num_arr->size(), vector<bool>(half_sum, false));

        uint32_t biggest_sum = 0;
        for (size_t i = 0; i < p_num_arr->size(); i++) {
            uint32_t possible_sum = p_num_arr->at(i);
            if(possible_sum > half_sum) continue;
            bool need_broadcast = !dp[i][possible_sum] ;
            dp[i][possible_sum] = true;
            if(!need_broadcast) continue;


            for (size_t x = 1; x + possible_sum < half_sum; x++) {
                for (size_t k = 0; k + 1 < i; k++) {
                    if (dp[k][x]) dp[i][x + sum] = true;
                    break;
                }
            }
        }

        for (size_t j = half_sum; j > 0; j--) {
            for (size_t i = 0; i < p_num_arr->size(); i++) {
                if (dp[i][j - 1]) {
                    printf("half sum : %lu\n", j - 1);
                    break;
                }
            }
        }
    }


};

#endif
class RecursiveArrayBalancer : public ArrayBalancer {
    const vector<uint32_t> *p_num_arr;
    uint32_t n_sum;
    uint32_t best_half_gap;
    vector<uint32_t> final_solution;

public:
    vector<uint32_t> solve(const vector<uint32_t> &num_arr) {
        this->n_sum = this->sum(num_arr);
        this->p_num_arr = &num_arr;
        this->best_half_gap = INT32_MAX;
        this->final_solution.clear();
        vector<uint32_t> cur_solution;
        solveRecursiveImp(0, 0, cur_solution);
        return final_solution;
    }

private:
    void solveRecursiveImp(size_t i, uint32_t cur_sum, vector<uint32_t> &cur_solution) {
        if (i >= p_num_arr->size() || best_half_gap == 0) return;
        //include n[i]
        const uint32_t sum = cur_sum + p_num_arr->at(i);
        uint32_t gap = n_sum - sum; //gap = | n_sum - 2*cur_sum |
        gap = gap > sum ? (gap - sum) : (sum - gap);
        //std::cout  << "arr:" << join_str(half_arr.begin(), half_arr.end()) << "," << n[i] <<"\n";
        //std::cout << "sum:" << sum << ",gap:" << gap  <<"\n";
        cur_solution.push_back(p_num_arr->at(i));
        if (gap < best_half_gap) {
            best_half_gap = gap;
            final_solution = cur_solution;
            if (gap == 0) return;
        }
        solveRecursiveImp(i + 1, sum, cur_solution);
        cur_solution.pop_back();
        solveRecursiveImp(i + 1, cur_sum, cur_solution);
    }
};

class BalanceArrayDataUtil {
public:
    struct Data {
        vector<uint32_t> num_arr;
        vector<uint32_t> expected_half_arr;
        std::string str() {
            std::stringstream  ss;
            for(auto n : num_arr) ss << n <<", ";
            return ss.str();
        }
    };

    vector<Data> genDataV1() {
        vector<Data> data {
                {{}, {}},
                {{1}, {1}},
                     {{1, 2}, {1}},
                        {{2, 1}, {2}},
                        {{1, 2, 3}, {1, 2}},
                        {{3, 2, 1}, {3}},
                        {{1, 3, 9}, {1, 3}},
                        {{1, 5, 3}, {1, 3}},
                        {{5, 1, 3}, {5}},
                        {{1, 2, 3, 2}, {1, 3}},
                        {{1, 9, 8, 100, 100}, {1, 8, 100}}
        };
        return data;
    };
};
#if 1

TEST(BalanceArray_test, resursive_solver) {
    auto test_data = BalanceArrayDataUtil().genDataV1();
    for (auto& data : test_data) {
        std::unique_ptr<ArrayBalancer> balancer(new RecursiveArrayBalancer());
        auto ha = balancer->solve(data.num_arr);
        EXPECT_EQ(ha, data.expected_half_arr) << "\ndata "  << data.str();
    }
};
#if 0
TEST(BalanceArray_test, dp_solver) {
    auto test_data = BalanceArrayDataUtil().genDataV1();
    for (auto& data : test_data) {
        std::unique_ptr<ArrayBalancer> balancer(new DPBalanceArray());
        auto ha = balancer->solve(data.num_arr);
        EXPECT_EQ(ha, data.expected_half_arr) << "\ndata "  << data.str();
    }
};
#endif
#endif
#if 0
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
 
 
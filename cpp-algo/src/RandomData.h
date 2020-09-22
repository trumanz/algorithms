#ifndef __RANDOM_DATA_H_
#define __RANDOM_DATA_H_
#include <algorithm>
#include <random>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <atomic> 
#include <assert.h>

class IntType4Test {
public:
static std::atomic<int64_t>  constructor_count;
static std::atomic<int64_t>  copy_assignment_count;
static std::atomic<int64_t>  less_operator_count;

public:
    int value;
public:
    IntType4Test(int v) {
        this->value = v;
        constructor_count++;
        //uint64_t c = constructor_count;
        //printf("%d, %lu\n", v, c);
    }
    IntType4Test(const IntType4Test& oth)  : IntType4Test(oth.value){
    }
    IntType4Test& operator=(const IntType4Test& oth) {
        if(this != &oth) {
            copy_assignment_count++;
            this->value = oth.value;
        }
        return *this;
    }
    bool operator<(const IntType4Test& oth) const{
        less_operator_count++;
        return this->value < oth.value;
    }

public:
    static void initCount();
    static std::string getCountInfo();
    
};

class RandomData{
private:
     
     std::mt19937_64 gen;
public:
     RandomData(unsigned seed = 0){
         gen = std::mt19937_64(seed);
         //gen = std::minstd_rand(seed);
     }
     int genRandomInt(int min, int max) {
         return std::uniform_int_distribution<>(min, max)(gen);
     }
     int* toCArr(const std::vector<int>& arr) {
         int* x = (int*) malloc(arr.size() * sizeof(int));
         for(size_t i = 0; i <arr.size(); i++) {
             x[i] = arr[i];
         }
         return x;
     }

     std::vector<int> genRandomArray(int arr_len_lb, int arr_len_ub, 
                                     int min_element, int max_element) {
         assert(arr_len_lb <= arr_len_ub);
         int len = arr_len_ub == arr_len_ub ? arr_len_ub: std::uniform_int_distribution<>(arr_len_lb, arr_len_ub)(gen);
         std::vector<int> arr(len, 0);
         for(int i = 0 ; i < len ; i++) {
             int element = std::uniform_int_distribution<>(min_element, max_element)(gen);
             arr[i] = element;
         }
         return arr;
     }
     void printArr(std::vector<int> arr) {
         for(size_t i = 0; i < arr.size(); i++) {
             printf("%d ,", arr[i]);
         }
         printf("\n");
     }
};

#endif
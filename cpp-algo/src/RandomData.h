#ifndef __RANDOM_DATA_H_
#define __RANDOM_DATA_H_
#include <algorithm>
#include <random>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

class RandomData{
private:
     std::random_device rd;
     std::mt19937 gen;
public:
     RandomData(){
         gen = std::mt19937(rd());
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

     std::vector<int> genRandomArray(int min_size, int max_size, 
                                     int min_element, int max_element) {
         int len = std::uniform_int_distribution<>(min_size, max_size)(gen);
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
#include "make_heap.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


class TxtNumCompare{
public:
    int compare(std::string num1, std::string num2)const {
        const char* n1 = num1.c_str();
        const char* n2 = num2.c_str();

        //printf("%s, %s\n", n1, n2);

        int rc = compare4Integer(n1, n2);
        if (rc != 0 ) return rc;

        while (isdigit(*n1)) ++n1 ;
        while (isdigit(*n2)) ++n2 ;
     
        if (*n1 == '.') ++n1;
        if (*n2 == '.') ++n2;

        //printf("[%s], [%s]\n", n1, n2);
        rc = lessThan4Fractional(n1,n2);
        return rc;

     }

    int lessThan4Fractional(const char*& num1, const char*& num2) const{
        while( isdigit(*num1) && isdigit(*num2) ) {
            if(*num1 < *num2) return -1;
            if(*num1 > *num2) return 1;
            num1++;
            num2++;
        }
        while (isdigit(*num1)) {
            if(*num1 > '0') return 1;
            ++num1;
        }
        while (isdigit(*num2)) {
            if(*num2 > '0') return -1;
            ++num2;
        }
        return 0;
    }
 
    // -1 lower  
    // 0 equal
    // 1 greater
    int compare4Integer(const char*& num1, const char*& num2) const{
        int rc = 0;
        while (*num1 == '0') num1++;
        while (*num2 == '0') num2++;

        while (isdigit(*num1) && isdigit(*num2)) {
            if(*num1 < *num2)  {
                rc = -1;
                break;
            }
            if(*num1 > *num2)  {
                rc = 1;
                break;
            }
            num1++;
            num2++;
        }
        while(isdigit(*num1) && isdigit(*num2))  {
            num1++;
            num2++;
        }

        if ( isdigit(*num1) ) return 1;
        if ( isdigit(*num2) ) return -1;
        return rc;

    }
};



TEST(TxtNumCompare, compare){
     
     TxtNumCompare compare;


     ASSERT_EQ(compare.compare(".", "."), 0);
     ASSERT_EQ(compare.compare(".", ".0"), 0);
     ASSERT_EQ(compare.compare(".", "0.0"), 0);
     ASSERT_EQ(compare.compare(".00", "0.000"), 0);


     ASSERT_EQ(compare.compare(".28", ""), 1);

     ASSERT_EQ(compare.compare("1135", "1234"), -1);
     ASSERT_EQ(compare.compare("1234", "123"), 1);

     ASSERT_EQ(compare.compare("1.2", "1.3"), -1);
     ASSERT_EQ(compare.compare("0.00", "0.000"), 0);

     ASSERT_EQ(compare.compare("123.00", "123.000"), 0);



};


class RandomNumGen{
public:
     std::default_random_engine generator;
  
    RandomNumGen(){
     }

    std::string genRandomString() {
       
        int dot = std::uniform_int_distribution<int>(0,1)(generator); 
        int int_len = std::uniform_int_distribution<int>(0,5)(generator);
        int frac_len = std::uniform_int_distribution<int>(0,5)(generator);
        if (frac_len> 0) dot = 1;

        std::string ns;
        for (int i = 0; i < int_len; i++) {
            int x = std::uniform_int_distribution<>(0,9)(generator);
            ns +=  '0' + x;
           
        }
        if (dot == 1) ns+= '.';
        for (int i = 0; i < frac_len; i++) {
            ns +=  '0' + std::uniform_int_distribution<>(0,9)(generator);
        }
        return ns;
    }

};

TEST(TxtNumCompare, randomTxtNum){
     

    TxtNumCompare compare;

    RandomNumGen gen;
     
     for (size_t i = 0; i < 100*1000; i++) {
        std::string n1 = gen.genRandomString();
        std::string n2 = gen.genRandomString();
 
        double d1 = 0;
        double d2 = 0;
        

        sscanf( n1.c_str(), "%lf",  &d1);
        sscanf( n2.c_str(), "%lf",  &d2);

        int expected_rc = 0;
        if (d1 < d2 ) expected_rc =  -1;
        if (d1 > d2 ) expected_rc =  1;

        //printf("===%lf, %lf, expected_rc=%d, d1<d2=%s\n", d1, d2, expected_rc, (d1<d2 ? "true": "false"));

         
        ASSERT_EQ(compare.compare(n1, n2), expected_rc);

    }


};

//
// Created by Zhou, Changkai on 2021/5/18.
//
#include "gtest/gtest.h"
#include "boost/date_time/posix_time/posix_time.hpp"
class Task{
public:
    virtual void handle() = 0;
};
class MaxFreqCotroller {
public:
    virtual bool handle(Task* task) = 0;
};

class CircularTokenMaxFreqController : public MaxFreqCotroller{
    const uint32_t freq_per_seconds;
    uint32_t free_token = 0;
    std::vector<uint32_t> token_used_per_millisecond;
    size_t circular_curr_index = 0;
    std::chrono::time_point<std::chrono::steady_clock> last_time;

public:
    CircularTokenMaxFreqController(const uint32_t _freq_per_seconds) : freq_per_seconds(_freq_per_seconds){
        token_used_per_millisecond.resize(1000, 0);
        //assume token already used in last second
        for(size_t i = 0; i < _freq_per_seconds; i++) {
            ++token_used_per_millisecond[i%1000];
        }
        free_token = 0;
        circular_curr_index = 0;
        last_time = std::chrono::steady_clock::now();
    }
    bool handle(Task* task) override {
        const auto now = std::chrono::steady_clock::now();
        const auto dur = now - last_time;
        long long int elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();


        if(elapsed_milliseconds > 1000) elapsed_milliseconds = 1000;
        if(elapsed_milliseconds >= 1)  {
            last_time = now;

           for(long long int i = 0 ; i < elapsed_milliseconds ; i++) {
               free_token += token_used_per_millisecond[circular_curr_index];
              token_used_per_millisecond[circular_curr_index] = 0;
              circular_curr_index = ( circular_curr_index + 1 ) % 1000;
           }
           // std::cout  << boost::posix_time::microsec_clock::local_time()
           //  << ", free_token:"  << free_token
           //  << ", elapsed_milliseconds:" <<  elapsed_milliseconds << "\n";
        }

        if(free_token > 0) {
            ++token_used_per_millisecond[(circular_curr_index+999)%1000];
            --free_token;
          // if(free_token == 0) {
          //     std::cout  << boost::posix_time::microsec_clock::local_time() << ", free_token:"  << free_token << "\n";
          // }
            task->handle();
            return true;
        }
        return false;
    }
};

class VirtTask : public Task {
    size_t count = 0;
public:
    virtual void handle() override {
        count++;
    }

};

TEST(MaxFreqCotroller, CircularTokenMaxFreqController){

    CircularTokenMaxFreqController freq_controller(1000);
    VirtTask task;
    size_t success_count = 0;

    const auto start = std::chrono::steady_clock::now();


    while(true) {
        bool rc = freq_controller.handle(&task);
        if(rc) {
            success_count++;
            //std::cout << "sucess_count " << success_count <<"\n";
            if(success_count > 2000)break;
        }
    }
    auto dur = std::chrono::steady_clock::now() - start;
    long long int elapsed_milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();

    GTEST_ASSERT_GT(elapsed_milliseconds, 1980);
    GTEST_ASSERT_LT(elapsed_milliseconds, 2020);


};

#include "RandomData.h"
#include <sstream>

std::atomic<int64_t>  IntType4Test::constructor_count(0);
std::atomic<int64_t>  IntType4Test::copy_assignment_count(0);
std::atomic<int64_t>  IntType4Test::less_operator_count(0);

void IntType4Test::initCount()
{
    constructor_count = 0;
    copy_assignment_count = 0;
    less_operator_count = 0;
}

std::string IntType4Test::getCountInfo()
{
    std::stringstream ss;
    ss << "constructor_count=" << static_cast<int64_t>(constructor_count);
    ss << ",copy_assignment_count=" << static_cast<int64_t>(copy_assignment_count);
    ss << ",less_operator_count=" << static_cast<int64_t>(less_operator_count);
    return ss.str();
}


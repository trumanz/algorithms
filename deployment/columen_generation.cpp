
#include <assert.h>
#include <cstdio>
#include <cstring>  // strlen
#include <map>
#include <vector>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ortools/base/commandlineflags.h"
#include "ortools/base/commandlineflags.h"
#include "ortools/base/logging.h"
#include "ortools/base/macros.h"
#include "ortools/base/stringprintf.h"
#include "ortools/linear_solver/linear_solver.h"

/* 1  1  1  2
A  0  1  1  1   10
B  1  0  1  0   10
C  1  0  1  1   10
*/
 
using namespace operations_research;

int main(){
    std::vector<std::vector<int> >matrix2;

    matrix2.push_back({1,0,0,0});
    matrix2.push_back({0,1,0,0});
    matrix2.push_back({0,0,1,0});
    matrix2.push_back({0,0,0,1});
    matrix2.push_back({1,1,0,0});
    matrix2.push_back({0,1,1,0});
    matrix2.push_back({0,0,1,1});

    int matrix[7][4] = {
         {1,0,0,0}
        ,{0,1,0,0}
        ,{0,0,1,0}
        ,{0,0,0,1}
        ,{1,1,0,0}
        ,{0,1,1,0}
        ,{0,0,1,1}
    };
    
    MPConstraint* mp_cons[4];
    const MPVariable* mp_vars[7];

    operations_research::MPSolver::OptimizationProblemType solver_type  = operations_research::MPSolver::GLOP_LINEAR_PROGRAMMING;
    MPSolver solver("ColumnGeneration", solver_type);


    solver.SuppressOutput();
    solver.MutableObjective()->SetMinimization();

    for(size_t i =0 ; i < sizeof(mp_cons)/sizeof(mp_cons[0]); i++) {
        mp_cons[i] = solver.MakeRowConstraint( 1., 1.);
    }
    for(size_t i = 0; i < 7; i++){
        MPVariable* const a = solver.MakeNumVar(0., 1., "");
        solver.MutableObjective()->SetCoefficient(a, 10);
        mp_vars[i] = a;
        for(size_t j =0 ; j < 4; j++){
             bool cover = matrix[i][j] == 1;
             if(cover) mp_cons[j]->SetCoefficient(a, 1.0);
        }
    }

    MPSolver::ResultStatus result =  solver.Solve();
    assert(MPSolver::OPTIMAL == result);
  
    static const double Ktolerance = 1e-5;  
    printf("cost = %lf\n", solver.Objective().Value());
    for(size_t i = 0 ;i < 7;i++)
    {
           printf("%lu, %f\n", i, mp_vars[i]->solution_value());
   //       if(mp_vars[i]->solution_value() >= 1. - Ktolerance) {
   //       }
    }
    for(size_t i = 0 ;i < 4;i++)
           printf("dual %lu, %f\n", i, mp_cons[i]->dual_value());
 
  return 0;
}

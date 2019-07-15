
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


static void printMatrix(const std::vector<std::vector<int> >& matrix, const std::vector<MPConstraint*>& mp_cons, const std::vector<const MPVariable*>& mp_vars)
{
   for(size_t i = 0; i < matrix.size(); i++){
       double dual_sum = 0.0;
       for(size_t j = 0; j < matrix[i].size(); j++) {
             int x = matrix[i][j];
             printf("%10d, ", x);
       }
       if(i < 2)  printf("===");
       else printf("  %f", mp_vars[i-2]->solution_value());
       printf("\n");
   }
    for(size_t i = 0 ;i < mp_cons.size();i++)
           printf("%f, ", mp_cons[i]->dual_value());
   printf("\n");
}

int main(){
    std::vector<std::vector<int> >matrix;

    matrix.push_back({1,1,1,1});// first constraint row  lb
    matrix.push_back({1,1,1,2});// first constraint row  rb
    matrix.push_back({1,0,0,0, 100}); //from second are var  
    matrix.push_back({0,1,0,0, 100});
    matrix.push_back({0,0,1,0, 100});
    matrix.push_back({0,0,0,1, 100});
    matrix.push_back({1,1,0,0, 10});
    matrix.push_back({0,1,1,0, 10});
    matrix.push_back({0,0,1,2, 10});
  
 
    std::vector<MPConstraint*> mp_cons;
    std::vector<const MPVariable*> mp_vars;

    operations_research::MPSolver::OptimizationProblemType solver_type  = operations_research::MPSolver::GLOP_LINEAR_PROGRAMMING;
    MPSolver solver("ColumnGeneration", solver_type);


    solver.SuppressOutput();
    solver.MutableObjective()->SetMinimization();

    //constraint
    for(size_t i = 0; i < matrix[0].size(); i++) {
        double lb = matrix[0][i];
        double rb = matrix[1][i];
        mp_cons.push_back(solver.MakeRowConstraint( lb, rb));
    }
    for(size_t i = 2; i < matrix.size(); i++){
        MPVariable* const var = solver.MakeNumVar(0., 1., "");
        mp_vars.push_back(var);
        solver.MutableObjective()->SetCoefficient(var, matrix[i][matrix[0].size()]); //cost hard code 10
        for(size_t j =0 ; j < matrix[0].size(); j++){
             if(matrix[i][j] != 0) mp_cons[j]->SetCoefficient(var, 0.0 + matrix[i][j]);
        }
    }

    MPSolver::ResultStatus result =  solver.Solve();
    assert(MPSolver::OPTIMAL == result);
  
    static const double Ktolerance = 1e-5;  
    printf("cost = %lf\n", solver.Objective().Value());
    for(size_t i = 0 ;i < mp_vars.size();i++)
    {
           printf("%lu, %f\n", i, mp_vars[i]->solution_value());
   //       if(mp_vars[i]->solution_value() >= 1. - Ktolerance) {
   //       }
    }
    for(size_t i = 0 ;i < mp_cons.size();i++)
           printf("dual %lu, %f\n", i, mp_cons[i]->dual_value());
 
    printMatrix(matrix,  mp_cons, mp_vars); 
  return 0;
}

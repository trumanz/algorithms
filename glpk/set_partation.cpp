#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <set>
#include <glpk.h>

class GlpRow{
public:
   int row_index;
   std::string name;
   glp_prob* lp;
public:
   GlpRow(glp_prob* lp, const char* name){
       	this->row_index = glp_get_num_rows(lp) + 1;
        this->lp = lp;
        this->name = name;
        glp_add_rows(lp, 1);
        glp_set_row_bnds(lp, this->row_index, GLP_FX, 1.0, 1.0);
        glp_set_row_name(lp, this->row_index, name);
   } 
};

class GlpCol{
public:
   int col_index;
   std::string name;
   glp_prob* lp;
   std::vector<GlpRow*> rows;
   std::vector<int> indexs;
        std::vector<double> values;
public:
   GlpCol(glp_prob* lp, const char* name, double coef, const std::vector<GlpRow*>& rows){
       	this->col_index = glp_get_num_cols(lp) + 1;
        this->name = name;
        this->lp = lp;
        this->rows = rows;
        glp_add_cols(lp, 1);
        glp_set_col_name(lp, this->col_index, name);
        glp_set_col_bnds(lp, this->col_index, GLP_LO, 0.0, 0.0);
        glp_set_obj_coef(lp, this->col_index, coef);
        glp_set_col_kind(lp, this->col_index, GLP_IV);
      
        //std::vector<int> indexs;
        indexs.push_back(0);//glpk from index 1, not 0; so push one dummy
        //std::vector<double> values;
        values.push_back(0);
        for(std::vector<GlpRow*>::const_iterator it = rows.begin(); 
             it != rows.end(); it++) {
             indexs.push_back((*it)->row_index);
             values.push_back(1.0);
        }
        glp_set_mat_col(lp, this->col_index, rows.size(), indexs.data(), values.data());
   }
   double getPrim() {
      return  glp_get_col_prim(lp, this->col_index);
   }
   double getMipValue() {
      return  glp_mip_col_val(lp, this->col_index);
   }
   std::string dumpStr(){
         std::stringstream ss;
         for(std::vector<GlpRow*>::iterator it = rows.begin(); it != rows.end(); it++) {
              ss << (*it)->name << ",";
         }
         return ss.str();
   }
};


GlpCol createCol(glp_prob *lp, int len, std::vector<GlpRow>& rows){
     std::set<int> is;
     for(int i = 0; i < len; i++){
        is.insert( rand()%(rows.size()) ); 
     }
     std::vector<GlpRow*> rs;
     for(std::set<int>::iterator it = is.begin(); it != is.end(); it++) {
         rs.push_back(&(rows[*it]));
     }
     double coef = 100.0 - rs.size();
     GlpCol col = GlpCol(lp, "", coef, rs);
     return col;
}

int main(void)
{ 
 glp_prob *lp;
 lp = glp_create_prob();
 glp_set_prob_name(lp, "sample");
 glp_set_obj_dir(lp, GLP_MIN);

 std::vector<GlpRow>  rows;
 for(char c = 'a'; c <= 'z'; c++){
     char name[10];
     sprintf(name, "%c", c);
     GlpRow r(lp, name);
     rows.push_back(r);
 }
 std::vector<GlpCol> cols;
 for(int i =0; i < 200; i++){
    GlpCol col = createCol(lp, 3,rows);
    cols.push_back(col);
 }
// glp_std_basis(lp);
 //glp_simplex(lp, NULL);

  glp_iocp parm;
  glp_init_iocp(&parm);
  parm.presolve = GLP_ON;
  int err = glp_intopt(lp, &parm);

 //printf("\nz = %f\n", glp_get_obj_val(lp));
 printf("\nz = %f\n", glp_mip_obj_val(lp));
 std::set<std::string> row_selected;
 size_t count = 0;
 for(std::vector<GlpCol>::iterator it = cols.begin(); it != cols.end(); it++){
     if(it->getMipValue() > 0.01 || it->getMipValue() < -0.01) {
          std::cout <<  it->getMipValue() << "  ";
          std::cout << it->dumpStr() << "\n";
          std::vector<GlpRow*> rs =  it->rows;
          for(size_t i = 0; i < rs.size(); i++){
                row_selected.insert(rs[i]->name);
                count++;
          };
     }
 }
 std::cout << "selected unique=" << row_selected.size() << ", all=" << count << "\n";
// printf("\nz = %g; x1 = %g; x2 = %g; x3 = %g\n",
//       glp_get_obj_val(lp), x1c.getPrim(), x2c.getPrim(), x3c.getPrim());
 glp_delete_prob(lp);
return 0;
}

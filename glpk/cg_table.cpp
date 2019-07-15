#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <glpk.h>

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <set>

class GlpRow{
public:
   int row_index;
   std::string name;
   glp_prob* lp;
   double dual_value;
public:
   GlpRow(glp_prob* lp, const char* name, int type,double lb, double ub){
       	this->row_index = glp_get_num_rows(lp) + 1;
        this->lp = lp;
        this->name = name;
        this->dual_value = 0.0;
        glp_add_rows(lp, 1);
        glp_set_row_bnds(lp, this->row_index, type, lb, ub);
        glp_set_row_name(lp, this->row_index, name);
   } 
   void updateDualValue(){
       this->dual_value = glp_get_row_dual(lp, this->row_index);
   }
   std::string dumpStr(){
         std::stringstream ss;
         ss<< "name:" << name << ", dual_vaule: " << dual_value;
         return ss.str();
   }
};

class GlpCol{
public:
   int col_index;
   std::string name;
   glp_prob* lp;
   double coef;
   std::vector<GlpRow*> rows;
   std::vector<int> indexs;
   std::vector<double> values;
public:
   GlpCol(glp_prob* lp, const char* name, double coef, const std::vector<GlpRow*>& rows){
       	this->col_index = glp_get_num_cols(lp) + 1;
        this->name = name;
        this->lp = lp;
        this->rows = rows;
        this->coef = coef;
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
         ss << "coef:[" << this->coef << "]";
         for(std::vector<GlpRow*>::iterator it = rows.begin(); it != rows.end(); it++) {
              ss << (*it)->name << ",";
         }
         return ss.str();
   }
};

class Table{
public:
    std::vector<GlpRow*> guests;
public:
    double getDualSum() const{
       double sum = 0.0;
       for(std::vector<GlpRow*>::const_iterator it =  guests.begin(); it != guests.end(); it++) {
          sum += (*it)->dual_value;
       }
       return sum;
    }
};
bool compareTableByDualVaule(const Table& first, const Table& second) {
    return first.getDualSum()  <  second.getDualSum();
}
std::vector<GlpRow*> createRowsForGuests(glp_prob *lp){  
 std::vector<GlpRow*>  rows;
 //const char* guests= "A B C D E F G I J K L M N O P Q R";
 const char* guests= "A B C D E F G I J K L M N O P Q R S T V W Y Z";
 for(int i = 0; i < strlen(guests); i++){
     if( guests[i] >= 'A' && guests[i] <= 'Z') { 
        char name[10];
        sprintf(name, "%c", guests[i]);
        GlpRow* r = new GlpRow(lp, name, GLP_FX, 1.0, 1.0);
        rows.push_back(r);
     }
 }
 return rows;
}


void insertToRows(std::list<Table> *all_tables,
                  const std::vector<GlpRow*>& combin, 
                  const std::vector<GlpRow*>& rows, int from, 
                  int needs) {
       assert(needs >= 0);
       if(needs == 0){
            Table t;
            t.guests = combin;
            all_tables->push_back(t);
       } else {
          for( int i = from; i < rows.size(); i++) {
             std::vector<GlpRow*> cb = combin;
             cb.push_back(rows[i]);
             insertToRows(all_tables, cb, rows, i+1, needs-1);
           }
       }
}
std::list<Table> createTables(const std::vector<GlpRow*>& rows, int min_len, int max_len){
   std::list<Table> all_tables;
   for(int  len = min_len; len <= max_len; len++) {
       for(int start = 0; start < rows.size(); start++) {
          std::vector<GlpRow*> combin;
          combin.push_back(rows[start]);
          insertToRows(&all_tables, combin, rows, start+1, len-1);
       }
   }
   return all_tables;
}

double getCoefOfTable(const std::vector<GlpRow*> guests, int max_guest_of_one_table){
      double coef = abs(guests.front()->name[0] - guests.back()->name[0] );
      coef += abs(max_guest_of_one_table -2 - guests.size())*5.0;
      return coef;
}



int main(void)
{ 
 glp_prob *lp;
 lp = glp_create_prob();
 glp_set_prob_name(lp, "sample");
 glp_set_obj_dir(lp, GLP_MIN);
 double max_table_count = 100.0;
 int max_guests_of_one_table = 8;

 std::vector<GlpRow*>  guest_rows = createRowsForGuests(lp);
 GlpRow* table_count_row =  new GlpRow(lp, "table_count", GLP_DB, 1.0, max_table_count);
 //create some 
 std::list<Table> combins_part = createTables(guest_rows, 1, 3);
 std::list<Table> combins_part_others = createTables(guest_rows, 4, max_guests_of_one_table);
 std::list<Table> combins_all;
 combins_all.insert(combins_all.end(), combins_part.begin(), combins_part.end());
 combins_all.insert(combins_all.end(), combins_part_others.begin(), combins_part_others.end());

 std::vector<GlpCol*>  cols;
 std::cout << "all_rows.size=" << guest_rows.size() << "\n";
 std::cout << "combins_part.size=" << combins_part.size() << "\n";
 std::cout << "combins_all.size=" << combins_all.size() << "\n";

 for(std::list<Table>::iterator it = combins_part.begin(); it != combins_part.end(); it++) {
      std::vector<GlpRow*>& guest_rows = it->guests;
      double coef = getCoefOfTable(guest_rows, max_guests_of_one_table);
      guest_rows.push_back(table_count_row);
      GlpCol* c =  new GlpCol(lp, "", coef, guest_rows);
      cols.push_back(c);
      std::cout << c->dumpStr() << "\n";
 }

 //solve master problem
 while(true) {
   glp_std_basis(lp);
   int lp_status = glp_simplex(lp, NULL);
   assert(lp_status == 0);
  
   if (glp_get_status(lp) != GLP_OPT) break ;
   for(std::vector<GlpRow*>::iterator it = guest_rows.begin();it != guest_rows.end(); it++) {
       GlpRow* r = *it;
       r->updateDualValue();
       //std::cout << r->dumpStr() << "\n";
   }
   combins_part_others.sort(compareTableByDualVaule);
   Table t =  combins_part_others.back();
   combins_part_others.pop_back();
   std::cout <<"add new column \n";
   std::vector<GlpRow*>& guest_rows = t.guests;
   double coef = getCoefOfTable(guest_rows, max_guests_of_one_table);
   guest_rows.push_back(table_count_row);
   GlpCol* c =  new GlpCol(lp, "", coef, guest_rows);
   cols.push_back(c);
   std::cout << c->dumpStr() << "\n";

 
 //printf("\nz = %f\n", glp_mip_obj_val(lp));
 printf("\nz = %f\n", glp_get_obj_val(lp));
 std::set<std::string> row_selected;
 size_t count = 0;
 for(std::vector<GlpCol*>::iterator it = cols.begin(); it != cols.end(); it++){
     GlpCol* col = *it;
     //double x = col->getMipValue();
     double x = col->getPrim();
     if(x > 0.01 || x < -0.01) {
          std::cout <<  x << "  ";
          std::cout << col->dumpStr() << "\n";
          std::vector<GlpRow*> rs =  col->rows;
          for(size_t i = 0; i < rs.size(); i++){
                row_selected.insert(rs[i]->name);
                count++;
          };
     }
 }
 }
// std::cout << "selected unique=" << row_selected.size() << ", all=" << count << "\n";
// printf("\nz = %g; x1 = %g; x2 = %g; x3 = %g\n",
//       glp_get_obj_val(lp), x1c.getPrim(), x2c.getPrim(), x3c.getPrim());
 glp_delete_prob(lp);
return 0;
}

//                  precmat.h 2017
#ifndef PRECMAT_H
#define PRECMAT_H
#include <iostream>
#include <string>
#include <map>
#include "basegramma.h"
//
class tPrecedence{
 typedef std::map<tBaseGramma::tSymbstr,
                  tBaseGramma::tRule> tReduceTable;
 tReduceTable ReduceTable;
 size_t matrsize;
 char **matr;
 std::string errmes;
//
public:
 tPrecedence();
 ~tPrecedence();
 operator bool ()const{return matr;}
 size_t size()const  {return matrsize;}
 int rel(int i, int j)const{
  return matr[i][j];
 } 

  std::string reltostr(int i, int j)const{
  static const char* tab[]=
       {"","<","=","<=",">","<>","=>","<=>"};
//       0  1   2   3    4   5    6     7
  return tab[matr[i][j]];
 } 
 tBaseGramma::tRule reduce(
           const tBaseGramma::tSymbstr& h)const{
	  tReduceTable::const_iterator p =
                             ReduceTable.find(h);
	  return (p==ReduceTable.end()) ?
                 tBaseGramma::tRule() : p->second;
 }
 std::string getError()const {return errmes;}
 void build(const tBaseGramma&);
 bool crtrdctbl(const tBaseGramma&);
};
//
void out_matrix(std::ostream&, const tBaseGramma&,
           const tPrecedence& );
#endif


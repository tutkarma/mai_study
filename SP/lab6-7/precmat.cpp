//                 precmat.cpp 2017
#include <iostream>
#include <string>
#include <iomanip>
#include "precmat.h"
using namespace std;

tPrecedence::tPrecedence():
     matrsize(0),matr(0){}
//
tPrecedence::~tPrecedence()
{
 if(matr){
   for (size_t i=0; i<matrsize; ++i) delete []matr[i];
   delete []matr;
 }
}

//

void tPrecedence::build(const tBaseGramma& gr)
{
  for (size_t i=0; i<matrsize; ++i) delete []matr[i];
  delete []matr;
  ReduceTable.clear();
  matr = 0;
  matrsize = 0;
  errmes = "???";

  if(!gr){errmes=gr.getError(); return;}
 if(!crtrdctbl(gr)) return;

 size_t abcsz=gr.ABCsize();
 matr = new char*[abcsz];
 size_t i=0;
 try{
  for (i=0; i<abcsz; ++i)
          matr[i] = new char[abcsz];
 }
 catch(std::bad_alloc){
  for(size_t j=0; j<i; ++j)
                 delete[]matr[j];
  delete[]matr;
  matr=0;
  throw;
 }

 matrsize = abcsz;
 for (i=0; i<matrsize; ++i)
   for (size_t j=0; j<matrsize; ++j) matr[i][j] = 0;

//
  tBaseGramma::tSymbstrset firsts, lasts;
  gr.makeFirst(firsts);
  gr.makeLast(lasts);

 size_t start=gr.getStart();
 for(size_t left=start; left<abcsz; ++left)
  for(size_t ialt=0; ialt<gr.altCount(left);
                                 ++ialt ){
     const tBaseGramma::tSymbstr& rp =
                        gr.rightPart(left,ialt);
     for(size_t k=1; k<rp.length(); ++k)
      {
       size_t A = rp[k-1];
       size_t B = rp[k];
// определяем отношение = между символами,
// стоящими рядом в правой части продукции
       matr[A][B] |= 2;

       const tBaseGramma::tSymbstr& last = lasts[A];
       const tBaseGramma::tSymbstr& first = firsts[B];
//
       if(!gr.terminal(B)){
//  B - НЕТЕМИНАЛ
// определяем отношение < между символом A и всеми
// символами из FIRST(B)
        for (size_t i=0; i<first.length(); ++i)
                       matr[A][first[i]] |= 1 ;
// определяем отношение > между всеми символами из
// LAST(A) и ТЕРМИНАЛАМИ(!) из FIRST(B)
        for (size_t j=0; j<last.length(); ++j)
          for (size_t i=0; i<first.length(); ++i) 
            if(gr.terminal(first[i]))
                 matr[last[j]][first[i]] |= 4 ;
        } //if...
       else {
// B - ТЕРМИНАЛ
// определяем отношение > между всеми символами из
// LAST(A) и символом B
        for (size_t j=0; j<last.length(); ++j)
                         matr[last[j]][B] |= 4 ;
       }//else...
     }//for k
   }//for ialt, for left
//
// определяем отношение < между символом
// #(маркер дна стэка) и всеми символами из
// FIRST(START)
  size_t marker = 1;
  const tBaseGramma::tSymbstr& last = lasts[start];
  const tBaseGramma::tSymbstr& first = firsts[start];
//
  for (size_t i=0; i<first.length(); ++i) 
                     matr[marker][first[i]] = 1 ;
// доопределяем отношение < между символами # и START
  matr[marker][start] |= 1;

// определяем отношение > между всеми символами из
// LAST(START) и символом #(маркер конца входной
// цепочки)
  for (size_t j=0; j<last.length(); ++j)
                      matr[last[j]][marker] = 4 ;
// доопределяем отношение > между символами START и #
  matr[start][marker] |= 4;
//
  errmes = "OK";
  return ;
}

bool tPrecedence::crtrdctbl(const tBaseGramma& gr){
 size_t abcsz=gr.ABCsize();
 size_t start=gr.getStart();
 for(size_t left=start; left<abcsz; ++left)
  for(size_t ialt=0; ialt<gr.altCount(left);
                                 ++ialt ){
     tBaseGramma::tSymbstr rp =
                        gr.rightPart(left,ialt);
     tBaseGramma::reverse(rp);
     tReduceTable::iterator it=ReduceTable.find(rp);
     if(it==ReduceTable.end()){
      ReduceTable[rp] =
                 tBaseGramma::tRule(left, ialt);
      continue;
     }
// правая часть уже есть в таблице
//b14, b18
        errmes = "GRAMMA:duplicated right "
                 "part for "+gr.decode(left);
        size_t precleft = it->second.left;
        if(left != precleft) errmes +=
                " and " + gr.decode(precleft) + " !";
    ReduceTable.clear();
    return false;
 }// for ialt, for left
 return true;
}

void out_matrix(std::ostream& out,
                const tBaseGramma& gr,
                const tPrecedence& prc){
 size_t maxw=gr.smbWidth()+1;
 if(maxw<5) maxw=5;

 size_t n=prc.size();
 for (size_t i = 0; i < n; ++i)
      out << setw(maxw) << gr.decode(i);
 out << endl;

 for (size_t i = 1; i < n; ++i){
  out << setw(maxw) << gr.decode(i);
  for (size_t j = 1; j < n; ++j)
              out << setw(maxw) <<
                  prc.reltostr(i,j);
  out << endl;
      }
}

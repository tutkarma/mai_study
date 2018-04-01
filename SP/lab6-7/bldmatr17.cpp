//                 bldmatr.cpp 2017
#include <iostream>
#include <iomanip>
#include "kngramma.h"
#include "precmat.h"

#include "basegramma.cpp"
#include "kngramma.cpp"
#include "precmat.cpp"
using namespace std;

int main()
{
  char buf[100];

  do{
   cout << "Input gramma name>";
   *buf = 0;
   cin.getline(buf,100);
  if (!*buf) break;

   char* name=buf;
   bool print=false;
   if(*name=='+'){ ++name; print=true;}
   string gramma_name = 
                  string( name )+".txt" ;
   cout << "Gramma:"<<
            gramma_name<<endl;
//____________________________________
  tGramma gr;
  gr.loadFromFile(gramma_name.c_str());
  outgr(cout,gr);
//
  tPrecedence prc;
  prc.build(gr);
  if(!prc){
   cout<<prc.getError()<<endl;
   continue;
  }
//
  if(print)out_matrix(cout,gr,prc);
 //
  size_t maxw=gr.smbWidth()+1;
  size_t n=prc.size();
  bool noconfl=true;
  for (size_t i=1; i<n; ++i)
    for (size_t j=1; j<n; ++j) {
      int k = prc.rel(i,j);
      if(k && k!=1 && k!=2 && k!=4){
        if(noconfl){
          noconfl=false;
          cout <<"Conflicts:"<< endl;
          }
        cout << setw(5) << prc.reltostr(i,j) <<" ";
        cout << setw(maxw) << gr.decode(i)<<" ";
        cout << setw(maxw) << gr.decode(j);
        cout << endl;
      }
   }
//
   if(noconfl)cout<<"No conflicts!\n";
 } while(true);
 return 0;
}

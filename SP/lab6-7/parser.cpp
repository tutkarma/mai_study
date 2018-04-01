//                 parser.cpp 2017
#include <sstream>
#include "parser.h"
using namespace std;

tParser::tParser(const char* gramma_name)
{
  PARSER_DEBUG=false;
  gr.loadFromFile(gramma_name);
  prc.build(gr);
  if(!prc){ferror_message=prc.getError();
           return; }             
}

int tParser::parse(const char* source_name){
  if(!prc){
// испорчены управл€ющие таблицы
    return 1;
   }
   ferror_message.clear();
   if(!lex.Begin(source_name)){
    ferror_message = string("Can't open file ")+
                                     source_name;
    return 1;
    }

   std::ostringstream buf;
  if(PARSER_DEBUG){
      cout << lex.Authentication()<<endl;
      cout << "______________________\n";
      }
   enum tState {stShift,stReduce};
   vector<tGramma::tSymb> stack;
   tState state = stShift;
   tGramma::tSymb a,b,term = 1;// маркер
   int rel;
//+++++++++++++++++++++++++++++++
   while(true){
    switch (state){
     case stShift:
         stack.push_back(term);
         if(PARSER_DEBUG){
           if(term!=1) cout<<"  <-  "
                        <<gr.decode(term)<<endl;
         }
         term = getTerm();
         if (!term){
                    buf << "Syntax: unknown token!";
                    goto LERROR;
                    }
         break;

     case stReduce:
// выделить обращенную основу
      tGramma::tSymbstr reverse_right_part;
      a = stack.back();
      do{
        reverse_right_part += a;
        b = a;
        stack.pop_back();
        a = stack.back();
        rel = prc.rel(a,b);
      }while(rel != 1); // отношение <

// найти левую часть продукции
      tGramma::tRule descr =
               prc.reduce(reverse_right_part);

  if(PARSER_DEBUG)
        out_prod(cout,gr,descr);

      tGramma::tSymb left = descr.left;
      if (left == 0){ //продукци€ не найдена
                    buf <<
                     "Syntax: chain '";
                    tGramma::tSymbstr chain(reverse_right_part);
                    tGramma::reverse(chain);
                    out_chain(buf,gr,chain);
                    buf << "' is not a right part of any production ! ";
                    goto LERROR;
                   }
// заменить основу символом левой части
      stack.push_back(left);
// проверить отношение между
// предшесвующим основе симлом стэка и
// символом левой части продукции
      rel = prc.rel(a,left);
      if (rel == 0){
                    buf <<
                     "Syntax: symbol '"
                  << gr.decode(a)
                  << "' can't precede nonterminal '"
                  << gr.decode(left)
                  << "' !";
                    goto LERROR;
                   }
// проверить условие допустимости цепочки
      if(stack.size() == 2 &&
         stack[1] == (tGramma::tSymb)gr.getStart() &&
         term == 1){// маркер коца
                     lex.End();
                     return 0;
        }
    }//switch...
// выбрать следующее состо€ние
   a = stack.back();
   b = term;
   rel = prc.rel(a,b);
   if (rel == 0){
                    buf <<
                     "Syntax: symbol '"
                  << gr.decode(a)
                  << "' can't precede terminal '"
                  << gr.decode(b)
                  << "' !";
                    break;
                  }
//              отношение >
    state = (rel == 4) ? stReduce : stShift;
   }
//+++++++++++++++++++++++++++++++
   LERROR:
// добавить к сообщению об ошибке номер
// строки и смещение
     buf<< endl;
     buf<< setw(4) << 
         lex.GetLineCount()<<"|"<<
         lex.GetLineText()<< endl;
     buf<< "     " << 
          setw(1+lex.GetStartPos()) << "^"
        << endl;
     ferror_message += buf.str();

  if(PARSER_DEBUG){
    cout <<   "Stack:";
    for( size_t i=0; i<stack.size(); ++i)
                  cout<<" "<<gr.decode(stack[i]);
    cout<<"  <-  "<<gr.decode(term)<<endl;
    }
     
     lex.End();
     return 1;
}

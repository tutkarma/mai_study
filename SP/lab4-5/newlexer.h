//                 lexer.h 2018
#ifndef LEXER_H
#define LEXER_H
#include "baselexer.h"
//********************************************
//*        Developed by  xxx		     *
//*             (c)  2018                    *
//********************************************
class tLexer:public tBaseLexer{
public:
// °±®­ «­»© ®¤ ° §° ®· 
 std::string Authentication()const{
                     return "xxx"
                +std::string("2018");}
//®­±°®°
 tLexer():tBaseLexer(){
//±®§¤   ®¬ »:

//  ­®« Azero
    addstr  (Azero,0,"+-", 2);
    addstr  (Azero,0,"0",  1);
    addstr  (Azero,2,"0",  1);
  Azero.final(1);

//________________________________________

// 
    addstr  (Adec,0,"0",   3);
    addstr  (Adec,3,".",   2);
    addrange(Adec,0,'1','9',1);
    addstr  (Adec,1,".",   2);
    addrange(Adec,2,'0','9',2);
  Adec.final(1);
  Adec.final(2);
//________________________________________

// ¤­ ®°
    addrange(Aid,0,'a','z',1);
    addrange(Aid,1,'a','z',1);
    addstr  (Aid,1,"!",   1);
  Aid.final(1);
//________________________________________

// ¤­ ®° °¤  
    addstr  (Aidq,0,"?",   1);
  Aidq.final(1);
}
};
#endif


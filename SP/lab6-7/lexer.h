//                 lexer.h 2018
#ifndef LEXER_H
#define LEXER_H
#include "baselexer.h"
//********************************************
//*        Developed by  AAD		     *
//*             (c)  2018                    *
//********************************************
class tLexer:public tBaseLexer{
public:
// персональный код разработчика
    std::string Authentication()const{
                        return "AAD"
                    +std::string("2018");}
//конструктор
    tLexer():tBaseLexer(){
//создать автоматы:

//  ноль Azero
        addstr  (Azero,0,"+-", 2);
        addstr  (Azero,0,"0",  1);
        addstr  (Azero,2,"0",  1);
        Azero.final(1);

//________________________________________

// 
        addstr(Adec,0,"+-",1);
        addstr(Adec,0,"0",2);
        addstr(Adec,0,".",5);
        addrange(Adec,0,'1','9',3);
        addstr(Adec,1,"0",4);
        addrange(Adec,1,'1','9',3);
        addstr(Adec,1,".",5);
        addstr(Adec,2,".",5);
        addstr(Adec,2,"eE",7);
        addstr(Adec,3,".",5);
        addstr(Adec,3,"eE",7);
        addrange(Adec,3,'0','9',3);
        addstr(Adec,4,".",5);
        addrange(Adec,5,'0','9',6);
        addrange(Adec,6,'0','9',6);
        addstr(Adec,6,"eE",7);
        addstr(Adec,7,"+-",8);
        addrange(Adec,8,'0','9',9);
        addrange(Adec,9,'0','9',9);
//......................
        Adec.final(3);
        Adec.final(6);
        Adec.final(9);
//________________________________________

// идентификатор
        //addrange(Aid,0,'a','z',1);
        //addrange(Aid,1,'a','z',1);
        //addstr  (Aid,1,"!",   1);
        addrange(Aid,0,'a','z',1);
        addrange(Aid,0,'A','Z',1);
        addstr(Aid,0,"!",1);
        addrange(Aid,1,'a','z',1);
        addrange(Aid,1,'A','Z',1);
        addrange(Aid,1,'0','9',1);
        addstr(Aid,1,"-",1);
        addstr(Aid,1,"!",1);
        Aid.final(1);
//________________________________________

// идентификатор предиката
        //addstr(Aidq,0,"?",1);
        //Aidq.final(1);
        addstr(Aidq,0,"?",2);
        addstr(Aidq,0,"-",1);
        addrange(Aidq,0,'a','z',1);
        addrange(Aidq,0,'A','Z',1);
        addstr(Aidq,1,"?",2);
        addstr(Aidq,1,"-",1);
        addrange(Aidq,1,'a','z',1);
        addrange(Aidq,1,'A','Z',1);
        addrange(Aidq,1,'0','9',1);
        Aidq.final(2);
}
};
#endif


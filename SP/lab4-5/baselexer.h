//                 baselexer.h 2018
#ifndef BASELEXER_H
#define BASELEXER_H
#include <iostream>
#include <fstream>
#include <string>
#include "fsm.h"

class tBaseLexer{
 std::ifstream fsource;//ïîòîê
//               ââîäà èç ôàéëà
 int         line;//ñ÷åò÷èê ñòðîê
 char        buf[1000];//áóôåð ñòðîêè
 static const size_t bufsize=1000;
 char*       start;//óêàçàòåëü íà÷àëà
//                   ëåêñåìû â áóôåðå
 char*       end;//óêàçàòåëü êîíöà
//                 ëåêñåìû â áóôåðå
 std::string lexeme;//ëåêñåìà

public:
//êîíñòðóêòîð
 tBaseLexer();
//äåñòðóêòîð
 ~tBaseLexer() {}
//ôóíêöèè-÷ëåíû
 bool Begin(const char* filename);//ïîä-
//            ãîòîâêà ê ðàáîòå ñ ôàéëîì
 void End(){fsource.close();}//çàâåðøåíèå
//                        ðàáîòû ñ ôàéëîì

 std::string GetToken();//èçâëåêàåò èç
//                 òåêñòà î÷åðåäíîé òîêåí
 std::string GetLexeme()const{return lexeme;}
 int GetLineCount()const{return line;}
 int GetStartPos()const{return start-buf;}
 int GetEndPos()const{return end-buf;}
 std::string GetLineText()const{
                     return std::string(buf);}

protected:
//àâòîìàòû
  tFSM Azero;//íîëü
  tFSM Adec; //âåùåñòâåííîå/öåëîå
  tFSM Aid;  //èäåíòèôèêàòîð
  tFSM Aidq; //èäåíòèôèêàòîð ïðåäèêàòà
private:
  tFSM Aoper;//îïåðàòîð
  tFSM Abool;//áóëåâñêàÿ êîíñòàíòà
  tFSM Astr; //ñòðîêà
};
#endif


//                 baselexer.h 2018
#ifndef BASELEXER_H
#define BASELEXER_H
#include <iostream>
#include <fstream>
#include <string>
#include "fsm.h"

class tBaseLexer{
 std::ifstream fsource;//поток
//               ввода из файла
 int         line;//счетчик строк
 char        buf[1000];//буфер строки
 static const size_t bufsize=1000;
 char*       start;//указатель начала
//                   лексемы в буфере
 char*       end;//указатель конца
//                 лексемы в буфере
 std::string lexeme;//лексема

public:
//конструктор
 tBaseLexer();
//деструктор
 ~tBaseLexer() {}
//функции-члены
 bool Begin(const char* filename);//под-
//            готовка к работе с файлом
 void End(){fsource.close();}//завершение
//                        работы с файлом

 std::string GetToken();//извлекает из
//                 текста очередной токен
 std::string GetLexeme()const{return lexeme;}
 int GetLineCount()const{return line;}
 int GetStartPos()const{return start-buf;}
 int GetEndPos()const{return end-buf;}
 std::string GetLineText()const{
                     return std::string(buf);}

protected:
//автоматы
  tFSM Azero;//ноль
  tFSM Adec; //вещественное/целое
  tFSM Aid;  //идентификатор
  tFSM Aidq; //идентификатор предиката
private:
  tFSM Aoper;//оператор
  tFSM Abool;//булевская константа
  tFSM Astr; //строка
};
#endif


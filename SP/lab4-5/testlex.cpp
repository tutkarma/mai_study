//                 testlex.cpp 2017
#include <iostream>
#include <fstream>
#include <iomanip>
#include "lexer.h"
#include "baselexer.cpp"
using namespace std;

int main(){
  tLexer lex;
//*****************************************
    while(true){
        char linebuf[1000];
        cout << "\nSource>";
        *linebuf = 0;
        cin.getline(linebuf,1000);
        if(*linebuf == 0) break; //завершение работы
        string source_name = string(linebuf) + ".ss";
        bool file=false;
    {
        ifstream tmp(source_name.c_str());
        // строка ввода похожа на имя файла
        if(tmp) file = true;
    }
    if(!file){
        source_name = "temp.ss";
        ofstream tmp(source_name.c_str());
        tmp << linebuf << endl;
    }
    cout << "\nSource file name:"
         << source_name << endl;
//  распечатать исходный текст с номерами строк
    {// начало блока
        ifstream tmp(source_name.c_str());
        int linecount=0;
        while(tmp){
        *linebuf=0;
        tmp.getline(linebuf,1000);
        cout<< setw(4)<< ++linecount<<"|"<< linebuf<<endl;
    }//while(tmp)...
     cout<<"_________________\n";
    }// конец блока
    if(!lex.Begin(source_name.c_str())){
        cout << "Can't open file "<< source_name <<endl;
        continue;
    }
    cout<<lex.Authentication()<<" Lexer scan:"<<endl;
//+++++++++++++++++++++++++++++++
    while(true){
        string token = lex.GetToken();
        string lexeme = lex.GetLexeme();
        cout <<setw(2)<<lex.GetLineCount()<<
         "/" <<setw(2)<<lex.GetStartPos()<<":"<<
        setw(5) << token <<"  "<< lexeme << endl;
        if(token == "#") break;
    }
//+++++++++++++++++++++++++++++++
    lex.End();
 }
//*****************************************
}

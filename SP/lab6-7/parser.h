//                         parser.h 2017
#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "kngramma.h"
#include "precmat.h"

class tParser{
public:
 tParser(const char* gramma_name);
 ~tParser(){}

 int parse(const char* source_name);
 const std::string& getMessage()const
                    {return ferror_message;}
 operator bool()const{return prc;}
 bool PARSER_DEBUG;
 const tGramma& getGramma()const{return gr;}

private:
// блокировать копирующие функции
 tParser(const tParser& );
 tParser& operator=(const tParser& );

 tLexer             lex;
 tGramma            gr;
 tPrecedence        prc;
 std::string        ferror_message;

// функция 
 tGramma::tSymb getTerm();

};

inline tGramma::tSymb tParser::getTerm(){
 std::string token=lex.GetToken();
 tGramma::tSymb term = gr.encode(token);

//  из токена $id извлекаются ключевые слова
 if(token == "$id"){
   std::string ident = lex.GetLexeme();
   tGramma::tSymb keyword=gr.encode(ident);//поиск в алфавите
// если идентификатор найден и является терминалом,
   if(keyword && gr.terminal(keyword))
//                        это ключевое слово
                          term = keyword;
  }
 return term;
}
#endif

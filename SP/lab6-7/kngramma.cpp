//    03.01.2017
#include "kngramma.h"
#include <utility>
#include <cstring>
#include <vector>
#include <fstream>
#include <algorithm>
//
using namespace std;

 string tGramma::compressVert(const char *p){
// устраняет удвоение | в записи продукций

  if(!p || !(*p)) return string();
  string s(1,*p);
  bool odd=( *p == '|');

  while(*(++p)){
   if( *p != '|'){
    if(odd) return string();
    s += *p;
    continue;
   }

   if(s[s.size()-1] == '|' ){
    if(odd){
     odd = false;
     continue;
    }
   }

   s += '|';
   odd = true;
  }// while...
 return odd ? string() : s;
 }

bool tGramma::isPrefix(const char * const p, 
                       const char * const prefix)
{
	size_t prefl=strlen(prefix);
	if(strlen(p)<prefl) return false;
	size_t i=0;
	while(prefix[i] && p[i]==prefix[i]) i++;
	return i==prefl;
}

int tGramma::myAtoi(const char * const p)
{
	if(p==NULL) return -1;
	int rez=0;
	size_t pl=strlen(p);
//	if(pl>3) return -1;
	for(size_t i=0; i<pl; ++i)
	{
		if(isdigit(p[i]))
		{
			rez=rez*10+(p[i]-'0');
		}
		else
		{
			return -1;
		}
	}
	return rez;
}

bool tGramma::addAlt(tSymb left, tAlt& alt){
  tAlternatives& alts = Prod[left - Start];
	int rplen = alt.rp.length();
	if(rplen)
	{                            
	if(rplen==1 && 	alt.rp[0]==left)
		{
// Нетерминал выводится сам из себя I -> I
//b13
        errmes = "GRAMMA:invalid production "
                 + decode(left)
                 + " -> " + decode(left) + " !";
    		  clear();
		  return false;
		}
		alts.push_back(alt);
       	        alt = tAlt();
		}
		else
		{
//  правая часть пустая
//b15
        errmes = "GRAMMA:empty right part for "
                 + decode(left) + " !";
		 clear();
		 return false;
		}
 return true;
}

void tGramma::loadFromFile(const char* filename)
{
	const int buffSize=300;
	const char delimiters[]=" \t\r";

	//Инициализация
        errmes = "?????";
	Start = 0;
	Abc.clear();
	Iabc.clear();
	Prod.clear();

	//Загрузка алфавита
	ifstream inputF(filename);
	if( !inputF.is_open() )
	{
                errmes = "GRAMMA: can't open file " +
                          string(filename);
		return;
	}

	char *p=NULL;
	char str[buffSize];

	/*Пропускаем все пустые строки
	и строки,  состоящие только из
	символов  класса   П Р О Б Е Л
	во входном файле*/
	str[0]='\0';
	while((!*str || 
               ((p=strtok(str,delimiters))==NULL))
	      && !inputF.eof())
	{
		inputF.getline(str,buffSize);
	}

	//если файл пуст
	if(p==NULL)
	{
//b1
          errmes = "GRAMMA: unexpected end of file ";
	  return;
	}

        if(strchr(p,'|'))
        {
//b20
          errmes = "GRAMMA: character '|' is not "
                   "allowed in a marker symbol";
	  return;
	}

	string MARKER=p;
	Abc.push_back("");
	Abc.push_back(MARKER);

	//загрузка алфавита из файла
	p=strtok(NULL,delimiters);
	int markerCount=1;
	while (markerCount<2)
	{
		while( (p!=NULL) &&
                       (MARKER != p) )
		{
	 //выполним проверку на уникальность
		 tABC::const_iterator ip = 
                    find(Abc.begin(), Abc.end(), p);
		 if(ip==Abc.end())
			{
				Abc.push_back(p);
			}
			else
			{
//b3
    errmes = "GRAMMA: multiple definition of terminal "
                         + string(p);
				clear();
				return;
			}
			p=strtok(NULL,delimiters);
		}
		if(p==NULL)
		{
			/* p==NULL, т.е.
			в текущей строке терминалов
			больше нет */
			if(!inputF.eof())
			{
			 inputF.getline(str,buffSize);
			 p=strtok(str,delimiters);
			}
			else
			{
	 /*если маркер не встретился второй раз, а
	файл прочтан полностью, тогда это ошибка*/
//b2
        errmes = "GRAMMA: expected marker "
               + MARKER
               + " at the end of alphabet ";
				clear();
				return;
			}
		}
		else
		{
		 /*  противном случае,
		 текущий терминал -- МАРКЕР*/
		 ++markerCount;
		 p=strtok(NULL,delimiters);
		 if(p!=NULL)
		  {
		/*В текущей строке после МАРКЕРА
		есть ещё что-то!*/
//b4
   errmes = "GRAMMA: definition of production "
            "must begin in a new line ";
			clear();
			return;
 		 }
		}
	}

	/*Расширение алфавита --
	введение списка нетерминалов*/
	Start=Abc.size();
	str[0]='\0';
	bool broken=false;
	/*broken = true, если в следующей строке
	находится продолжение текущего списка
	альтернатив*/


	while(!inputF.eof())
	{
	  inputF.getline(str,buffSize);
		
//Поиск следующего нетерминального символа
       		while((!(*str) ||
                  ((p=strtok(str,delimiters))==NULL))
                      &&  !inputF.eof())
		{
		  inputF.getline(str,buffSize);
		}

		if(p==NULL)
			break;

		if(!broken)
		{
	/*текущий СИМВОЛ - левый СИМВОЛ продукции
	выполним проверку на уникальность*/
			string e = compressVert(p);
			if(e.empty())
			{				 
//b5
                errmes = "GRAMMA: incorrect symbol " +
                          string(p);
				clear();
				return;
			}
		tABC::const_iterator ip = 
                  find(Abc.begin(), Abc.end(), e);
		if(ip==Abc.end())
			{
				Abc.push_back(e);
				 
			}
			else
			{
	/*данный  СИМВОЛ  уже есть!
	возможно, в качестве левого
	С И М В О Л А  используется
	терминал!
	*/
//b6
    errmes = "GRAMMA: multiple definition of symbol "
                         + e;
				 
				clear();
				return;
			}
		}

		/*если в конце строки находится
		"ПРОБЕЛ + |", значит данная продукция
		многострочная. Последующие строки
		нужно пропустить*/
		broken=false;

//		уже обработали левый символ
//		(или обрабатываем очередной правый)
		while(p!=NULL)
		{
		  broken = (strcmp(p,"|")==0);
	          p=strtok(NULL,delimiters);
		}
	}
	/*если список альтернатив прерван,
	то это ошибка*/
	if(broken)
	{
//b8
   errmes = "GRAMMA: no last alternative "
            "of nonterminal "
          + Abc.back();
		clear();
		return;
	}
	/*если не было прочитано ни одного
	нетерминала, это ошибка*/
	if(ABCsize() == Start)           
	{
//b7
   errmes = "GRAMMA: no productions ";
		clear();
		return;
	}

	if(ABCsize() > 256)
	{
   errmes = "GRAMMA: total number of symbols > 255";
		clear();
		return;
	}
  
  Smbwidth =0;
// построить инвертированный алфавит
  for(size_t i=1; i<Abc.size(); ++i){
    string s = decode(i);
    Iabc[s] = i;
    size_t ln = s.length();
    if(ln > Smbwidth) Smbwidth = ln;
 }
 Prod.resize(Abc.size()-Start);

/* Получение списка продукций
	 (второй проход) */
	inputF.close();
	inputF.clear();
	inputF.open(filename,ios_base::in);
	markerCount=0;
	tSymb left=Start;
	bool active;

	//Повторно перебираем весь АЛФАВИТ
	while(markerCount<2)
	{
		str[0]='\0';
		while((!(*str) ||
                  ((p=strtok(str,delimiters))==NULL))
                      && !inputF.eof())
		{
			  inputF.getline(str,buffSize);
		}
		while(p!=NULL)
		{
			if(MARKER == p)
			{
				++markerCount;
			}
			p=strtok(NULL,delimiters);
		}
	}

//составляем список продукций
	while(!inputF.eof())
	{
		str[0]='\0';

//Переходим к следующей продукции
		while((!(*str) || 
                  ((p=strtok(str,delimiters))==NULL))
                      && !inputF.eof())
		{
		  inputF.getline(str,buffSize);
		}
//  если файл закончился, то выход из цикла
		if(p==NULL)
			break;

		p=strtok(NULL, delimiters);

		if(p==NULL || !isPrefix(p,"->"))
		{
//b10
       errmes="GRAMMA:expected -> after left part "+
                         decode (left);
			clear();
			return;
		}

		//начало альтернативы
		if(strcmp(p,"->"))
		  p+=2;
		else
		  p=strtok(NULL, delimiters);

		active=true;
		tAlt currentAlternative;
		                                                                  
		while(active)
		{
 		  while(p!=NULL
                     && strcmp(p, "|")
                     && !isPrefix(p,MARKER.c_str()))
			{
//   пока не встретится символ | или МАРКЕР
			string e = compressVert(p);
			if(e.empty())
				{
//b10
                errmes = "GRAMMA: incorrect symbol " +
                          string(p);
				clear();
					return;
				}
	         tSymb r =  encode(e);
				if(r == 0)
				{
//  текущего СИМВОЛА нет в алфавите
//b11,b9
                errmes = "GRAMMA: unknown symbol "
                         + e
                         + " in a production of " 
                         + decode(left);                                  
					clear();
					return;
				}
			currentAlternative.rp +=r;
			p=strtok(NULL, delimiters);
			}
	if(p==NULL)
	{
// по следний символ строки отличен от |,
// то есть запись всех альтернатив
// для данной левой части завершена 
	currentAlternative.hndl =0;
		if(!addAlt(left,currentAlternative))
					return;
		active=false;
	}
	else
	{
	currentAlternative.hndl =0;
	if(isPrefix(p,MARKER.c_str()))
	{
// далее следует дескриптор
// семантического преобразования

	int z;
	if(MARKER != p)
	{
// p содержит маркер
	p+=MARKER.size();
	}
	else
	{
// р равен маркеру
	p=strtok(NULL,delimiters);
	}
	z=myAtoi(p);
	if(z>999 || z<0)
	  {
           z = 0;
//b16
            errmes = "GRAMMA:wrong descriptor '"
                 + string(p)
                 + "' at production of "
                 + decode(left);
		clear();
		return;
	  }
	currentAlternative.hndl =z;
	p=strtok(NULL,delimiters);
	if(p!=NULL && strcmp(p,"|"))
	  {
// после ДСП символ отличен от |
//b17
            errmes = "GRAMMA:expected ' |' "
                     "or new line after descriptor "
                     " at production of "
                 + decode(left);
		clear();
		return;
	  }
	}

// p=="|" или NULL
//добавляем в список альтернатив
	if(!addAlt(left,currentAlternative))
					return;
	if(p==NULL)
	{
		active=false;
	}
	else
	{
//получаем следующий символ
	p=strtok(NULL,delimiters);
	if(p==NULL)
	{
//список альтернатив разрывается - просматриваем
// строки дольше
	str[0]='\0';
	while((!(*str) || 
             ((p=strtok(str,delimiters))==NULL))
              && !inputF.eof())
		{
		inputF.getline(str,buffSize);
		}
	}
	active=true;
	}
	}
	}
	 ++left;
	}

 inputF.close();
 return;
}


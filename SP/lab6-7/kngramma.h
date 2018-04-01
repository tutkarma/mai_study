//               kngramma.h 2017
#ifndef GRAMMA_H
#define GRAMMA_H

#include "basegramma.h"
//
class tGramma:public tBaseGramma{
//
 void clear() {
	Abc.clear(); Start=0; Iabc.clear();
        Prod.clear();
	}

 static bool isPrefix(const char * const p, 
                      const char * const prefix);
 static std::string compressVert(const char *p);
 static int myAtoi(const char * const p);

public:
 tGramma(): tBaseGramma() {};
 void loadFromFile(const char* filename);

protected:
 bool addAlt(tSymb left, tAlt& alt);

};
//
#endif



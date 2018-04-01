//                 basegramma.h 2017
#ifndef BASEGRAMMA_H
#define BASEGRAMMA_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
//
class tBaseGramma{
public:
 typedef unsigned char      tSymb;
 typedef unsigned short     tAtrfun;
 typedef unsigned char      tAltind;
 typedef std::basic_string<tSymb> tSymbstr;
 typedef std::vector<tSymbstr>   tSymbstrset;
 struct tRule{
  tSymb left;
  tAltind ialt;
  tRule(tSymb aleft=0, tAltind aind=0):
         left(aleft), ialt(aind){}
 }; //tRule

 struct tAlt{
  tSymbstr rp;
  tAtrfun hndl;
  tAlt(tSymbstr arp=tSymbstr(), tAtrfun ahndl=0):
         rp(arp), hndl(ahndl){}
 }; //tAlt

protected:
 typedef std::vector <std::string>    tABC;
 typedef std::map <std::string,tSymb> tInverseABC;
 typedef std::vector <tAlt>  	      tAlternatives;
 typedef std::vector <tAlternatives>  tProductions;
//

 size_t       Smbwidth;
 tABC         Abc;
 size_t       Start;
 tInverseABC  Iabc;
 tProductions Prod;

 std::string errmes;
 tAlt NULLALT;
//

public:
 tBaseGramma():
    Start(0),Smbwidth(0), errmes("?????") {};
 ~tBaseGramma() {};

 operator bool()const   {return Start;}
 size_t getStart()const {return Start;}
 size_t ABCsize()const  {return Abc.size();}
 size_t smbWidth()const  {return Smbwidth;}
 bool   terminal(tSymb code)const {
                            return code<(tSymb)Start; }
 std::string decode(tSymb code)const {
                             return  Abc[code];}
 tSymb encode(const std::string& s)const {
    tInverseABC::const_iterator p = Iabc.find(s);
    return p==Iabc.end() ? 0 : p->second;
			 }
 size_t altCount(tSymb left_part)const
		  {return
               Prod[left_part - Start].size();}

 const tSymbstr& rightPart(tSymb left_part,
            tAltind alt_index)const
  {return Prod[left_part - Start][alt_index].rp;}

 const tAlt& getAlt(const tRule& rule)const{
  return rule.left ? Prod[rule.left - Start][rule.ialt] :
                     NULLALT;
 }

 std::string getError()const {return errmes;}

 static void reverse(tSymbstr& );

 void makeFirst(tSymbstrset&)const;
 void makeLast(tSymbstrset&)const;
 void makeFollow(tSymbstrset&)const;
//
 static void addunique(tSymbstr& strset, tSymb a);
 void add_to_first(tSymb a, tSymbstr& first)const;
 void createfirst(tSymb a, tSymbstr& first)const;
 void add_to_last(tSymb a, tSymbstr& last)const;
 void createlast(tSymb a, tSymbstr& last)const;

}; //tBaseGramma

void out_chain(std::ostream& out, const tBaseGramma& gr,
               const tBaseGramma::tSymbstr& rp);
void out_prod(std::ostream& out, const tBaseGramma& gr,
           const tBaseGramma::tRule& rule);
void outgr(std::ostream& out, const tBaseGramma& gr);
void enumgr(std::ostream& out, const tBaseGramma& gr);
#endif



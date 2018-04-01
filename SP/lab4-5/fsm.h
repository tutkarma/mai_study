#ifndef FSM_H
#define FSM_H
#include <vector>
#include <map>
#include <set>

class tFSM{
public:
//  типы
  typedef char tSymbol;
  typedef unsigned char tState;
  typedef std::set<tState> tStateSet;
// конструктор
  tFSM(){}; //создает "пустой" автомат
// функции-члены
//     добавл€ет одну команду (from,c)->to
  void add(tState from, tSymbol c, tState to);
  void final(tState st);//включает одно состо€ние
//                      во множество заключительных
  int  apply(const tSymbol* input);//примен€ет автомат
//                                к входной цепочке

  void clear();// очищает автомат
  size_t size()const{return table.size();}//выдает
//       размер (количество состо€ний) автомата

  friend class tFSMcheck;// класс дл€ проверки
//                      корректности автомата
private:
// представление детерминированного конечного
//            автомата
  typedef std::map<tSymbol,tState> tTransMap;
  typedef std::vector<tTransMap> tStateTable;

  tStateTable 	table;  //таблица состо€ний
  tStateSet 	finals; //множество заключитеьных
                        // состо€ний
};
// функции-помощники
  void addstr(tFSM& fsm,
              tFSM::tState from, const tFSM::tSymbol *str,
              tFSM::tState to);
  void addrange(tFSM& fsm,
                tFSM::tState from, tFSM::tSymbol first,
                tFSM::tSymbol last, tFSM::tState to);
//------------------------------------------------------
//        –≈јЋ»«ј÷»я
inline void tFSM::add(tState from,tSymbol c,tState to){
  size_t sz=1+(from > to ? from : to);//1+max(from,to)
  if (sz > table.size())table.resize(sz);//увеличивает
//                 размер вектора до sz
  table[from][c] = to; //два перегруженных оператора []:
                     //один дл€ vector, другой дл€ map.
}

inline void tFSM::final(tState st){finals.insert(st);}

inline void tFSM::clear(){finals.clear();
                          table.clear();
}

inline int tFSM::apply(const tSymbol* input){
  if(table.empty()) return 0;// пуста€ таблица
//                              состо€ний
  tState state=0; //начальное состо€ние
  int accepted=0;

// цикл прохода по входной цепочке
  while (*input){
    tTransMap::iterator iter;// итератор
//                              контейнера map
    tTransMap &trans=table[state];// ссылка на таблицу 
//                         переходов из состо€ни€ state

    if ((iter=trans.find(*input))==
                     trans.end()) break;// нет перехода

    state = iter->second; //новое состо€ние
    ++accepted;
    ++input;
  }//конец цикла
//          состо€ние не заключительное?
  return(finals.count(state)==0)? 0 : accepted;
}

  inline void addstr(tFSM& fsm,
              tFSM::tState from, const tFSM::tSymbol *str,
              tFSM::tState to){
   for(; *str; ++str) fsm.add(from, *str, to);
  }

  inline void addrange(tFSM& fsm,
                tFSM::tState from, tFSM::tSymbol first,
                tFSM::tSymbol last, tFSM::tState to){
   for(tFSM::tSymbol i=first; i<=last; ++i) fsm.add(from, i, to);
  }

#endif

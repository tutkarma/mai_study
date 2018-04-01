//                 basegramma.cpp 2017
#include "basegramma.h"
#include <iomanip>
using namespace std;

void tBaseGramma::reverse(tSymbstr& s)
{
 size_t n=s.length(), nd=n-1, nh=n/2;
 for(size_t i=0; i<nh; ++i){
	tSymb  c = s[i];
		s[i] = s[nd-i];
	s[nd-i] = c;
	}
}

void out_chain(std::ostream& out, const tBaseGramma& gr,
               const tBaseGramma::tSymbstr& rp)
{
    for(size_t i=0; i<rp.length(); ++i)
                  out << " " << gr.decode(rp[i]);
}

void out_prod(std::ostream& out, const tBaseGramma& gr,
           const tBaseGramma::tRule& rule){
  const tBaseGramma::tAlt& alt = gr.getAlt(rule);
  size_t maxw = gr.smbWidth();
  if(maxw<8) maxw=8;
  out << setw(maxw) << gr.decode(rule.left)
        <<" -> ";
  for( size_t i=0; i < alt.rp.length(); ++i)
        out << gr.decode(alt.rp[i]) <<" ";
  out << gr.decode(1) << alt.hndl;
  out << endl; 
}

void outgr(std::ostream& out, const tBaseGramma& gr)
{
  if(!gr){ out << gr.getError() << endl; return;};

 size_t wmax=gr.smbWidth();
 out << gr.decode(1)<<" " << gr.decode(2)<< endl;
 for (size_t i=3,lc=0; i<gr.getStart(); ++i){
   out<< setw(wmax)<<gr.decode(i) <<" ";
   if(++lc>3) {lc=0; out<<endl;};
   }
   out << "\n" << gr.decode(1)<< endl;

 for (size_t left=gr.getStart(); left<gr.ABCsize()
                               ; ++left){
   string sleft = gr.decode(left);
   out<< setw(wmax)<<sleft <<" ->";
   size_t ac = gr.altCount(left), ac1 = ac - 1;
   for(size_t ialt=0; ialt<ac; ++ialt){
    const tBaseGramma::tAlt& alt =
       gr.getAlt(tBaseGramma::tRule(left,ialt));
    out_chain(out,gr,alt.rp);
    int d= alt.hndl;
    if(d) out << " " << gr.decode(1)<< d;
    if(ialt != ac1) out << " |\n"<<
                  setw(wmax)<<string()<<"   ";
   }
   out << endl;
  }
   out << endl;
}

void enumgr(std::ostream& out, const tBaseGramma& gr)
{
  if(!gr){ out << gr.getError() << endl; return;};

 size_t wmax=gr.smbWidth();
 out << gr.decode(1)<<" " << gr.decode(2)<< endl;
 for (size_t i=3,lc=0; i<gr.getStart(); ++i){
   out<< setw(wmax)<<gr.decode(i) <<" ";
   if(++lc>3) {lc=0; out<<endl;};
   }
   out << "\n" << gr.decode(1)<< endl;

 int count=0;
 for (size_t left=gr.getStart(); left<gr.ABCsize()
                               ; ++left){
   string sleft = gr.decode(left);
   out<< setw(wmax)<<sleft <<" ->";
   size_t ac = gr.altCount(left), ac1 = ac - 1;
   for(size_t ialt=0; ialt<ac; ++ialt){
    const tBaseGramma::tSymbstr& rp =
                          gr.rightPart(left,ialt);
    out_chain(out,gr,rp);
    int d= count>998 ? 0 : ++count;
     out << " " << gr.decode(1)<< d;
    if(ialt != ac1) out << " |\n"<<
                  setw(wmax)<<string()<<"   ";
   }
   out << endl;
  }
   out << endl;
}
//  Только для НЕУКОРАЧИВАЮЩИХ грамматик
void tBaseGramma::addunique(tSymbstr& strset, tSymb a){
 if (strset.find_first_of(a) == tSymbstr::npos)
                                     strset +=a;
}

void tBaseGramma::add_to_first(tSymb a,
                               tSymbstr& first)const
{// a нетерминал
  for(size_t ialt=0; ialt<altCount(a); ++ialt )
            addunique(first, rightPart(a,ialt)[0]);
}

void tBaseGramma::createfirst(tSymb a,
                              tSymbstr& first)const
{
  first.clear();
  if(terminal(a)){first+=a; return;}
  add_to_first(a, first);
  for(size_t i=0; i<first.length(); ++i )
     if(!terminal(first[i]))
                add_to_first(first[i],first);
}

void tBaseGramma::add_to_last(tSymb a,
                              tSymbstr& last)const
{// a нетерминал
  for(size_t ialt=0; ialt<altCount(a); ++ialt )
   {
     const tSymbstr &rp = rightPart(a,ialt);
     addunique(last, rp[rp.length()-1]);
   }
}

void tBaseGramma::createlast(tSymb a,
                         tSymbstr& last)const
{
  last.clear();
  if(terminal(a)) return;
  add_to_last(a, last);
  for(size_t i=0; i<last.length(); ++i )
     if(!terminal(last[i]))
                  add_to_last(last[i],last);
}

void tBaseGramma::makeFirst(tSymbstrset& smbs)const{
  smbs.clear();
  size_t sz = ABCsize();
  smbs.resize(sz);
  size_t start = getStart();
  for(size_t i=0; i<start; ++i)
            smbs[i] = tSymbstr(1, i);
  tSymbstr temp;
  temp.reserve(sz);
  for(size_t left=start; left<sz; ++left){
   createfirst(left, temp);
   smbs[left] = temp;
  }
}

void tBaseGramma::makeLast(tSymbstrset& smbs)const{
  smbs.clear();
  size_t sz = ABCsize();
  smbs.resize(sz);
  size_t start = getStart();
  tSymbstr temp;
  temp.reserve(sz);
  for(size_t left=start; left<sz; ++left){
   createlast(left, temp);
   smbs[left] = temp;
  }
}

void
  tBaseGramma::makeFollow(tSymbstrset& follow)const{
  follow.clear();
  size_t sz = ABCsize();
  follow.resize(sz);
  size_t start = getStart();
  tSymbstrset firsts, lasts;
  makeFirst(firsts);
  makeLast(lasts);

// цикл перебора всех пар, соседствующих в правых частях
   for (size_t left=start; left<sz; ++left){
     for(size_t ialt=0; ialt<altCount(left); ++ialt){
      const tSymbstr& rp=rightPart(left,ialt);
      for (size_t i=1; i<rp.length(); ++i){
       tSymb x=rp[i-1];
//
       if(terminal(x)) continue;
       tSymb y=rp[i];
//
       const tSymbstr& last_x = lasts[x];
       const tSymbstr& first_y = firsts[y];
       for(size_t k=0; k< first_y.length(); ++k){
        tSymb b=first_y[k];
//
        if(!terminal(b)) continue;
        addunique(follow[x], b);
        for(size_t j=0; j< last_x.length(); ++j){
         tSymb a=last_x[j];
//
         if(terminal(a)) continue;
         addunique(follow[a], b);
        }//for j
       }//for k
      }//for i
     }//for ialt
   }//for left

  const tSymb marker=1;
  addunique(follow[start], marker);
//äîáàâèòü marker â FOLLOW äëÿ âñåõ íåòåðìèíàëîâ èç
//           LAST(start)
 const tSymbstr& last_start = lasts[start];
 for(size_t i=0; i<last_start.length(); ++i){
  tSymb x = last_start[i];
  if(!terminal(x))
            addunique(follow[x], marker);
 }
}


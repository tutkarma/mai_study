//coin.cpp
#include "mlisp.h"

double cc(double amount, double largest__coin);
double count__change(double amount);
double next__coin(double coin);
double GR__AMOUNT();
extern double LAGEST__COIN;

double LAGEST__COIN=10.;
double dd = 2.;
double mm = 12.;

double cc(double amount, double largest__coin){
{return
 (  (amount == 0 || largest__coin == 1.) ? 1.
  : !(amount  > 0 && largest__coin > 0) ? 0
  : (cc(amount, next__coin(largest__coin)) +
     cc(amount - largest__coin, largest__coin)
    )
 );
}}

double count__change(double amount){
{return
 cc(amount, LAGEST__COIN);
}}

double next__coin(double coin){
{return 
  ( coin ==10. ? 5.
  : coin == 5. ? 3.
  : coin == 3. ? 2.
  : coin == 2. ? 1.
  : 0
 );
}}

double GR__AMOUNT(){{
  return mm * 100. + dd;
}}


int main(){
 display(" AAD variant 6");newline();
 display(" 1-2-3-5-10");newline();
 display("count__change for 100 \t= ");
 display(count__change(100.));newline();
 display("count__change for ");
 display(GR__AMOUNT());
 display(" \t= ");
 display(count__change(GR__AMOUNT()));newline();

 std::cin.get();
 return 0;
}
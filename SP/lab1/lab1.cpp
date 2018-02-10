/*
(define(even-bits n)
  (cond((= n 0)1)
       ((=(remainder n 2)0)
          (even-bits (quotient n 2)))
       (else(odd-bits(quotient n 2)))
       ))
(define(odd-bits n)
  (cond((= n 0)0)
       ((=(remainder n 2)0)
          (odd-bits (quotient n 2)))
       (#t(even-bits(quotient n 2)))
       ))
(define(display-bin n)
  (display(remainder n 2))
  (if(= n 0)0 (display-bin (quotient n 2)))
       )
(define(report-results n)
  (display "Happy birthday to you!\n\t")
  (display n)(display " (decimal)\n\t")
  (display-bin n)(display "(reversed binary)\n")
  (display "\teven?\t")(display (if(=(even-bits n)1) "yes" "no"))
  (newline)
  (display "\todd?\t")(display (if(=(odd-bits n)1) "yes" "no"))
  (newline)
  0
       )
;***** Date of YOUR birthday *******
(define dd 31)
(define mm 12)
(define yyyy 2018)
;***********************************
(report-results (+ (* dd 1000000)
                   (* mm 10000)
                   yyyy))
*/
#include "mlisp.h"

double odd__bits(double n);
double even__bits(double n);
double display__bin(double n);
double report__results(double n);

double dd = 2;
double mm = 12;
double yyyy = 1997;

double odd__bits(double n)
 {
  return (n == 0 ? 0
       : remainder(n,2) == 0 ?
                odd__bits(quotient(n,2))
       : true ? even__bits(quotient(n,2))
       : _infinity);
 }
 
double even__bits(double n)
{
 	return (n == 0 ? 1
	 : remainder(n,2) == 0 ?
	 			even__bits(quotient(n,2))
	 : odd__bits(quotient(n,2)));
}

double display__bin(double n)
{
	display(remainder(n,2));
	return (n == 0 ? 0 : display__bin(quotient(n,2)));	
}

double report__results(double n)
{
	display("Happy birthday to you!\n\t");
	display(n);
	display(" (decimal)\n\t");
	display__bin(n);
	display("(reversed binary)\n");
	display("\teven?\t");
	display(even__bits(n) == 1 ? "yes" : "no");
	newline();
	display("\todd?\t");
	display(odd__bits(n) == 1 ? "yes" : "no");
	newline();
	return 0;
}

int main(){
 display(report__results (dd*1000000+
                         mm*10000+
                         yyyy));
 newline();

 std::cin.get();
 return 0;
}

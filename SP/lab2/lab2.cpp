//  half-interval.cpp 2018
/*
; replace with YOUR definition of fun !
(define(fun z)
  (set! z (- z (/ 106 107)(/ e)))
  (+ (* 0.25 (expt z 3))
   (- z 1.2502)
  )
)
*/
#include "mlisp.h"
//...
double half__interval__metod(double a, double b);
double __AAD__try(double neg__point, double pos__point);
bool close_enough_Q(double x, double y);
double average(double x, double y);
double root(double a, double b);
double fun(double z);

double tolerance = 0.00001;

double half__interval__metod(double a, double b)
{
    {//let
    double a__value = fun(a);
    double b__value = fun(b);
    return (a__value < 0) && (b__value > 0) ? __AAD__try(a, b)
            : (a__value > 0) && (b__value < 0) ? __AAD__try(b, a) 
            : b + 1.;
    }//let   
}

double __AAD__try(double neg__point, double pos__point)
{
    {//let
    double midpoint = average(neg__point, pos__point);
    double test__value = 0;
    display("+");
    return (close_enough_Q(neg__point, pos__point) ? midpoint
        : true ?
                test__value = fun(midpoint),
                test__value > 0 ? __AAD__try(neg__point, midpoint) :
                test__value < 0 ? __AAD__try(midpoint, pos__point) :
                midpoint : _infinity
           );
    }//let
}


bool close_enough_Q(double x, double y)
{
    return abs(x - y) < tolerance;
}

double average(double x, double y)
{
    return double(x + y) / 2.;
}

double root(double a, double b)
{
    display("interval=\t[");
    display(a);
    display(" , ");
    display(b);
    display("]\n");
    {//let
    double temp = half__interval__metod(a, b);
    newline();
    display("discrepancy=\t");
    display(fun(temp));newline();
    display("root=\t\t");
    display((temp - b - 1.) == 0 ? "[bad]" : "[good]");
    return temp;
    }//let
}

double fun(double z)
{
    z = z - double(106) / 107 - double(1)/e;
    return 0.25 * expt(z, 3) + z - 1.2502;
}

int main(){
    display("\" AAD variant 6\""); newline();
    display(root(2, 3)); newline();
    std::cin.get();
}

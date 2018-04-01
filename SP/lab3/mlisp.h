/*  mlisp.h 2017  */
#include<iostream>
#include<cmath>
#include<limits>
  const double _infinity=
     std::numeric_limits<double>::infinity();
  const double pi= 4.*std::atan(1.);
  const double e = std::exp(1.);
  inline double display(double x){
     std::cout.precision(16);
     std::cout<<x;return 0;}
  inline double display(int x){
     std::cout<<x;return 0;}
  inline double display(char x){
     std::cout<<x;return 0;}
  inline double display(const char* x){
     std::cout<<x;return 0;}
  inline double display(bool x){
     std::cout<<'#'<<(x ? 't' : 'f');return 0;}
  inline double newline(){
    std::cout<< std::endl;return 0;}
  inline double remainder(double x, double y)
    { return std::fmod(x, y);}
  inline double expt(double x, double y)
    { return std::pow(x, y);}
  inline double abs(double x){ return std::fabs(x);}
using std::sin;using std::cos;
using std::tan;using std::exp;
using std::log;using std::atan;



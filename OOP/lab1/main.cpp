#include <cstdlib>
#include <cstring>
#include "trapeze.h"
#include "rhomb.h"
#include "pentagon.h"

int main(void)
{
    const int size = 16;
    char s[size];
    std::cout << "Use 'help' or 'h' to get help." << std::endl;
    Trapeze *ptr_tr = nullptr;
    Rhomb *ptr_rh = nullptr;
    Pentagon *ptr_pen = nullptr;
    while (1) {
        std::cin.getline(s, size);
        std::cin.clear();
        std::cin.sync();
        if (!strcmp(s, "create_trapeze") || !strcmp(s, "cr_tr")) {
            if (ptr_tr != nullptr) delete ptr_tr;
            std::cout << "Enter smaller base, bigger base, left side and right side." << std::endl;
            ptr_tr = new Trapeze(std::cin);
        } else if (!strcmp(s, "create_rhomb") || !strcmp(s, "cr_rh")) {
            if (ptr_rh != nullptr) delete ptr_rh;
            std::cout << "Enter side and smaller angle." << std::endl;
            ptr_rh = new Rhomb(std::cin);
        } else if (!strcmp(s, "create_pentagon") || !strcmp(s, "cr_pen")) {
            if (ptr_pen != nullptr) delete ptr_pen;
            std::cout << "Enter side." << std::endl;
            ptr_pen = new Pentagon(std::cin);
        } else if (!strcmp(s, "print_trapeze") || !strcmp(s, "pr_tr")) {
            if(ptr_tr == nullptr) {
                std::cout << "Trapeze doesn't exist." << std::endl;
            } else {
                ptr_tr->Print();
            }
        } else if (!strcmp(s, "print_rhomb") || !strcmp(s, "pr_rh")) {
            if(ptr_rh == nullptr) {
                std::cout << "Rhomb doesn't exist." << std::endl;
            } else {
                ptr_rh->Print();
            }
        } else if (!strcmp(s, "print_pentagon") || !strcmp(s, "pr_pen")) {
            if(ptr_pen == nullptr) {
                std::cout << "Pentagon doesn't exist." << std::endl;
            } else {
                ptr_pen->Print();
            }
        } else if (!strcmp(s, "square_trapeze") || !strcmp(s, "sq_tr")) {
            if(ptr_tr == nullptr) {
                std::cout << "Trapeze doesn't exist." << std::endl;
            } else {
                std::cout << "Square: " << ptr_tr->Square() << std::endl;
            }
        } else if (!strcmp(s, "square_rhomb") || !strcmp(s, "sq_rh")) {
            if(ptr_rh == nullptr) {
                std::cout << "Rhomb doesn't exist." << std::endl;
            } else {
                std::cout << "Square: " << ptr_rh->Square() << std::endl;
            }
        } else if (!strcmp(s, "square_pentagon") || !strcmp(s, "sq_pen")) {
            if(ptr_pen == nullptr) {
                std::cout << "Pentagon doesn't exist." << std::endl;
            } else {
                std::cout << "Square: " << ptr_pen->Square() << std::endl;
            }
        } else if (!strcmp(s, "quit") || !strcmp(s, "exit") || !strcmp(s, "q")) {
            if (ptr_tr != nullptr) {
                delete ptr_tr;
                std::cout << "Trapeze deleted " << std::endl;
            }
            if (ptr_rh != nullptr) {
                delete ptr_rh;
                std::cout << "Rhomb deleted " << std::endl;
            }
            if (ptr_pen != nullptr) {
                delete ptr_pen;
                std::cout << "Pentagon deleted " << std::endl;
            }
            break;
        } else if (!strcmp(s, "help") || !strcmp(s, "h")) {
            std::cout << "Commands 'create_trapeze' and 'cr_tr' create new trapeze with your parameters." << std::endl;
            std::cout << "Commands 'create_rhomb' and 'cr_rh' create new rhomb with your parameters." << std::endl;
            std::cout << "Commands 'create_pentagon' and 'cr_pen' create new pentagon with your parameters." << std::endl;
            std::cout << "Commands 'print_trapeze' and 'pr_tr' output parameters of trapeze." << std::endl;
            std::cout << "Commands 'print_rhomb' and 'pr_rh' output parameters of rhomb."  << std::endl;
            std::cout << "Commands 'print_pentagon' and 'pr_pen' output parameters of pentagon."  << std::endl;
            std::cout << "Commands 'square_trapeze' and 'sq_tr' output square of trapeze." << std::endl;
            std::cout << "Commands 'square_rhomb' and 'sq_rh' output square of rhomb." << std::endl;
            std::cout << "Commands 'square_pentagon' and 'sq_pen' output square of pentagon." << std::endl;
            std::cout << "Commands 'quit' and 'q' exit the program." << std::endl;
        }
    }
    return 0;
}

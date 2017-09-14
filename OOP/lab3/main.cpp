#include <iostream>
#include <memory>
#include <cstdlib>
#include <cstring>
#include "trapeze.h"
#include "rhomb.h"
#include "pentagon.h"
#include "TList.h"

void menu()
{
    std::cout << "Choose an operation:" << std::endl;
    std::cout << "1) Add trapeze" << std::endl;
    std::cout << "2) Add rhomb" << std::endl;
    std::cout << "3) Add pentagon" << std::endl;
    std::cout << "4) Delete figure from list" << std::endl;
    std::cout << "5) Print list"  << std::endl;
    std::cout << "0) Exit" << std::endl;
}

int main(void)
{
    int32_t act = 0;
    TList list;
    std::shared_ptr<Figure> ptr;
    do {
        menu();
        std::cin >> act;
        switch(act) {
            case 1:
                ptr = std::shared_ptr<Figure>(new Trapeze(std::cin));
                list.Push(ptr);
                break;
            case 2:
                ptr = std::shared_ptr<Figure>(new Rhomb(std::cin));
                list.Push(ptr);
                break;
            case 3:
                ptr = std::shared_ptr<Figure>(new Pentagon(std::cin));
                list.Push(ptr);
                break;
            case 4:
                ptr = list.Pop();
                std::cout << ptr.use_count() << std::endl;
                break;
            case 5:
                std::cout << list << std::endl;
                break;
            case 0:
                break;
            default:
                std::cout << "Incorrect command" << std::endl;;
                break;
        }
    } while (act);
    return 0;
}

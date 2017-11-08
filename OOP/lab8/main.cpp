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
    std::cout << "5) Sort list"  << std::endl;
    std::cout << "6) Print list"  << std::endl;
    std::cout << "0) Exit" << std::endl;
}

int main(void)
{
    int32_t act = 0;
    TList<Figure> list;
    std::shared_ptr<Figure> ptr;
    do {
        menu();
        std::cin >> act;
        switch(act) {
            case 1:
                ptr = std::make_shared<Trapeze>(std::cin);
                list.Push(ptr);
                break;
            case 2:
                ptr = std::make_shared<Rhomb>(std::cin);
                list.Push(ptr);
                break;
            case 3:
                ptr = std::make_shared<Pentagon>(std::cin);
                list.Push(ptr);
                break;
            case 4:
                ptr = list.Pop();
                break;
            case 5:
                std::cout << "1 to regular sort, 2 to parallel" << std::endl;
                std::cin >> act;
                if (act == 1) {
                    list.Sort();
                } else if (act == 2) {
                    list.ParSort();
                } else {
                    std::cout << "Unknown command" << std::endl;
                    break;
                }
                std::cout << list << std::endl;
                break;
            case 6:
                if(!list.IsEmpty()) {
                    std::cout << list << std::endl;
                } else {
                    std::cout << "List is empty." << std::endl;
                }
                break;
            case 0:
                list.Del();
                break;
            default:
                std::cout << "Incorrect command" << std::endl;;
                break;
        }
    } while (act);
    return 0;
}

#include "TList.h"
#include <iostream>
#include "trapeze.h"
#include "pentagon.h"
#include "rhomb.h"
#include "TStack.h"

void menu(void)
{
    std::cout << "Choose an operation:" << std::endl;
    std::cout << "1) Add trapeze" << std::endl;
    std::cout << "2) Add rhomb" << std::endl;
    std::cout << "3) Add pentagon" << std::endl;
    std::cout << "4) Delete by criteria" << std::endl;
    std::cout << "5) Print"  << std::endl;
    std::cout << "0) Exit" << std::endl;
}

int main(void)
{
    TStack<TList<Figure>, std::shared_ptr<Figure> > stack;
    int act, index;
    Trapeze tmp1;
    Rhomb tmp2;
    Pentagon tmp3;

    do {
        menu();
        std::cin >> act;
        switch(act) {
            case 1:
                std::cin >> tmp1;
                stack.push(std::make_shared<Trapeze>(tmp1));
                std::cout << "Item was added" << std::endl;
                break;
            case 2:
                tmp2.setParams(std::cin);
                stack.push(std::make_shared<Rhomb>(tmp2));
                std::cout << "Item was added" << std::endl;
                break;
            case 3:
                tmp3.setParams(std::cin);
                stack.push(std::make_shared<Pentagon>(tmp3));
                std::cout << "Item was added" << std::endl;
                break;
            case 4:{
                std::cout << "Enter criteria" << std::endl;
                std::cout << "1) by type\n2) lesser than square\n";
                std::cin >> index;
                if (index == 1) {
                    std::cout << "1) trapeze\n2) rhomb\n3) pentagon\n";
                    std::cout << "Enter type" << std::endl;
                    std::cin >> index;
                    stack.removeByType(index);
                } else if (index == 2) {
                    double sqr = 0.0;
                    std::cout << "Enter square" << std::endl;
                    std::cin >> sqr;
                    stack.removeLesser(sqr);
                } else {
                    break;
                }
                break;
            }
            case 5:
                stack.print();
                break;
            case 0:
                break;
            default:
                std::cout << "Incorrect command" << std::endl;;
                break;
        }

    } while(act);

    return 0;
}

#ifndef UNTITLED1_MULTIMAP_H
#define UNTITLED1_MULTIMAP_H
#include <iostream>
#include <map>
#include <time.h>
#include <math.h>
#include <random>
#include <algorithm>
#include "String.h"
#include "cstring"

void analyzeMultimapOperations(int numOper) {
    std::multimap<int, int> mm;
    std::multimap<int, int>::iterator Iter;
    clock_t t = clock();
    for (int i = 0; i < numOper; i++) {
        int key = rand() % 100;
        int num = rand() % 100;
        mm.insert({key, num});
    }
    std::cout << "Add elems to multimap (int): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;

    t = clock();
    int key = rand() % 100;
    int num = rand() % 100;
    mm.insert({key, num});//добавляем
    std::cout << "Add random elem to multimap (int): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;

    t = clock();
    auto itr = mm.find(key);//находим эл с ключом
    std::cout << "Find elem in multimap (int): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;

    t = clock();
    mm.erase(itr);//удаляем элемент на который указывает итератор
    std::cout << "Delete elem in multimap (int): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;


}


void analyzeMultimapOperationsString(int numOper) {
    std::multimap<int, String> mm;
    std::multimap<int, String>::iterator Iter;
    clock_t t = clock();
    for (int i = 0; i < numOper; i++) {
        int key = rand() % 100;
        int num = rand() % 100;
        std::string newString = "Str " + std::to_string(num);
        mm.insert({key, newString.c_str()});
    }
    std::cout << "Add elems to multimap (String): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;

    t = clock();
    int key = rand() % 100;
    int num = rand() % 100;
    std::string newString = "Str " + std::to_string(num);
    mm.insert({key, newString.c_str()});
    std::cout << "Add random elem to multimap (String): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;

    t = clock();
    auto itr = mm.find(key);
    std::cout << "Find elem in multimap (String): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;

    t = clock();
    mm.erase(itr);
    std::cout << "Delete elem in multimap (String): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;

}
#endif //UNTITLED1_MULTIMAP_H

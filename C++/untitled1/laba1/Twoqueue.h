#ifndef UNTITLED1_TWOQUEUE_H
#define UNTITLED1_TWOQUEUE_H
#include <iostream>
#include <deque>
#include <time.h>
#include <math.h>
#include <random>
#include <algorithm>
#include "String.h"
#include "cstring"
//для int
void analyzeDequeOperations(int numOperations) {
    std::deque<int> dq;//контейнер
    std::deque<int>::iterator Iter;//объявляем итератор
    clock_t t = clock();//фиксация времени
    for (int i = 0; i < numOperations; i++) {
        int num = rand() % 100;
        dq.push_back(num);
    }
    std::cout << "Add elems to deque (int): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;
//итератор со случайной позицией
    t = clock();
    auto iter = ++dq.cbegin() + (rand() % 998);//сдвигаем позицию и добовляем к генератору рандомное значение
    dq.insert(iter, 1000);//вставляем в случайную позицию
    std::cout << "Add random elem to deque (int): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;



    t = clock();
    std::sort(dq.begin(), dq.end());//сортируем
    std::cout << "Sort elems in deque (int): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;

    t = clock();
    Iter = std::find(dq.begin(), dq.end(),98);//проходимся по всем и проверяем равен ли нашему числу
    std::cout << "Find elem in deque (int): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;

    t = clock();
    dq.erase(iter);
    std::cout << "Delete elem in deque (int): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;
}
//для работы со String
void analyzeDequeOperationsString(int numOperations) {
    std::deque<String> dq;
    std::deque<String>::iterator Iter;
    clock_t t = clock();
    for (int i = 0; i < numOperations; i++) {
        int num = rand() % 100;
        std::string newString = "Str " + std::to_string(num);//преобразуем число в str
        dq.emplace_back(newString.c_str());  // Передаем C-строку
    }
    std::cout << "Add elems to deque (String): " << (clock() - t + .0 ) / CLOCKS_PER_SEC << std::endl;

    t = clock();
    auto iter1 = ++dq.cbegin() + (rand() % 998);//сдвигаем итераттор на случайное значение от2 до 997
    const String str1("qqqq");
    dq.insert(iter1, str1);
    std::cout << "Add random elem to deque (String): " << (clock() - t + .0 ) / CLOCKS_PER_SEC << std::endl;


    t = clock();
    std::sort(dq.begin(), dq.end());//от большего к меньшему с помощью ASCLl
    std::cout << "Sort elems in deque (String): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;


    t = clock();
    const String str("Str 10");
    auto iter = std::find_if(dq.begin(), dq.end(), [&str](const String& s) {//текущий сравниваем с нашим
        return s == str;
    });
    std::cout << "Find elem in deque (String): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;

    t = clock();
    dq.erase(iter);
    std::cout << "Delete elem in deque (String): " << (clock() - t + .0) / CLOCKS_PER_SEC << std::endl;
}
#endif //UNTITLED1_TWOQUEUE_H

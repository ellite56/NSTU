#include <iostream>
#include <cstring>
#include <fstream>
#include "String.h"
#include "BinaryString.h"
#include "ToDoList.h"
#include "List.h"
#include "Exeption.h"
#include "test.h"


void testConstructor() {
    String str1 = String();
    const char *str2 = " 0 10";
    if (strcmp(str1.toString(), str2) == 0) {
        std::cout << "testConstructor: Done" << std::endl;
    }
    else {
        std::cout << "testConstructor: Failed " << "Your: " << str2 << "Programm: " << str1.toString() << std::endl;
    }
}

void testConstructorWithParam1() {
    String str1("Hello");
    const char* str2 = "Hello 5 6";
    if (strcmp(str1.toString(), str2) == 0) {
        std::cout << "testConstructorWithParam1: Done" << std::endl;
    }
    else {
        std::cout << "testConstructorWithParam1: Failed \n" << "Your: " << str2 << "\nProgramm: " << str1.toString() << std::endl;
    }
}

void testConstructorWithParam2() {
    String str1("");
    const char* str2 = " 0 1";
    if (strcmp(str1.toString(), str2) == 0) {
        std::cout << "testConstructorWithParam2: Done" << std::endl;
    }
    else {
        std::cout << "testConstructorWithParam2: Failed \n" << "Your: " << str2 << "\nProgramm: " << str1.toString() << std::endl;
    }
}

void testConstructorCopy() {
    String str = String("apple");
    String str1(str);
    const char *str2 = "apple 5 6";
    if (strcmp(str1.toString(), str2) == 0) {
        std::cout << "testConstructorCopy: Done" << std::endl;
    }
    else {
        std::cout << "testConstructorCopy: Failed \n" << "Your: " << str2 << "\nProgramm: " << str1.toString() << std::endl;
    }
}

void testSetStr1() {
    String str1 = String("apple");
    str1.setStr("four");
    const char *str2 = "four 4 6";
    if (strcmp(str1.toString(), str2) == 0) {
        std::cout << "testSetStr: Done" << std::endl;
    }
    else {
        std::cout << "testSetStr: Failed \n" << "Your: " << str2 << "\nProgramm: " << str1.toString() << std::endl;
    }
}

void testSetStr2() {
    String str1 = String("apple");
    str1.setStr("Counter Strike");
    const char *str2 = "Counter Strike 14 24";
    if (strcmp(str1.toString(), str2) == 0) {
        std::cout << "testSetStr: Done" << std::endl;
    }
    else {
        std::cout << "testSetStr: Failed \n" << "Your: " << str2 << "\nProgramm: " << str1.toString() << std::endl;
    }
}

void testSearchStr1() {
    String str1 = String("apple");
    const char *str2 = "pl";
    int myInt = 2;
    if (str1.searchStr(str2) == myInt) {
        std::cout << "testSearchStr: Done" << std::endl;
    }
    else {
        std::cout << "testSearchStr: Failed \n" << "Your: " << myInt << "\nProgramm: " << str1.searchStr(str2) << std::endl;
    }
}

void testSearchStr2() {
    String str1 = String("apple");
    const char *str2 = "pla";
    int myInt = -1;
    if (str1.searchStr(str2) == myInt) {
        std::cout << "testSearchStr: Done" << std::endl;
    }
    else {
        std::cout << "testSearchStr: Failed \n" << "Your: " << myInt << "\nProgramm: " << str1.searchStr(str2) << std::endl;
    }
}

void testStaticFunction() {
    String str1 = String("apple");
    int myInt = 1;
    if (String::objectCount() == myInt) {
        std::cout << "testStaticFunction: Done" << std::endl;
    }
    else {
        std::cout << "testStaticFunction: Failed \n" << "Your: " << myInt << "\nProgramm: " << String::objectCount() << std::endl;
    }
}







void testOperatorPlCh() {
    String str1 = String("apple");
    const char* str2 = " dd";
    String str3 = str1 + str2;
    String strAnswer("apple dd");
    if (strcmp(str3.toString(), strAnswer.toString()) == 0) {
        std::cout << "testOperatorPlusChar: Done" << std::endl;
    }
    else {
        std::cout << "testOperatorPlusChar: Failed \n" << "Your: " << strAnswer.toString() << "\nProgramm: " << str3.toString() << std::endl;
    }
}

void testOperatorPlObj() {
    String str1 = String("apple");
    String str2 = String("boy");
    String str3 = str1 + str2;
    String strAnswer("appleboy");
    if (strcmp(str3.toString(), strAnswer.toString()) == 0) {
        std::cout << "testOperatorPlusObject: Done" << std::endl;
    }
    else {
        std::cout << "testOperatorPlusObject: Failed \n" << "Your: " << strAnswer.toString() << "\nProgramm: " << str3.toString() << std::endl;
    }
}

void testOperatorMinCh() {
    String str1 = String("apple");
    const char* str2 = "pl";
    String str3 = str1 - str2;
    String strAnswer("ape");
    if (strcmp(str3.toString(), strAnswer.toString()) == 0) {
        std::cout << "testOperatorMinusChar: Done" << std::endl;
    }
    else {
        std::cout << "testOperatorMinusChar: Failed \n" << "Your: " << strAnswer.toString() << "\nProgramm: " << str3.toString() << std::endl;
    }
}

void testOperatorIndex1() {
    String str1 = String("apple");
    const char str2 = str1[0];
    const char strAnswer = 'a';
    if (str2 == strAnswer) {
        std::cout << "testOperatorIndex1: Done" << std::endl;
    }
    else {
        std::cout << "testOperatorIndex1: Failed \n" << "Your: " << strAnswer << "\nProgramm: " << str2 << std::endl;
    }
}

void testOperatorIndex2() {
    String str1 = String("apple");
    str1[0] = 'p';
    const char str2 = str1[0];
    const char strAnswer = 'p';
    if (str2 == strAnswer) {
        std::cout << "testOperatorIndex2: Done" << std::endl;
    }
    else {
        std::cout << "testOperatorIndex2: Failed \n" << "Your: " << strAnswer << "\nProgramm: " << str2 << std::endl;
    }
}

void testOperatorAssign() {
    String str1("Hello");
    String str2("banana");
    String str3("apple");
    str2 = str1 = str3;
    if (strcmp(str2.toString(), str3.toString()) == 0) {
        std::cout << "testOperatorAssign: Done" << std::endl;
    }
    else {
        std::cout << "testOperatorAssign: Failed \n" << "Your: " << str3.toString() << "\nProgramm: " << str2.toString() << std::endl;
    }
}





// - 3 -
void testOstream() {
    String str1("Hello");
    std::cout << str1 << std::endl;
}

void testIstream() {
    String str1 = String();
    std::cin >> str1;
    std::cout << str1 << std::endl;
}

void testTextFile() {
    String str("Hello");
    std::ofstream ofs("text.txt", std::ios::app);
    if (!ofs){
        std::cerr << "Error: unable to write to text.txt " << std::endl;
        exit(1);
    }
    ofs << str << std::endl;
    ofs.close();

    std::ifstream ifs("text.txt");
    if (!ifs){
        std::cerr << "Error: unable to write to text.txt " << std::endl;
        exit(1);
    }

    if (ifs.is_open())
    {
        String _str = String("");
        while (ifs >> _str)
        {
            if (strcmp(str.getStr(), _str.getStr()) == 0) {
                std::cout << "testTextFileOne: Done" << std::endl;
            }
            else {
                std::cout << "testTextFileOne: Failed \n" << "Your: " << str << "\nProgramm: " << _str << std::endl;
            }
        }
    }
    ifs.close();
};

void testBinaryFileOne() {
    String str("Hello");

    std::ofstream ofs("employee.dat", std::ios::out | std::ios::binary) ;
    if (ofs.is_open()) {
        str.serialize(ofs);
    }
    ofs.close();


    String str1;
    std::ifstream ifs("employee.dat", std::ios::in | std::ios::binary);
    if (ifs.is_open()) {
        str1.deserialize(ifs);  // Десериализация объекта

    }

    if (strcmp(str.getStr(), str1.getStr()) == 0) {
        std::cout << "testBinaryFileOne: Done" << std::endl;
    }
    else {
        std::cout << "testBinaryFileOne: Failed \n" << "Your: " << str << "\nProgramm: " << str1 << std::endl;
    }
    ifs.close();


}

void testBinaryFileSeveral() {
    String ps[] {{"Ser ge"},
                 {"Bjap"},
                 {"Denie"},
                 {"Kempson"},
    };

    std::ofstream ofs("out_course.dat", std::ios::out | std::ios::binary);
    if(ofs.is_open()) {
        for(auto& p : ps)
            p.serialize(ofs);
    }
    ofs.close();

    String ps_r[10];
    int count = 0;

    std::ifstream ifs("out_course.dat", std::ios::in | std::ios::binary);
    if(ifs.is_open()) {
        while(count < 10) {
            ps_r[count].deserialize(ifs);
            count++;
        }
    }
    ifs.close();

    char* answer[4] = {"Ser ge", "Bjap", "Denie", "Kempson"};
    int trueFasleAnswer = 0;
    for(int i = 0; i < 4; i++) {
        if (strcmp(ps_r[i].getStr(), answer[i]) == 0) {
            trueFasleAnswer = 1;
        }
        else {
            trueFasleAnswer = 0;
            break;
        }
    }

    if (trueFasleAnswer == 1) {
        std::cout << "testBinaryFileSeveral: Done" << std::endl;
    }
    else {
        std::cout << "testBinaryFileSeveral: Failed \n" << "Your: " << answer[1] << "\nProgramm: " << ps_r[1] << std::endl;
    }
}




// -- 4 Наследование в BinaryString --

void testConstBinaryStr() {
    BinaryString str1 = BinaryString();
    const char *str2 = " 0 10";
    if (strcmp(str1.toString(), str2) == 0) {
        std::cout << "testConstBinaryStr: Done" << std::endl;
    }
    else {
        std::cout << "testConstBinaryStr: Failed " << "Your: " << str2 << "Programm: " << str1.toString() << std::endl;
    }
}

void testConstWithParamBinaryStr() {
    BinaryString str1("0111101001");
    const char* str2 = "0111101001 10 11";
    if (strcmp(str1.toString(), str2) == 0) {
        std::cout << "testConstWithParamBinaryStr: Done" << std::endl;
    }
    else {
        std::cout << "testConstWithParamBinaryStr: Failed \n" << "Your: " << str2 << "\nProgramm: " << str1.toString() << std::endl;
    }
}

void testConstrCopyBinaryString() {
    BinaryString str = BinaryString("01110101011");
    BinaryString str1(str);
    const char *str2 = "01110101011 11 12";
    if (strcmp(str1.toString(), str2) == 0) {
        std::cout << "testConstrCopyBinaryString: Done" << std::endl;
    }
    else {
        std::cout << "testConstrCopyBinaryString: Failed \n" << "Your: " << str2 << "\nProgramm: " << str1.toString() << std::endl;
    }
}

void testDecimalSystemBinaryString() {
    BinaryString str = BinaryString("01110101011");
    int myInt = 939;
    if (str.DecimalSystem() == myInt) {
        std::cout << "testDecimalSystemBinaryString: Done" << std::endl;
    }
    else {
        std::cout << "testDecimalSystemBinaryString: Failed \n" << "Your: " << myInt << "\nProgramm: " << str.DecimalSystem() << std::endl;
    }
}

// -- Наследование в ToDoList --

void testConstrToDoList() {
    ToDoList str1 = ToDoList();
    const char *str2 = " Start:01.01.2000 13:00:00 End:01.01.2000 13:00:00 Status:Not completed";
    if (strcmp(str1.toString(), str2) == 0) {
        std::cout << "testConstrToDoList: Done" << std::endl;
    }
    else {
        std::cout << "testConstrToDoList: Failed " << "Your: " << str2 << "Programm: " << str1.toString() << std::endl;
    }
}

void testConstrrWithParamToDoList() {
    ToDoList str1("Apple", "12.12.2024", "12.12.2024");
    const char* str2 = "Apple Start:12.12.2024 End:12.12.2024 Status:Not completed";
    if (strcmp(str1.toString(), str2) == 0) {
        std::cout << "testConstrrWithParamToDoList: Done" << std::endl;
    }
    else {
        std::cout << "testConstrrWithParamToDoList: Failed \n" << "Your: " << str2 << "\nProgramm: " << str1.toString() << std::endl;
    }
}

void testConstCopyToDoList() {
    ToDoList str("Apple", "12.12.2024", "12.12.2024");
    ToDoList str1(str);
    const char *str2 = "Apple Start:12.12.2024 End:12.12.2024 Status:Not completed";
    if (strcmp(str1.toString(), str2) == 0) {
        std::cout << "testConstCopyToDoList: Done" << std::endl;
    }
    else {
        std::cout << "testConstCopyToDoList: Failed \n" << "Your: " << str2 << "\nProgramm: " << str1.toString() << std::endl;
    }
}

void testMarkCompletedToDoList() {
    ToDoList str("Apple", "12.12.2024", "12.12.2024");
    str.markCompleted();
    const char *str2 = "Apple Start:12.12.2024 End:12.12.2024 Status:Not completed";
    if (strcmp(str.toString(), str2) == 0) {
        std::cout << "testMarkCompletedToDoList: Done" << std::endl;
    }
    else {
        std::cout << "testMarkCompletedToDoList: Failed \n" << "Your: " << str2 << "\nProgramm: " << str.toString() << std::endl;
    }
}
//6 лаба
void testException() {
    try {
        BinaryString str{"11113111000"};
        std::cout << str[40] << std::endl;
    }
    catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << std::endl;
    }
    catch (const binaryError& err)
    {
        std::cout << "Invalid char error: " << err.what() << std::endl;//исключение по бинарности
    }
    catch (const std::out_of_range& messageError) {
        std::cerr << "Out of range error: " << messageError.what() << std::endl;//исключение при выходе за диапазон
    }
    catch (...){
        std::cout<<"error"<<std::endl;
    }

}

// -- 7 --
void testIntStack() {
        // Тест 1: Стек для целых чисел
        std::cout << "Test 1: Stack for integers" << std::endl;
        Stack<int> intStack;

        // Добавляем элементы в стек
        intStack.push(15);
        intStack.push(23);
        intStack.push(38);

        // Отображаем содержимое стека
        std::cout << intStack.toString() << std::endl;

        // Печатаем и удаляем элементы по одному
        std::cout << "Pop element: " << intStack.pop() << std::endl;
        std::cout << intStack.toString() << std::endl;

        std::cout << "Pop element: " << intStack.pop() << std::endl;
        std::cout << intStack.toString() << std::endl;

        std::cout << "Pop element: " << intStack.pop() << std::endl;
        std::cout << "Is stack empty? " << (intStack.isEmpty() ? "Yes" : "No") << std::endl;
}
void testDoubleStack() {
        // Тест 2: Стек для вещественных чисел
        std::cout << "\nTest 2: Stack for doubles" << std::endl;
        Stack<float> doubleStack;

        // Добавляем элементы в стек
        doubleStack.push(3.14f);
        doubleStack.push(5.71f);
        doubleStack.push(1.41f);

        // Отображаем содержимое стека
        std::cout << doubleStack.toString() << std::endl;

        // Печатаем и удаляем элементы по одному
        std::cout << "Pop element: " << doubleStack.pop() << std::endl;
        std::cout << doubleStack.toString() << std::endl;

        std::cout << "Pop element: " << doubleStack.pop() << std::endl;
        std::cout << doubleStack.toString() << std::endl;

        std::cout << "Pop element: " << doubleStack.pop() << std::endl;
        std::cout << "Is stack empty? " << (doubleStack.isEmpty() ? "Yes" : "No") << std::endl;
}

void testStringStack() {
        // Тест 3: Стек для строк (тип String*)
        std::cout << "\nTest 3: Stack for String*" << std::endl;
        Stack<String*> stringStack;

        // Создаем строки и добавляем их в стек
        String* str1 = new String("apple");
        String* str2 = new String("World");
        String* str3 = new String("nstu");

        stringStack.push(str1);
        stringStack.push(str2);
        stringStack.push(str3);

        // Отображаем содержимое стека
        std::cout << stringStack.toString() << std::endl;

        // Печатаем и удаляем элементы по одному
        std::cout << "Pop element: " << *(stringStack.pop()) << std::endl;
        std::cout << stringStack.toString() << std::endl;

        std::cout << "Pop element: " << *(stringStack.pop()) << std::endl;
        std::cout << stringStack.toString() << std::endl;

        std::cout << "Pop element: " << *(stringStack.pop()) << std::endl;
        std::cout << "Is stack empty? " << (stringStack.isEmpty() ? "Yes" : "No") << std::endl;

        // Освобождение памяти для строк
        delete str1;
        delete str2;
        delete str3;
}

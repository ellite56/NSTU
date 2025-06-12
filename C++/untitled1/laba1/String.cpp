#include <iostream>
#include <cstring>
#include <fstream>
#include "String.h"
#include <exception>

String::String(): maxStr(10), lenStr(0) {
    str = new char[maxStr];
    if (!str) {
        throw std::bad_alloc();
    }
    str[0] = '\0';
    count++;
}


String::String(const char* _str) {
    maxStr = (int)strlen(_str) + 1;
    lenStr = (int)strlen(_str);
    str = new char[maxStr];
    if (!str) {
        throw std::bad_alloc();
    }
    strcpy(str, _str);
    count++;
}


String::String(const String& other) {
    maxStr = other.maxStr;
    lenStr = other.lenStr;
    str = new char[maxStr];
    if (!str) {
        throw std::bad_alloc();
    }
    strcpy(str, other.str);
}



String::~String() {
    delete[] str;
    count--;
}


void String::setStr(const char* _str) {
    if (strlen(_str) > maxStr) {
        while (strlen(_str) > maxStr) {
            maxStr *= 2;
        }
        delete[] str;
        str = new char[maxStr];
        if (!str) {
            throw std::bad_alloc();
        }
        strcpy(str, _str);
    }

    else {
        strcpy(str, _str);
    }

    lenStr = (int)strlen(str);
}


char* String::getStr() {
    return str;
}

const char* String::getStr1() {
    return str;
}

void String::setLenStr(int number) {
    lenStr = number;
}

int String::getLenStr() {
    return lenStr;
}

void String::setMaxStr(int number) {
    maxStr = number;
}

int String::getMaxStr() {
    return maxStr;
}


int String::searchStr(const char *_str) {
    for (int i = 0; str[i] != '\0'; i++) {
        int j = 0;
        for (; _str[j] != '\0' && str[i+j] == _str[j]; j++);
        if (_str[j] == '\0') {
            return i;
        }
    }
    return -1;
}


char* String::toString() {
    char* returnedString = new char[maxStr + 50];
    if (!returnedString) {
        throw std::bad_alloc();
    }
    sprintf(returnedString, "%s %d %d", str, lenStr, maxStr);
    return returnedString;
}


// Статические методы

unsigned String::objectCount() {
    return count;
}




// Перегрузка операторов

String operator + (const String &str1, const char* str2) {
    String result;
    result.lenStr = str1.lenStr + strlen(str2);
    result.maxStr = str1.lenStr + strlen(str2) + 1;
    result.str = new char[str1.maxStr];
    if (!result.str) {
        throw std::bad_alloc();
    }
    strcpy(result.str, str1.str);
    for (size_t i = 0; strlen(str2) > i; i++) {
        result.str[str1.lenStr+i] = str2[i];
    }
    result.str[str1.lenStr + strlen(str2)] = '\0';
    return result;
}


String operator + (const String &str1, const String &str2) {
    return str1 + str2.str;
}


String operator - (const String &str1, const char* str2) {
    String result;
    int i = 0;
    int j = 0;
    for (; str1.str[i] != '\0'; i++) {
        j = 0;
        for (; str2[j] != '\0' && str1.str[i + j] == str2[j]; j++);
        if (str2[j] == '\0') {
            break;
        }
    }

    if (str2[j] == '\0') {
        result.lenStr = str1.lenStr - strlen(str2);
        result.maxStr = str1.lenStr - strlen(str2) + 1;
        result.str = new char[str1.maxStr];
        if (!result.str) {
            throw std::bad_alloc();
        }
        for (j = 0; result.lenStr > j; j++) {
            if (j >= i) {
                result.str[j] = str1.str[j+strlen(str2)];
            }
            else {
                result.str[j] = str1.str[j];
            }
        }
        result.str[result.lenStr] = '\0';
    }

    else {
        result.lenStr = str1.lenStr;
        result.maxStr = str1.lenStr + 1;
        result.str = new char[str1.maxStr];
        if (!result.str) {
            throw std::bad_alloc();
        }
        strcpy(result.str, str1.str);
    }

    return result;
}


char& String::operator[](unsigned int index) {
    if (index > lenStr) {
        throw std::out_of_range("Error! Index is out of stack bounds!");
    }
    return str[index];
}


String& String::operator=(const String& other) {
    if (this != &other) {
        delete[] str;
        maxStr = other.maxStr;
        lenStr = other.lenStr;
        str = new char[maxStr];
        if (!str) {
            throw std::bad_alloc();
        }
        strcpy(str, other.str);
    }
    return *this;
}



// - 3-
std::ostream& operator << (std::ostream &os, String &str)
{
    return os << str.getStr();
}

std::istream& operator >> (std::istream& in, String& str1)
{
    char str[1000];
    in.getline(str, 999, '\n');
    str1.setStr(str);
    return in;
}

void String::serialize(std::ofstream& ofs) const {
    ofs.write(reinterpret_cast<const char*>(&lenStr), sizeof(lenStr)); // Запись текущей длины строки
    ofs.write(str, lenStr); // Запись содержимого строки
}

String& String::deserialize(std::ifstream& ifs) {
    ifs.read(reinterpret_cast<char*>(&lenStr), sizeof(lenStr)); // Чтение текущей длины строки
    str = new char[lenStr + 1]; // Выделение памяти для строки
    if (!str) {
        throw std::bad_alloc();
    }
    ifs.read(str, lenStr); // Чтение содержимого строки
    str[lenStr] = '\0'; // Завершающий нуль
    return *this;
}
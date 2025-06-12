#ifndef UNTITLED1_STRING_H
#define UNTITLED1_STRING_H
#include <iostream>
#include <cstring>
class String {

public:
    String();                                               // Конструктор по умолчанию
    String(const char* _str);                               // Конструктор с параметрами
    String(const String& other);                                  // Конструктор копирования
    virtual ~String();                                              // Деструктор
    void setStr(const char* _str);
    char* getStr();
    const char* getStr1();
    void setMaxStr(int number);
    int getMaxStr();
    void setLenStr(int number);
    int getLenStr();
    virtual char* toString();
    int searchStr(const char* _str);

    static unsigned objectCount();


    friend String operator + (const String &str1, const char* str2);
    friend String operator + (const String &str1, const String &str2);
    friend String operator - (const String &str1, const char* str2);
    char& operator[](unsigned index);
    String& operator=(const String& counter);
    bool operator<(const String& other) const {
        return strcmp(this->str, other.str) < 0;//+ если 1 больше  2 - наоборот и 0 если равно
    }
    bool operator==(const String& other) const {
        return (lenStr == other.lenStr) && (strcmp(str, other.str) == 0);
    }

    void serialize(std::ofstream& ofs) const;              // Метод сериализации
    String& deserialize(std::ifstream& ifs);

private:
    char* str;                                              // Строка
    int maxStr;                                             // Максимальная длина
    int lenStr;                                             // Текущая длина без учёта '\0'
    static unsigned count;
};

std::ostream& operator << (std::ostream &os, String &str);
std::istream& operator >> (std::istream& in, String& str1);

#endif //UNTITLED1_STRING_H

#ifndef UNTITLED1_BINARYSTRING_H
#define UNTITLED1_BINARYSTRING_H
#include"String.h"
#include"List.h"
class BinaryString : public String {
public:
    BinaryString();
    BinaryString(const char* _str);
    BinaryString(BinaryString &other);

    int DecimalSystem();
    int Examination(const char* _str);

};
#endif //UNTITLED1_BINARYSTRING_H

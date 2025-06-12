#include <iostream>
#include <cmath>
#include <cstring>
#include "BinaryString.h"
#include "Exeption.h"

BinaryString::BinaryString() {
    String();
}

BinaryString::BinaryString(const char* _str): String(_str) {
    Examination(_str);
}

BinaryString::BinaryString(BinaryString &other): String(other) {}

int BinaryString::DecimalSystem() {
    int sum = 0;
    for (int i = 0, n = getLenStr() - 1; i < getLenStr(); i++, n--) {
        sum += (getStr()[i] - '0') * pow(2, n);
    }
    return sum;
}

int BinaryString::Examination(const char* _str) {
    for (int i = 0; i < strlen(_str); i++) {
        if (_str[i] != '1' && _str[i] != '0' ) {
            throw binaryError("Error! String must contain only 0 and 1!");//ошибка на бинарность
        }
    }
    return 1;
}




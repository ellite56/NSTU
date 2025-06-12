#include <iostream>
#include "Exeption.h"

binaryError::binaryError(const std::string& message): message{message}//конструктор
{}

const char* binaryError::what() const noexcept {//noexcept говорит о том , что мсключение не будет выброгено
    return message.c_str();//текст про ошибку
}

#ifndef UNTITLED1_EXEPTION_H
#define UNTITLED1_EXEPTION_H
#include <exception>
#include <cstring>
//класс исключений
class binaryError: public std::exception//стандартный класс исключений
{
public:
    binaryError(const std::string& message);//конструктор с парметром
    const char* what() const noexcept override;

private:
    std::string message;    // сообщение об ошибке
};
#endif //UNTITLED1_EXEPTION_H

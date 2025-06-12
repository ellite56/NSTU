#ifndef UNTITLED1_LIST_H
#define UNTITLED1_LIST_H

#include <iostream>
#include <stdexcept>
#include <cstring>
#include <sstream>


// Шаблонный класс Stack
template<typename T>
class Stack {
private:
    // Вложенная структура для узлов списка
    struct Node {
        T data;
        Node *next;

        Node(T value) : data(value), next(nullptr) {}
    };

    Node *top;  // Указатель на верхний элемент стека

public:
    // Конструктор и деструктор
    Stack() : top(nullptr) {}

    ~Stack() { clear(); }

    // Методы стека
    bool isEmpty() const {//пустой ли стек
        return top == nullptr;
    }

    void push(T value) {//добавление новых элементов
        Node *newNode = new Node(value);
        newNode->next = top;
        top = newNode;
    }

    T pop() {//удаление
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        Node *temp = top;
        top = top->next;
        T value = temp->data;
        delete temp;
        return value;
    }

    void insertObject(int index, T value) {//вставка по индексу
        if (index == 0) {
            push(value);
            return;
        }

        Node *current = top;
        for (int i = 0; current != nullptr && i != index; ++i) {
            current = current->next;
        }
        if (current) {
            Node *newNode = new Node(value);
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    T &getObject(int index) {//извлечение элемента по индексу
        Node *current = top;
        for (int i = 0; current != nullptr && i != index; ++i) {
            current = current->next;
        }
        if (current) {
            return current->data;
        }
        throw std::out_of_range("Index out of range");
    }

    void removeObject(int index) {//удаление по идексу
        if (index == 0) {
            pop();
            return;
        }

        Node *current = top;
        for (int i = 0; current != nullptr && i != index - 1; ++i) {
            current = current->next;
        }
        if (current && current->next) {//если стек не пуст и то, что следующий элемент существует
            Node *temp = current->next;
            current->next = temp->next;
            delete temp;
        }
    }

    void display() const {//вывод
        Node *current = top;
        while (current) {
            std::cout << current->data << std::endl;
            current = current->next;
        }
    }

    void clear() {//очистка
        while (!isEmpty()) {
            pop();
        }
    }

    // Метод toString для получения строкового представления стека
    const char *toString() const {
        Node *current = top;
        std::stringstream ss;
        ss << "Stack contents: ";

        while (current) {
            ss << current->data;

            if (current->next) {
                ss << ", ";
            }
            current = current->next;
        }
        // Возвращаем C-строку (массив символов)
        char *s = new char[ss.str().size() + 1];
        strcpy(s, ss.str().c_str());
        return s;  // Возвращаем указатель на C-строку
    }
};

// Специализация стека для типа String*
template<>
class Stack<String *> {
private:
    // Вложенная структура для узлов списка
    struct Node {
        String *data;
        Node *next;

        Node(String *value) : data(value), next(nullptr) {}
    };

    Node *top;  // Указатель на верхний элемент стека

public:
    // Конструктор и деструктор
    Stack() : top(nullptr) {}

    ~Stack() { clear(); }

    // Методы стека
    bool isEmpty() const {
        return top == nullptr;
    }

    void push(String *value) {
        Node *newNode = new Node(value);
        newNode->next = top;
        top = newNode;
    }

    String *pop() {
        if (isEmpty()) {
            throw std::out_of_range("Stack is empty");
        }
        Node *temp = top;
        top = top->next;
        String *value = temp->data;
        delete temp;
        return value;
    }

    void insertObject(int index, String *value) {
        if (index == 0) {
            push(value);
            return;
        }

        Node *current = top;
        for (int i = 0; current != nullptr && i != index; ++i) {
            current = current->next;
        }
        if (current) {
            Node *newNode = new Node(value);
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    String *getObject(int index) {
        Node *current = top;
        for (int i = 0; current != nullptr && i != index; ++i) {
            current = current->next;
        }
        if (current) {
            return current->data;
        }
        throw std::out_of_range("Index out of range");
    }

    void removeObject(int index) {
        if (index == 0) {
            pop();
            return;
        }

        Node *current = top;
        for (int i = 0; current != nullptr && i != index; ++i) {
            current = current->next;
        }
        if (current && current->next) {
            Node *temp = current->next;
            current->next = temp->next;
            delete temp;
        }
    }

    void display() const {
        Node *current = top;
        while (current) {
            std::cout << *(current->data) << std::endl;  // Для типа String* используем разыменование указателя
            current = current->next;
        }
    }

    void clear() {
        while (!isEmpty()) {
            String *value = pop();
            delete value;  // Освобождение памяти, так как объекты String были созданы с помощью new
        }
    }

    // Метод toString для получения строкового представления стека
    const char *toString() const {
        Node *current = top;
        std::stringstream ss;
        ss << "stack contents"<<std::endl;
        while (current) {
            ss << current->data->toString();
            ss<<std::endl;
            current = current->next;
        }
        char *s = new char[ss.str().size() + 1];
        strcpy(s, ss.str().c_str());
        return s;  // Возвращаем указатель на C-строку
    }
};


#endif //UNTITLED1_LIST_H

#include"String.h"
#ifndef UNTITLED1_TODOLIST_H
#define UNTITLED1_TODOLIST_H
class ToDoList : public String {
public:
    ToDoList();
    ToDoList(const char* _str, const char* _startTime, const char* _endTime);
    ToDoList(ToDoList &other);
    ~ToDoList();
    void markCompleted();
    char* toString() override;

private:
    char* startTime;
    char* endTime;
    bool isComplete;
};

#endif //UNTITLED1_TODOLIST_H

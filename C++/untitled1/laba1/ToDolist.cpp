#include <iostream>
#include <cstring>
#include "ToDoList.h"

ToDoList::ToDoList() : String() {
    startTime = new char[20];
    strcpy(startTime, "01.01.2000 13:00:00");
    endTime = new char[20];
    strcpy(endTime, "01.01.2000 13:00:00");
    isComplete = false;
}

ToDoList::ToDoList(const char* _str, const char *_startTime, const char *_endTime) : String(_str) {
    startTime = new char[strlen(_startTime)];
    strcpy(startTime, _startTime);
    endTime = new char[strlen(_endTime)];
    strcpy(endTime, _endTime);
    isComplete = false;
}

ToDoList::ToDoList(ToDoList &other) : String(other) {
    startTime = new char[strlen(other.startTime)];
    strcpy(startTime, other.startTime);
    endTime = new char[strlen(other.endTime)];
    strcpy(endTime, other.endTime);
    isComplete = other.isComplete;
}

ToDoList::~ToDoList() {
    delete[] startTime;
    delete[] endTime;
}

void ToDoList::markCompleted() {
    isComplete = false;
}

char* ToDoList::toString() {
    char* returnedString = new char[80];
    sprintf(returnedString, "%s Start:%s End:%s Status:%s", getStr() ,startTime, endTime, isComplete ? "Completed" : "Not completed");
    return returnedString;
}

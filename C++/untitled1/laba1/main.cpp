#include"String.h"
#include"Test.h"
#include"Binarystring.h"
#include"ToDolist.h"
#include"List.h"
#include"Twoqueue.h"
#include"Multimap.h"

unsigned String::count = 0;

int main() {
    //1 лаба
    /*testConstructor();
    testConstructorWithParam1();
    testConstructorWithParam2();
    testConstructorCopy();
    testSetStr1();
    testSetStr2();
    testSearchStr1();
    testSearchStr2();
    testStaticFunction();*/
    //2 лаба
/*
    testOperatorPlCh();
    testOperatorPlObj();
    testOperatorMinCh();
    testOperatorIndex1();
    testOperatorIndex2();
    testOperatorAssign();*/
    //3 лаба

   /* testOstream();
    testIstream();
    testTextFile();
    testBinaryFileOne();
    testBinaryFileSeveral();
*/
    //4 лаба

  /*  testConstBinaryStr();
    testConstWithParamBinaryStr();
    testConstrCopyBinaryString();
    testDecimalSystemBinaryString();

    testConstrToDoList();
    testConstrrWithParamToDoList();
    testConstCopyToDoList();
    testMarkCompletedToDoList();*/


    //5-7 лаба
    /*testIntStack();
    testDoubleStack();
    testStringStack();*/

    //6 лаба
    //testException();

    //8 лаба
    int n = 500000;
    analyzeDequeOperations(n);
    std::cout << std::endl;
    analyzeDequeOperationsString(n);
    std::cout << std::endl;
    analyzeMultimapOperations(n);
    std::cout << std::endl;
    analyzeMultimapOperationsString(n);
    std::cout << std::endl;

    return 0;
}

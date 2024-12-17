#include <iostream>
#include <functional>
#include <cassert>
#include <numeric>

#include "simple_vector.h"
#include "tests.h"

int main() {
    
    BaseTest1();
    BaseTest2();
    TestReserveConstructor();
    TestReserveMethod();
    TestTemporaryObjConstructor();
    TestTemporaryObjOperator();
    TestNamedMoveConstructor();
    TestNamedMoveOperator();
    TestNoncopiableMoveConstructor();
    TestNoncopiablePushBack();
    TestNoncopiableInsert();
    TestNoncopiableErase();
    
    return 0;
}
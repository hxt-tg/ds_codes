#define DEBUG
#define _GLIBCXX_USE_C99 1


#include "exp.h"
#include <iostream>
using namespace std;

void pause() {
    cout << endl << "Type [Enter] to continue ...";
    getchar();
}

void linked_list_test() {
    LinkedList<int> ll;
    for (int i = 2; i < 5; i++)
        ll.insert(i, 999);
    cout << ll[0] << endl;
    cout << ll[1] << endl;
    cout << ll[2] << endl;
    ll.remove((unsigned int)0);
    ll.remove((unsigned int)0);
    ll.remove((unsigned int)0);
    cout << ll[0] << endl;
}

void exp_test() {
    Expression a("2+5X^2"), b("3+3X");
    ExpData d = { -2, 1 };
    cout << (a/d).toDblLine("Out[1]: ", true) << endl;
    cout << (a/d).devirate().toDblLine("Out[2]: ", true) << endl;
}

int main() {
    exp_test();

    pause();
    return 0;
}

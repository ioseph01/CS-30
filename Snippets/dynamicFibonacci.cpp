#include <iostream>
using namespace std;

int fib(int n) {
    if (n < 2) { return n; }
    cout << "A\n";
    int a = 0, b = 1, temp;
    for (int i = 2; i < n; i++) {
        temp = a + b;
        a = b;
        b = temp;
    }
    return a + b;
}

int main()
{
    cout << fib(5) << endl;

    return 0;
}

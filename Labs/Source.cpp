#include <iostream>
#include <vector>
using namespace std;

int fib_bottom2(int n);
int fib_bottom(int n);
int fib_top(int n);
int fib_top(int n, vector<int>& memo);


int fib(int n) {
	if (n < 2) return n;
	return fib(n - 1) + fib(n - 2);
}

int main() {

	cout << "fib(10) rec:" << fib(10) << endl;
	cout << "fib(10) top:" << fib_top(10) << endl;
	//cout << "fib(10) bot:" << fib_bottom(10) << endl;
	//cout << "fib(10) bot2:" << fib_bottom2(10) << endl;
}

// --------------- Fibonacci - top down approach

int fib_top(int n) {

	vector<int> memo(n + 1, -1);
	return fib_top(n, memo);
}

int fib_top(int n, vector<int>& memo) {
	if (n < 2) return n;
	if (memo[n] != -1)
		return memo[n];
	memo[n] = fib_top(n - 1, memo) + fib_top(n - 2, memo);
	return memo[n];
}


// --------------- Fibonacci - bottom up approach

// #1
int fib_bottom(int n) {
	vector<int> memo(n + 1);
	memo[0] = 0;
	memo[1] = 1;
	for (int i = 2; i <= n; i++) {
		memo[i] = memo[i - 1] + memo[i - 2];
	}
	return memo[n];
}

// #2
int fib_bottom2(int n) {
	int a = 0, b = 1, temp;
	if (n == 0) return a;
	for (int i = 2; i < n; i++) {
		temp = a + b;
		a = b;
		b = temp;
	}
	return a + b;
}
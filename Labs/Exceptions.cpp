#include <vector>
#include <iostream>
#include <stdexcept>


using namespace std;


class InvalidSize : public invalid_argument { 
	public:
		InvalidSize(const char* s) : invalid_argument(s) { }
};

class StackFull : public runtime_error {
	private:
		int m_i;

	public:
		StackFull(const char* s, int i) : runtime_error(s), m_i(i) {}
		int GetValue() const { return m_i; }
};

class StackEmpty : public runtime_error {
	public:
		StackEmpty(const char* s) : runtime_error(s) {}
};


class IntStack
{
public:
	// MaxSize should be an unsigned int, but for the sake of example...
	IntStack(int MaxSize)
	{
		if (MaxSize < 0) { throw InvalidSize("Cannot create a negative size stack"); }
		else {
			data.resize(MaxSize);
			cur_index = 0;
		}
	}

	void push(int new_data)
	{
		if (cur_index == data.size()) { throw StackFull("Push to full stack", new_data); }
		else
		{
			data.at(cur_index) = new_data;
			cur_index++;
		}
	}

	int pop()
	{
		if (cur_index == 0) { throw StackEmpty("Pop from empty stack"); }
		else
		{
			// pop off the int and return it
			cur_index--;
			return data.at(cur_index);
		}
	}

private:
	vector<int> data;
	unsigned int cur_index;
};


void StackError() {
	try { throw; }
	catch (InvalidSize& a) { cerr << a.what() << endl; }
	catch (StackFull& a) { cerr << a.what() << ", value = " << a.GetValue() << endl; }
	catch (StackEmpty& a) { cerr << a.what() << endl; }
	catch (...) { cerr << "Unknown exception" << endl; }
}

int main()
{
	// Testing Constructor
	try {
		IntStack c_test(-10);
		c_test.push(3);
		c_test.push(4);
		c_test.pop();
		c_test.pop();
	}
	catch (...) { StackError(); }

	

	try {
		// Testing push
		IntStack push_test(5);

		for (unsigned int i = 0; i < 7; i++) {
			push_test.push(i);
		}
	}
	catch (...) { StackError(); }

	try {
		// Testing pop
		IntStack pop_test(2);

		pop_test.push(1);
		pop_test.push(2);
		pop_test.pop();
		pop_test.pop();
		pop_test.pop();

		cout << "Completed!" << endl;
	}
	catch (...) { StackError(); }

	
}

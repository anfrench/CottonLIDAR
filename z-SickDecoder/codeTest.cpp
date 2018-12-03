#include <iostream>
#include <string>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


class a
{
	public:
	a(){}
	virtual void printA(){}
	virtual void printB(){}
	virtual void print()
	{
		cout<<"this is a test\n";
		printA();
		printB();
		cout<<"no more\n";
	}
};


class B:public a
{
	public:
	B(){}
	virtual void printA(){cout<<"this is a\n";}
	virtual void printB(){cout<<"this is b\n";}
	
};


string swap2(string toSwap);
string swap4(string toSwap);
int main()
{
	
	B test;
	//cout<<"test\n";
	test.print();
	return 0;
}

string swap2(string toSwap)
{
	return toSwap.substr(2)+toSwap.substr(0,2);	
}




string swap4(string toSwap)
{
	return swap2(toSwap.substr(4)) + swap2(toSwap.substr(0,4));	
}

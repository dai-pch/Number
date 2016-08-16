#include <iostream>
using namespace std;

int main()
{
	char i1 = 127, i2 = 127, i3;
	int i4;
	i3 = i1 + i2;
	i4 = i1 + i2;
	cout << i1 << ", " << i2 << endl;
	cout << i3 << ", " << i4 << endl;
	system("pause");
	return 0;
}
#include <iostream>
using namespace std;

int main()
{
	unsigned char i1 = 127, i2 = 127;
	short i3;
	int i4 = 65535, i5 = 65535, i6 = i4 + i5;
	//i3 = i1 - i2;
	//i4 = i1 + i2;
	i3 = i4+i5;
	cout << i1 << ", " << i2 << endl;
	cout << i3 << ", " << i4 << endl;
	system("pause");
	return 0;
}
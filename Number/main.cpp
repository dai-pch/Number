#include <iostream>
#include <vector>
using namespace std;

int main()
{
	unsigned char i1 = 127, i2 = 127;
	short i3;
	int i4 = -5, i5 = 4, i6 = i4 + i5;
	//i3 = i1 - i2;
	//i4 = i1 + i2;
	i3 = i4/i5;
	vector<int> a(5,10);
	cout << a[0];
	cout << i1 << ", " << i2 << endl;
	cout << i3 << ", " << i4 << endl;
	system("pause");
	return 0;
}
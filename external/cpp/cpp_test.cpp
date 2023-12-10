#include <iostream>

//extern "C" {
#include "../../inc/mcc/array.h"
//}

using namespace std;

void test_array()
{
	int arr[5] = {1, 2, 3, 4, 5};
	int arr1[5] = {1, 2, 3, 4, 5};
	int r;

	r = is_array_equal(arr, arr1, 5);
	cout << "is_array_equal: " << r << endl;
}

int main()
{
	cout << "C++ calls mcc" << endl;
	test_array();
	cout << "C++ calls mcc done" << endl;
	return 0;
}
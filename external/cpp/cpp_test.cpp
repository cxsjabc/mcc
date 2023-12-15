#include <iostream>

//extern "C" {
#include "../../inc/mcc/args.h"
#include "../../inc/mcc/array.h"
#include "../../inc/mcc/compile_log.h"
#include "../../inc/mcc/config.h"
#include "../../inc/mcc/env.h"
#include "../../inc/mcc/error.h"
#include "../../inc/mcc/features.h"
#include "../../inc/mcc/file.h"
#include "../../inc/mcc/help.h"
#include "../../inc/mcc/id.h"
#include "../../inc/mcc/keyword.h"
#include "../../inc/mcc/lex.h"
#include "../../inc/mcc/log.h"
#include "../../inc/mcc/mcc_base.h"
#include "../../inc/mcc/mcc_state.h"
#include "../../inc/mcc/mem.h"
#include "../../inc/mcc/mem_buf.h"
#include "../../inc/mcc/print.h"
#include "../../inc/mcc/size.h"
#include "../../inc/mcc/string.h"
#include "../../inc/mcc/time.h"
#include "../../inc/mcc/token.h"
#include "../../inc/mcc/token_internal.h"
#include "../../inc/mcc/type.h"
#include "../../inc/mcc/ver.h"
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
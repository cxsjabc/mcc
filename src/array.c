#include <stdio.h>

#include "mcc/array.h"

int is_array_equal(int arr[], int expect_arr[], int size)
{
    int i = 0;

    for (i = 0; i < size; i++) {
        if (arr[i] != expect_arr[i])
            return 0;
    }

    return 1;
}
#!/bin/bash

usage() {
    echo "Usage: ./$0 <code_str> <code_type>"
    exit 0
}

if [ $# -lt 1 ]; then
    echo "Missing arguments."
    usage
elif [ $# -eq 1 ]; then
    CODE_STR="$1"
    CODE_TYPE="int"
elif [ $# -eq 2 ]; then
    CODE_STR="$1"
    CODE_TYPE="$2"
else
    usage
fi

FILE_NAME="__code_str_0a_b1__.c"

gen_defined_int() {
cat << EOF > $FILE_NAME
#include <stdio.h>

int main()
{  
    printf("$CODE_STR is %lld\n", (unsigned long long)$CODE_STR);

    return 0;
}
EOF
}

gen_defined_str() {
cat << EOF > $FILE_NAME
#include <stdio.h>

int main()
{  
    printf("$CODE_STR is %s\n", $CODE_STR);

    return 0;
}
EOF
}

if [ "$CODE_TYPE" == "str" ]; then
    gen_defined_str
else
    gen_defined_int
fi

gcc -o $(basename $FILE_NAME).elf -Werror $FILE_NAME
if [ $? -eq 0 ]; then
    cat $FILE_NAME
    ./$(basename $FILE_NAME).elf
else
    echo "Compile failed."
fi

#rm $(basename $FILE_NAME).elf
#rm $FILE_NAME

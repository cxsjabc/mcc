#!/bin/bash

usage() {
    echo "Usage: ./$0 <define_name> <define_type>"
    exit 0
}

if [ $# -lt 1 ]; then
    echo "Missing arguments."
    usage
elif [ $# -eq 1 ]; then
    DEFINE_NAME=$1
elif [ $# -eq 2 ]; then
    DEFINE_NAME=$1
    DEFINE_TYPE=$2
else
    usage
fi

FILE_NAME="__get_define_0a_b1__.c"

gen_defined_name() {
cat << EOF > $FILE_NAME
#include <stdio.h>

int main()
{  
#if defined($DEFINE_NAME)
    printf("$DEFINE_NAME is defined\n");
#else
    printf("$DEFINE_NAME is undefined.\n");
#endif

    return 0;
}
EOF
}

gen_defined_int() {
cat << EOF > $FILE_NAME
#include <stdio.h>

int main()
{  
#if defined($DEFINE_NAME)
    printf("$DEFINE_NAME is %d\n", $DEFINE_NAME);
#else
    printf("$DEFINE_NAME is undefined.\n");
#endif

    return 0;
}
EOF
}

gen_defined_str() {
cat << EOF > $FILE_NAME
#include <stdio.h>

int main()
{  
#if defined($DEFINE_NAME)
    printf("$DEFINE_NAME is %s\n", $DEFINE_NAME);
#else
    printf("$DEFINE_NAME is undefined.\n");
#endif

    return 0;
}
EOF
}

if [ "$DEFINE_TYPE" == "str" ]; then
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
    gen_defined_name
    gcc -o $(basename $FILE_NAME).elf $FILE_NAME
    [ $? -eq 0 ] && cat $FILE_NAME && ./$(basename $FILE_NAME).elf
fi

rm $(basename $FILE_NAME).elf
rm $FILE_NAME

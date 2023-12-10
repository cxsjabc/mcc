#!/bin/bash

find inc external test src -name "*.[ch]" -or -name "*.cpp" | xargs sed -i 's/    /\t/g'

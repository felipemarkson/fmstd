#!/bin/bash
set -e

LIB=$1
LIB_NOEXT=${LIB%.*}
BIN_NAME=$(basename $LIB_NOEXT)_test
BIN_NAME_GNU=$(basename $LIB_NOEXT)_test_gnu
C_FILE=$BIN_NAME.c
TEST_FLAG="FM_TEST"


clang-format -style=file -i $LIB

cppcheck --quiet --language=c --std=c99 --max-ctu-depth=1000 --inconclusive \
         --suppress=missingIncludeSystem \
         --suppress=unusedStructMember \
         --enable=style,warning,performance,portability,missingInclude \
         --check-level=exhaustive \
         $LIB

WARNINGSCLANG="  -Warray-bounds-pointer-arithmetic -Wconditional-uninitialized \
                 -Wloop-analysis -Wshift-sign-overflow -Wassign-enum \
                 -Wtautological-constant-in-range-compare -Wformat-type-confusion \
                 -Widiomatic-parentheses -Wunreachable-code-aggressive
                 -Wno-nullability-extension"

WARNINGSGCC="    -Warray-bounds=2 -Wattribute-alias=2 -fanalyzer"

WARNINGSCOMMON=" -fstrict-aliasing -Wall -Wextra -Wpedantic -Wdouble-promotion \
                 -Warray-bounds -Wmissing-include-dirs -Wshift-overflow -Wswitch-default \
                 -Wnull-dereference -Wbad-function-cast \
                 -Wswitch-enum -Wunused-const-variable -Wstrict-overflow=5 \
                 -Walloca -Wfloat-equal -Wshadow -Wcast-qual \
                 -Wcast-align -Wwrite-strings -Wconversion -Wparentheses \
                 -Wstrict-prototypes -Wold-style-definition  -Wno-multichar \
                 -Wredundant-decls -Winline -ftrapv"

rm -f $C_FILE
echo "#define $TEST_FLAG" >> $C_FILE
echo "#include \"$LIB\"" >> $C_FILE

export UBSAN_OPTIONS="print_stacktrace=1"

clang   -ggdb -O3 -std=c99 -pedantic \
        $WARNINGSCOMMON $WARNINGSCLANG \
        -fsanitize=address,leak,undefined,nullability,float-divide-by-zero,implicit-conversion,local-bounds,nullability\
        -fno-omit-frame-pointer -fno-optimize-sibling-calls \
        -o $BIN_NAME $C_FILE

gcc     $WARNINGSCOMMON $WARNINGSGCC -ggdb -O3 -std=c99 -pedantic -o $BIN_NAME_GNU $C_FILE

./$BIN_NAME
valgrind --quiet ./$BIN_NAME_GNU 1>/dev/null

rm $C_FILE
rm $BIN_NAME
rm $BIN_NAME_GNU
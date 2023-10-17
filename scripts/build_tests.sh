files=$(ls test/*.c)
set -e

cppcheck -I. --quiet --enable=all --language=c --std=c89 --max-ctu-depth=1000 \
--inconclusive --suppress=missingIncludeSystem --suppress=unreadVariable  test/

DEBUG="-Og -ggdb"
OPT="-O3"
WARNINGS="  -Wall -Wextra -Werror -Wformat-security -Wfloat-equal -Wshadow \
            -Wswitch-enum -Wbad-function-cast -Wconversion -Wlogical-not-parentheses \
            -Wnull-dereference -Wpedantic -Wformat=2 -Warray-bounds \
            -Warray-bounds-pointer-arithmetic -Wconditional-uninitialized \
            -Wloop-analysis -Wshift-sign-overflow -Wassign-enum -Wpointer-arith \
            -Wtautological-constant-in-range-compare -Wformat-type-confusion \
            -Widiomatic-parentheses -Wunreachable-code-aggressive"
CFLAGS="-std=c99 -pedantic"

rm -rf bin/test
mkdir -p bin/test
for file in $files; do
    bin=${file%.*}
    bin_d=${bin}.d
    echo "building $file"
    clang $WARNINGS $CFLAGS $OPT -I. $file -o bin/$bin
    if [ "$ENV_BUILD" != "GH" ]; then
        clang $WARNINGS $CFLAGS $DEBUG -I. $file -o bin/$bin_d
    fi
done
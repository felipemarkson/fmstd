set -e

clang-format -style=file -i fmda.h

cppcheck --quiet --language=c --std=c99 --max-ctu-depth=1000 --inconclusive \
         --suppress=missingIncludeSystem \
         --enable=style,warning,performance,portability,missingInclude \
         --check-level=exhaustive \
         fmda.h

WARNINGS="  -Wall -Wextra -Werror -Wformat-security -Wfloat-equal -Wshadow \
            -Wswitch-enum -Wbad-function-cast -Wconversion -Wlogical-not-parentheses \
            -Wnull-dereference -Wpedantic -Wformat=2 -Warray-bounds \
            -Warray-bounds-pointer-arithmetic -Wconditional-uninitialized \
            -Wloop-analysis -Wshift-sign-overflow -Wassign-enum -Wpointer-arith \
            -Wtautological-constant-in-range-compare -Wformat-type-confusion \
            -Widiomatic-parentheses -Wunreachable-code-aggressive \
            -fstrict-aliasing -Wstrict-aliasing"

rm -f fmda_test.c
echo "#define FMDA_TEST" >> fmda_test.c
echo "#include \"fmda.h\"" >> fmda_test.c

clang   -ggdb -O3 -std=c99 -pedantic \
        $WARNINGS \
        -fsanitize=address,undefined,leak,integer \
        -fno-omit-frame-pointer -fno-optimize-sibling-calls \
        -o fmda_test fmda_test.c

gcc     -ggdb -O3 -std=c99 -pedantic -o fmda_test_valgrind fmda_test.c

./fmda_test
valgrind --quiet ./fmda_test_valgrind

rm fmda_test.c
rm fmda_test
rm fmda_test_valgrind
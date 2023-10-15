files=$(ls test/*.c)
set -e

cppcheck -I. --quiet --enable=all --language=c --std=c89 --max-ctu-depth=1000 \
--inconclusive --suppress=missingIncludeSystem --suppress=unreadVariable  test/

DEBUG="-Og -ggdb"
CFLAGS="-std=c89 -pedantic -Wall -Wextra -Werror"

mkdir -p bin/test
for file in $files; do
    bin=${file%.*}
    echo "building $file"
    clang $CFLAGS $DEBUG -I. $file -o bin/$bin
done
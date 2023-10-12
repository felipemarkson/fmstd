files=$(ls test/*.c)
set -e

cppcheck -I. --quiet --enable=all --language=c --std=c89 --max-ctu-depth=1000 \
--inconclusive --suppress=missingIncludeSystem --suppress=unreadVariable  test/

DEBUG="-Og -ggdb" 

mkdir -p bin/test
for file in $files; do
    bin=${file%.*}
    echo "building $file"
    clang -std=c89 -pedantic -Wall -Wextra -Werror $DEBUG -I. $file -o bin/$bin
done
set -e
files=$(ls bin/test/*)
for file in $files; do
    echo "running $file"
    valgrind -q --leak-check=full $file
done
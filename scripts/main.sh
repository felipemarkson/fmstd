
set -e

./scripts/format.sh
./scripts/assembly_header.sh
clang-format -style=file --Werror -dry-run fmstd.h
./scripts/build_tests.sh
./scripts/run_tests.sh
./scripts/build_run_example.sh

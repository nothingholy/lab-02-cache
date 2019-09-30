#! /bin/sh

set -ex

./scripts/tests.sh

cmake -H. -B .build -DBUILD_COVERAGE=On
cmake --build .build --target gcov
cmake --build .build --target test
gcovr -r  .
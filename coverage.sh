#!/bin/bash
cd /Users/alex/CLionProjects/b24506_trufanov.ar/3/cmake-build-coverage
make -j4
ctest
lcov --capture --directory . --output-file coverage.info --ignore-errors inconsistent,format
lcov --remove coverage.info '/usr/*' '*/tests/*' '/opt/homebrew/*' --output-file coverage.info --ignore-errors inconsistent
genhtml coverage.info --output-directory coverage_report --ignore-errors inconsistent,corrupt,category,unsupported
open coverage_report/index.html


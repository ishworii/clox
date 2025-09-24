#!/bin/bash

# Build and run all tests in the tests directory

set -e  # Exit on any error

echo "=== Building clox ==="
if [ -d "build" ]; then
    rm -rf build
fi
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
cd ..

echo ""
echo "=== Running all tests ==="
echo ""

# Track test results
total_tests=0
passed_tests=0
failed_tests=0

# Find all .lox files in tests directory
for test_file in tests/*.lox; do
    if [ -f "$test_file" ]; then
        total_tests=$((total_tests + 1))

        echo "----------------------------------------"
        echo "Running: $(basename "$test_file")"
        echo "----------------------------------------"

        # Run the test and capture exit code
        if ./build/clox "$test_file"; then
            echo ""
            echo "PASSED: $(basename "$test_file")"
            passed_tests=$((passed_tests + 1))
        else
            echo ""
            echo "FAILED: $(basename "$test_file")"
            failed_tests=$((failed_tests + 1))
        fi

        echo ""
    fi
done

# Print summary
echo "========================================"
echo "TEST SUMMARY"
echo "========================================"
echo "Total tests: $total_tests"
echo "Passed: $passed_tests"
echo "Failed: $failed_tests"

if [ $failed_tests -eq 0 ]; then
    echo ""
    echo "All tests passed!"
    exit 0
else
    echo ""
    echo "Some tests failed!"
    exit 1
fi
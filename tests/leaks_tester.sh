#!/bin/bash

# Test if file leaked when running with valgrind
SOURCE_FILE="dummy_error.c"
EXECUTABLE="dummy_error"
VALGRIND_LOG="valgrind_errors.log"

# Compile the C file
echo "Compiling $SOURCE_FILE..."
gcc -g -o "$EXECUTABLE" "$SOURCE_FILE"

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Running valgrind memory leak check..."

# Run valgrind and send errors to log file
valgrind --leak-check=full --error-exitcode=1 --log-file="$VALGRIND_LOG" ./"$EXECUTABLE"

# Check bytes in use at exit - if > 0, there's a leak
BYTES_IN_USE=$(grep "in use at exit:" "$VALGRIND_LOG" | sed 's/.*in use at exit: \([0-9,]*\) bytes.*/\1/' | tr -d ',')

if [ "$BYTES_IN_USE" -gt 0 ]; then
    echo "❌ MEMORY LEAK DETECTED!"
    echo "$BYTES_IN_USE bytes leaked"
    echo "Valgrind output:"
    cat "$VALGRIND_LOG"
    TEST_RESULT="FAILED"
else
    echo "✅ NO MEMORY LEAKS DETECTED"
    TEST_RESULT="PASSED"
fi

# Clean up
rm -f "$EXECUTABLE" "$VALGRIND_LOG"

# Exit with appropriate code
if [ "$TEST_RESULT" = "PASSED" ]; then
    echo "Test PASSED"
    exit 0
else
    echo "Test FAILED"
    exit 1
fi

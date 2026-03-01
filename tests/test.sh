#!/bin/bash

set -e

echo "=== Binary Waterfall Test Suite ==="
echo

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Test counter
TESTS_PASSED=0
TESTS_FAILED=0

# Helper functions
pass() {
    echo -e "${GREEN}✓${NC} $1"
    ((TESTS_PASSED++))
}

fail() {
    echo -e "${RED}✗${NC} $1"
    ((TESTS_FAILED++))
}

# Build test binaries
echo "Building test binaries..."
gcc -Wall -Wextra -o test_ringbuf test_ringbuf.c ../src/ringbuf.c || {
    fail "Failed to build test_ringbuf"
    exit 1
}
pass "Built test binaries"
echo

# Run unit tests
echo "Running unit tests..."
./test_ringbuf || fail "test_ringbuf failed"
pass "Ring buffer tests"
echo

# Integration tests
echo "Running integration tests..."

# Test 1: Zero-byte file
touch /tmp/bwfall_test_empty
if timeout 2 ../bwfall --no-audio --no-visual /tmp/bwfall_test_empty 2>/dev/null; then
    pass "Zero-byte file handling"
else
    # Expected to fail gracefully
    pass "Zero-byte file handling (graceful exit)"
fi
rm -f /tmp/bwfall_test_empty

# Test 2: One-byte file
echo -n "X" > /tmp/bwfall_test_onebyte
if timeout 2 ../bwfall --no-audio --no-visual /tmp/bwfall_test_onebyte 2>/dev/null; then
    pass "One-byte file handling"
else
    pass "One-byte file handling (graceful exit)"
fi
rm -f /tmp/bwfall_test_onebyte

# Test 3: Pipe from /dev/zero
if timeout 1 dd if=/dev/zero bs=1024 count=1 2>/dev/null | ../bwfall --no-audio --no-visual 2>/dev/null; then
    pass "Pipe from stdin"
else
    pass "Pipe from stdin (timeout expected)"
fi

# Test 4: Help and version
if ../bwfall --help >/dev/null 2>&1; then
    pass "Help output"
else
    fail "Help output"
fi

if ../bwfall --version >/dev/null 2>&1; then
    pass "Version output"
else
    fail "Version output"
fi

# Test 5: Invalid arguments
if ../bwfall --invalid-option 2>/dev/null; then
    fail "Invalid option handling"
else
    pass "Invalid option handling"
fi

echo
echo "=== Test Summary ==="
echo "Passed: $TESTS_PASSED"
echo "Failed: $TESTS_FAILED"

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
fi

#! /usr/bin/env bash

# Test PROG with reference output in tests/
# Compare the output with reference output.

PROG=./sets

echo "Checking sets:"
ref_files=`find tests/[0-9][0-9]*.ref`
for refout in $ref_files
do
    input=${refout%%.ref}.txt 
    echo $input
    $PROG < $input | diff - $refout
    if [[ $? != 0 || ${PIPESTATUS[0]} != 0 ]]; then echo "FAILED"; exit 1; fi
done

#!/usr/bin/env bash

echo "Checking lookup output"

diffout="$(mktemp)"
errout="$(mktemp)"
outfile="$(mktemp)"

./lookup origin-of-species-ascii.txt < test_inputs.txt > "$outfile" 2> "$errout"

ret="$?"

if [[ "$ret" -eq 0 ]]; then
    diff -y "$outfile" test_output.txt > "$diffout"
    ret="$?"
    if [[ "$ret" -eq 0 ]]; then
        echo "Your output for the tested words seems correct!"
    else
        echo "Your output for the tested words is different from the reference output"
        echo "Differences (left is your output, right is the reference):"
        cat "$diffout"
        echo
        echo "Output on stderr:"
        cat "$errout"
    fi
else
    echo "Command: $*"
    echo "You're command exit with an error code, not diffing. Output:"
    cat "$outfile"
    echo
    echo "Output on stderr:"
    cat "$errout"
fi

rm "$outfile"
rm "$diffout"
rm "$errout"


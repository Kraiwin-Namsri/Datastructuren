#! /usr/bin/env bash

# Test PROG with all txt files in tests/
# Compare the output with reference output file *_ref.txt

PROG=./queue

echo "Checking alphabetical priority:"
ref_files=`find tests/name_?_ref.txt`
for refout in tests/opgave_voorbeeld_ref.txt $ref_files
do
    input=${refout%%_ref.txt}.txt 
    echo $input
    $PROG < $input | diff - $refout
done
 
echo
echo "Checking age priority with -y flag:"
ref_files=`find tests/age_?_ref.txt`
for refout in $ref_files
do
    input=${refout%%_ref.txt}.txt 
    echo $input
    $PROG -y < $input | diff - $refout
done

echo
echo "Checking alphabetical priority with duration:"
ref_files=`find tests/name_dur_?_ref.txt`
for refout in tests/opgave_voorbeeld_duration_ref.txt $ref_files
do
    input=${refout%%_ref.txt}.txt 
    echo $input
    $PROG < $input | diff - $refout
done

echo
echo "Checking age priority with duration:"
ref_files=`find tests/age_dur_?_ref.txt`
for refout in $ref_files
do
    input=${refout%%_ref.txt}.txt 
    echo $input
    $PROG -y < $input | diff - $refout
done
 

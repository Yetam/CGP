#!/bin/bash
rm skrypt_output.txt
for bits in {4..8}
do
  for reps in {1..10}
  do
    ./CGP $bits >> skrypt_output.txt
    echo "Done $reps in bits: $bits"
  done
done

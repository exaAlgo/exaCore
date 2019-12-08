#!/bin/bash

# Setup variables
tests=(`ls -I '*.okl' -I '*.sh'`)
backends=("/host" "/occa/cpu" "/occa/gpu/opencl")

# Set debug to 0 to avoid output
export EXA_DEBUG=0

for t in "${tests[@]}"; do
  for b in "${backends[@]}"; do
    ./${t} ${b} >out 2>err
    wait $!
    if [ ! -s out ] && [ ! -s err ]; then
      echo "Test: ${t} ${b} ok."
    else
      echo "Test: ${t} ${b} not ok."
    fi
  done
done

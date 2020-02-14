#!/bin/bash

function exaInfo() {
  local GREEN='\033[0;92m'
  local NC='\033[0m'
  echo -e "${GREEN} $1 ${NC}"
}
export -f exaInfo

function exaError() {
  local RED='\033[0;31m'
  local NC='\033[0m'
  echo -e "${RED} $1 ${NC}"
}
export -f exaError

# Setup variables
tests=(`ls -I "*.okl" -I "*.sh" -I "*.log"`)
backends=("/host" "/occa/cpu" "/occa/gpu/opencl")

# Set debug to 0 to avoid debug output
export EXA_DEBUG=0

for t in "${tests[@]}"; do
  for b in "${backends[@]}"; do
    ./${t} ${b} >out.log 2>err.log
    wait $!
    if [ ! -s err.log ]; then
      exaInfo "Test: ${t} ${b} ok."
    else
      exaError "Test: ${t} ${b} not ok."
    fi
  done
done

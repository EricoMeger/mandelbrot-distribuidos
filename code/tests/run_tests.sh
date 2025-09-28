#!/bin/bash

executables=(
    "mandelbrot_2_dynamic" 
    "mandelbrot_4_dynamic"
    "mandelbrot_8_dynamic"
    "mandelbrot_2_guided"
    "mandelbrot_4_guided" 
    "mandelbrot_8_guided"
)

run_executable() {
    local exec_name=$1
    echo "running $exec_name"
    
    for i in {1..5}; do
        ./$exec_name
        echo "concluded $i run of $exec_name"
        echo "----------------------------------------"
    done
    echo ""
}

for exec in "${executables[@]}"; do
    run_executable "$exec"
done

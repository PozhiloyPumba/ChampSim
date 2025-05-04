#!/bin/bash

task(){
    echo The $1 item
    ../ChampSim/bin/champsim --warmup_instructions 10000000 --simulation_instructions 50000000 --json out/$1.json $1
}

N=8

find ./ -name "*.xz" | (
    while read filepath; do
        task "${filepath}" &
        if [[ $(jobs -r -p | wc -l) -ge $N ]]; then wait -n; fi
    done;
    wait
)

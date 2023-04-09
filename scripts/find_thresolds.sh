#!/bin/bash 
for th_pot in $(seq -45 0.5 -40);do
    for th_rmaxnorm in $(seq 0.3 0.005 0.4);do
        for llindarNeg in $(seq -0.04 0.005 0);do
            for llindarPos in $(seq 0 0.005 0.04);do
                for th_r1norm in $(seq 0 0.0001 0.0015);do
                    echo -n "th_rmaxnorm=$th_rmaxnorm llindarNeg=$llindarNeg llindarPos=$llindarPos th_r1norm=$th_r1norm th_pot=$th_pot  "
                    scripts/run_get_pitch.sh $th_rmaxnorm $llindarNeg $llindarPos $th_r1norm $th_pot > /dev/null
                    pitch_evaluate pitch_4k/train/*f0ref | fgrep TOTAL
                done  
            done
        done 
    done
done | sort -t: -k 2n;
exit 0
#!/bin/bash 
for th_rmaxnorm in $(seq 0.354 0.001 0.356); do
    for llindarPos in $(seq 0.0051 0.0001 0.0055 ); do
        for llindarNeg in $(seq -0.0154 -0.0001 -0.0157 ); do
            for th_r1norm in $(seq 0.691 0.001 0.695 ); do
                    echo -n "th_rmaxnorm=$th_rmaxnorm llindarPos=$llindarPos llindarNeg=$llindarNeg th_r1norm=$th_r1norm th_pot=-66"
                    scripts/run_get_pitch.sh $th_rmaxnorm $llindarPos $llindarNeg $th_r1norm -66  > /dev/null
                    pitch_evaluate pitch_db/train/*.f0ref | fgrep TOTAL   
            done
        done 
    done
done | sort -t: -k 2n;
exit 0


#best result run_get_pitch 0.354 0.0052 -0.0156 0.605 -66.2 --> TOTAL 91,20

                    
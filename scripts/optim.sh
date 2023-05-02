#!/bin/bash 
    for th_rmaxnorm in $(seq 0.3 0.005 0.4);do
     for llindarPos in $(seq 0 0.001 0.03);do
        for llindarNeg in $(seq -0.03 0.001 0);do
                for th_r1norm in $(seq 0 0.01 0.8);do
                    for th_pot in $(seq -45 0.5 -40);do
                    echo -n "th_rmaxnorm=$th_rmaxnorm llindarPos=$llindarPos llindarNeg=$llindarNeg th_r1norm=$th_r1norm th_pot=$lth_pot"
                    scripts/run_get_pitch.sh $th_rmaxnorm $llindarPos $llindarNeg $th_r1norm $th_pot > /dev/null
                    pitch_evaluate pitch_db/train/*.f0ref | fgrep TOTAL
                done  
            done
        done 
    done
done | sort -t: -k 2n;
exit 0
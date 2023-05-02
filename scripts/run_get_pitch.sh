#!/bin/bash

# Put here the program (maybe with path)


th_rmaxnorm=$1
llindarPos=$2
llindarNeg=$3
th_r1norm=$4
th_pot=$5
#GETF0="get_pitch"

GETF0="get_pitch --th_rmaxnorm=$th_rmaxnorm --llindarPos=$llindarPos --llindarNeg=$llindarNeg --th_r1norm=$th_r1norm --th_pot=$th_pot"
for fwav in pitch_db/train/*.wav; do
    ff0=${fwav/.wav/.f0}
    echo "$GETF0 $fwav $ff0 ----"
    $GETF0 $fwav $ff0 > /dev/null || ( echo -e "\nError in $GETF0 $fwav $ff0" && exit 1 )
done

#pitch_evaluate pitch_db/train/*.f0ref

exit 0

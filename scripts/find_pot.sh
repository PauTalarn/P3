
for th_pot in $(seq -45 0.01 -80 ); do
   echo -n "th_rmaxnorm=0.355 llindarPos=0.0052 llindarNeg=-0.0155 th_r1norm=0.694 th_pot=$th_pot"
   scripts/run_get_pitch.sh 0.355 0.0052 -0.0155 0.694 $th_pot  > /dev/null
   pitch_evaluate pitch_db/train/*.f0ref | fgrep TOTAL
                
done | sort -t: -k 2n;
exit 0
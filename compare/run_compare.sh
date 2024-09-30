#!/bin/bash

avl_exe="../build/compare/cmp_avl"
set_exe="../build/compare/cmp_set"
answer_file="compare.out"

touch $answer_file
echo -n > $answer_file

printf "file\t\t\t\t\t\t\tavl_time\tset_time\tratio\n" >> $answer_file

for file in ../tests/end_to_end/tests_in/test_*.in
do
    run_file="./${file}"
    avl_time=$($avl_exe < $run_file)
    set_time=$($set_exe < $run_file)
    ratio=`echo "scale=2; $set_time / $avl_time" | bc`

    echo $file
    printf "AVL time:\t\t%.6s\n" $avl_time
    printf "Set time:\t\t%.6s\n" $set_time
    printf "Ratio   :\t\t%.5s\n" $ratio
    echo

    printf "$file\t\t%.6s\t\t%.6s\t\t%s\n" $avl_time $set_time $ratio >> $answer_file
done
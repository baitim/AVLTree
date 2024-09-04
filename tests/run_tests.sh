#!/bin/bash

avl_exe="../build/tests/avl"
set_exe="../build/tests/set"
answer_file="tests_dat/answer.dat"

touch $answer_file
echo -n > $answer_file

printf "file\t\t\t\t\tavl_time\t\t\tset_time\t\t\tratio\n" >> $answer_file

for file in tests_dat/test_*.dat
do
    run_file="./${file}"
    avl_time=$($avl_exe < $run_file)
    set_time=$($set_exe < $run_file)
    ratio=`echo "scale=2; $avl_time / $set_time" | bc`

    echo $file
    printf "AVL time:\t\t%s\n" $avl_time
    printf "Set time:\t\t%s\n" $set_time
    printf "Ratio   :\t\t%s\n" $ratio
    echo

    printf "$file\t\t$avl_time\t\t$set_time\t\t%s\n" $ratio >> $answer_file
done
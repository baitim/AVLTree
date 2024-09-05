import os
import glob
import subprocess

avl_exe = "../../build/tests/end_to_end/avl"

answer_dir = "answers"
os.system("mkdir -p " + answer_dir)

test_num = 0
files = list(map(str, glob.glob("../tests_dat/test_*.in")))
files.sort()

for file in files :

    file_name = answer_dir + "/answer_" + f'{test_num+1:03}' + ".out"
    os.system("touch " + file_name)
    os.system("echo -n > " + file_name)
    ans_file = open(file_name, 'w')

    test_str = str(file) + "\n"

    command = avl_exe + " < " + file
    avl_answer = subprocess.check_output(command, shell=True).decode("utf-8")

    test_str += avl_answer

    ans_file.write(test_str)
    ans_file.close()
    test_num += 1
    print("test",  test_num, "passed")
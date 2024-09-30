import os
import glob
import subprocess
import matplotlib.pyplot as plt

cmp_avl_exe = "../build/compare/cmp_avl"
cmp_set_exe = "../build/compare/cmp_set"

files = list(map(str, glob.glob("bench_in/test_*.in")))
files.sort()

name_out_file = "compare.out"
os.system("touch " + name_out_file)
os.system("echo -n > " + name_out_file)
out_file = open(name_out_file, 'w')
out_file.write("file\t\t\t\t\tavl_time\tset_time\tratio\n")

test_num = 0
counts = []
avl_times = []
set_times = []
for file in files :

    with open(file, 'r') as fp:
        counts.append(sum(1 for line in fp))

    command = cmp_avl_exe + " < " + file
    avl_time = (float)(subprocess.check_output(command, shell=True).decode("utf-8"))

    command = cmp_set_exe + " < " + file
    set_time = (float)(subprocess.check_output(command, shell=True).decode("utf-8"))

    ratio = round(set_time / avl_time, 2)

    time_str = file + "\t" + f'{avl_time:<8}' + \
                      "\t" + f'{set_time:<8}' + \
                      "\t" + f'{ratio:<4}' + "\n"

    out_file.write(time_str)

    avl_times.append(avl_time * 1000)
    set_times.append(set_time * 1000)
    test_num += 1
    print("test",  test_num, "passed")

out_file.close()


plt.plot(counts, avl_times, marker='o', label='avl')
plt.plot(counts, set_times, marker='o', label='set')
plt.title("Times")

plt.xlabel('count')
plt.ylabel('time, ms')

plt.legend()
plt.savefig("times.png")
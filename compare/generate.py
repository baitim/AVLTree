import random
import os

to_curr_dir = os.path.dirname(os.path.realpath(__file__))
bench_in_dir = to_curr_dir + "/bench_in"
os.system("mkdir -p " + bench_in_dir)

counts = []
max_count  = (int)(1e5)
step_count = (int)(2 * 1e3)
for i in range(step_count, max_count, step_count):
    counts.append(i)

for test_num in range(0, len(counts)) :
    file_name = bench_in_dir + "/test_" + f'{test_num+1:03}' + ".in"
    file = open(file_name, 'w')

    count_inserts = counts[test_num]
    min_number    = 1
    max_number    = 1000000
    used_inserts  = 0

    count_ranges  = (int)(count_inserts / 2)
    min_range     = (int)(min_number - min_number * 0.01) - 1
    max_range     = (int)(max_number * 1.01) + 1
    used_ranges   = 0

    insert_chance = 0.75
    precision     = 10000

    count_parts = max(1, (int)(count_ranges / 100))
    sum = count_ranges + count_inserts
    split_sum = sum // count_parts
   # print("count = ", sum / 100000)
    k = 0
    for i in range(count_parts) :

        test_str = ""
        for j in range(split_sum) :
            k += 1
         #   if (k % 100000 == 0) :
           #     print("k = ", k / 100000)

            choice_request = random.randint(1, precision) / precision

            if (used_inserts < count_inserts and choice_request < insert_chance) :
                test_str += "k " + str(random.randint(1, max_number)) + "\n"
                used_inserts += 1
            else :
                if (used_ranges < count_ranges) :
                    first  = str(random.randint(min_number, max_number))
                    second = str(random.randint(min_number, max_number))

                    test_str += "q " + str(first) + " " + str(second) + "\n" 

                    used_ranges += 1

        file.write(test_str)

    l = 0
    for j in range(sum - k) :
        l += 1

        test_str = ""

        choice_request = random.randint(1, precision) / precision

        if (used_inserts < count_inserts and choice_request < insert_chance) :
            test_str += "k " + str(random.randint(1, max_number)) + "\n"
            used_inserts += 1
        else :
            if (used_ranges < count_ranges) :
                first  = str(random.randint(min_number, max_number))
                second = str(random.randint(min_number, max_number))

                test_str += "q " + str(first) + " " + str(second) + "\n" 

                used_ranges += 1

        file.write(test_str)
    
    file.close()
    print("test ", test_num + 1, " generated")
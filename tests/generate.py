import random
count_tests = 5

for i in range(0, count_tests) :
    file_name = "test_" + f'{i+1:02}' + ".dat"
    file = open(file_name, 'w')

    count_inserts = random.randint(100, 300)
    min_number    = 1
    max_number    = 200000
    used_inserts  = 0

    count_ranges  = random.randint(200, 400)
    min_range     = (int)(min_number - min_number * 0.01) - 1
    max_range     = (int)(max_number * 1.01) + 1
    used_ranges   = 0

    insert_chance = 0.75 * count_ranges / count_inserts
    precision     = 10000

    test_str = ""

    for j in range(count_inserts + count_ranges) :
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

    print("test ", i + 1, " generated")
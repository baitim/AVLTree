<h1 align="center">AVL tree</h1>

## Description

 Implementation of the AVL(Adelson-Velsky and Landis) tree and a comparison with the std::set.

## How to run

1. Clone <br>
    write <code>git clone https://github.com/baitim/AVL_tree.git</code> in terminal

2. Go to folder <br>
    write <code>cd AVL_tree</code> in terminal

3. Build <br>
    write <code>cmake . -B build ; cmake --build build</code> in terminal

4. Run <br>
    write <code>./build/tests/end_to_end/avl</code> in terminal <br>

## How to test

1. Prepare
    - Go to folder <br>
        write <code>cd tests</code> in terminal

    - Generate tests <br>
        write <code>python3 generate.py</code> in terminal

2. Testing
    - Compare <br>
        write <code>cd compare</code> in terminal <br>
        write <code>./run_compare.sh</code> in terminal <br>
        The result of the comparison will be in the "compare.out" file

    - End to end <br>
        write <code>cd end_to_end</code> in terminal <br>
        write <code>python3 run_end_to_end.py</code> in terminal <br>
        The test result will be in the "answers/answer_*.out" file

<p align="center"><img src="https://github.com/baitim/AVL_tree/blob/main/images/cat.gif" width="40%"></p>

## Support
**This project is created by [baitim](https://t.me/bai_tim)**
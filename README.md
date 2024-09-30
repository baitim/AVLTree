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
    write <code>./build/src/avl</code> in terminal <br>

## How to test

* Testing
    - End to end & Unit<br>
        in root dir write <code>ctest --test-dir build</code> in terminal <br>
        maybe you will need these flags for the ctest <code>--rerun-failed --output-on-failure</code>

## How to compare

* Preparing
    write <code>cd compare</code> in terminal <br>
    write <code>python3 generate.py</code> in terminal <br>

* Comparing
    write <code>python3 compare.py</code> in terminal <br>
    The result of the comparison will be in the "compare.out" and "times.png" file

<p align="center"><img src="https://github.com/baitim/AVL_tree/blob/main/compare/times.png" width="50%"></p>

<p align="center"><img src="https://github.com/baitim/AVL_tree/blob/main/images/cat.gif" width="40%"></p>

## Support
**This project is created by [baitim](https://t.me/bai_tim)**
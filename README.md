<h1 align="center">AVLTree</h1>

## Description

 Implementation of the AVL(Adelson-Velsky and Landis) tree and a comparison with the std::set.

## How to integrate
 
 use [storage](https://github.com/baitim/ConanPackages), project = "avl_tree", version = "1.0", user = "baitim"

## How to run

1. Clone <br>
    <code>git clone https://github.com/baitim/AVLTree.git</code>

2. Go to folder <br>
    <code>cd AVLTree</code>

3. Prepare conan <br>
    <code>uv sync --group dev; source .venv/bin/activate</code><br>
    <code>conan profile detect --force</code>

4. Init dependencies <br>
    <code>conan install . --build=missing -s compiler.cppstd=gnu20</code><br>
    maybe you will need these flags for the conan <code>-s build_type=Debug</code>

5. Build <br>
    <code>cmake --preset release</code><br>
    <code>cmake --build build/Release</code>

5. Run <br>
    write <code>./build/Release/src/avl</code> in terminal <br>

## How to test

* Testing
    - End to end & Unit<br>
        <code>ctest --test-dir build/Release --output-on-failure</code>

## How to compare

* Preparing
    write <code>cd compare</code> in terminal <br>
    write <code>python3 generate.py</code> in terminal <br>

* Comparing
    write <code>python3 compare.py</code> in terminal <br>
    The result of the comparison will be in the "compare.out" and "times.png" file

<figure>
    <p align="center"><img src="https://github.com/baitim/AVLTree/blob/main/compare/times.png" width="80%"></p>
    <figcaption><p align="center">i5-1135G7</p></figcaption>
</figure>

<p align="center"><img src="https://github.com/baitim/AVLTree/blob/main/images/cat.gif" width="40%"></p>

## Support
**This project is created by [baitim](https://t.me/bai_tim)**
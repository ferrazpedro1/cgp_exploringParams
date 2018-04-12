## CGP Library: Exploring Parameters to Optimize Digital Circuits Design

Juliana M. M. Araujo, Pedro A. P. Ferraz, Carlos A. P. S. Martins, Zélia M. A. Peixoto, Flávia M. F. Ferreira

### Introduction

This work uses the Cartesian Genetic Programming (CGP) Library, implemented by Andrew Turner (http://www.cgplibrary.co.uk) in C, to generate some different digital circuits. The goal is to run the GGP several times, changing the parameters in a specific interval, to evaluate the relationships between CGP parameters. So, it's possible to optimize the design and guide the user to configure the best parameters, given a truth table.

The tests and results are described in our paper (to be available when accepted).

We've used the G++ compiler and the script was written to work with bash.

### Usage

1. Clone our CGP repository, and we'll call the directory that you cloned CGP_Library into `$CGP_ROOT`
    ```Shell
    git clone https://github.com/ferrazpedro1/cgp_exploringParams
    ```
    
2. Build the project. You'll need to compile the C files on `$CGP_ROOT/src/`
    ```Shell
    cd $CGP_ROOT/
    make all
    ```
    
3. The main file is the bash script `$CGP_ROOT/run_CGP.sh`. There you'll find all the parameters that are customizable, as well as the input truth table file. Once configured, just run it:
    ```Shell
    cd $CGP_ROOT/
    ./run_CGP.sh
    ```
    
 4. The run_CGP.sh is configured to solve a 6-Bit Parity circuit. Other examples are inside `$CGP_ROOT/datasets/`. Most of them are from Andrew Turner files.

5. Output files, logs and .dot diagrams are saved on `$CGP_ROOT/output/` folder.

6. If you want to analyze the generated data, a CSV-type file is also saved in the same output directory. During our tests, we've analyzed the data using R.

---

If you have any suggestions, improvement or anything else regarding the project, please let us know!

Thank you!

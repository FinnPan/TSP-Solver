# TSP-Solver
## 1 GA-EAX

A Powerful TSP Solver using GA and EAX

Author: Yuichi Nagata

Code is copied and revised from repo: https://github.com/nagata-yuichi/GA-EAX.git

### COMPILE

Perform the following commands. Then, the executable file "jikken" will be created.

g++ -o jikken -O3 main.cpp env.cpp cross.cpp evaluator.cpp indi.cpp rand.cpp kopt.cpp sort.cpp -lm -I ../include/

### USAGE

It is recommended to use the Normal version. If the Normal version cannot work due to the lack of memory in your computer (e.g. 5.3GB (N = 25000)), use the Large version (e.g. 730MB (N = 25000)). The Large version will be about 1.3-1.5 times slower (see Appendix B of the Online Supplement of the paper).

Execute the following command to run the GA.
  ./jikken <integer1> <string1> <integer2> <integer3> <string2>

  Example: ./jikken 10 DATA 100 30 rat575.tsp

  Meaning of the arguments:

   - <integer1> = number of trials
   - <string1>  = filename to which results are written
   - <integer2> = number of population (300 is recommended if N > 10,000)
   - <integer3> = number of offspring solutions (30 is recommended)
   - <string2>  = instance filename (the instance must conform with TSPLIB format)

After the execution of the GA is finished, two files (DATA_Result and DATA_BestSol) will be generated.

  DATA_Result: A summary of the results is recorded in the following entry

  0 6773 173 0 3
  1 6773 174 0 3
  2 6773 166 0 3
  3 6773 173 0 3
  4 6773 173 0 3
  5 6773 171 0 3
  6 6773 182 0 3
  7 6773 168 0 3
  8 6773 173 0 3
  9 6773 173 0 3

  - 1st column: trial number
  - 2nd column: best tour length obtained in each run
  - 3rd column: the number of generations in each run
  - 4th column: the execution time (sec) for generating the initial population
  - 5th column: the execution time (sec) for each run of the GA


  DATA_BestSol: Best solutions are recorded for every run in the following entry

  575 6773
  1 24 25 26 27 28 29 52 50 51 74 73 72 49 48 47 70 71 93 94 116 ......
  ......
  ......

  - 1st line: number of cities, tour length
  - 2nd line: a solution representing a sequence of the cities
  - ....
  - ....

### ADDITIONAL USAGE

By using a function gEnv->WritePop() in the main function (main.cpp), all tours in the population at the end of each run are recorded to a file (DATA_POP_*), where * is the trial number. 

When you wish to run the GA, starting with a given initial population, execute the following command to run the GA, where <string3> is the filename of the initial population. The file "DATA_POP_0" obtained above is an example of the input file.

  ./jikken <integer1> <string1> <integer2> <integer3> <string2> <string3>

  Example: ./jikken 10 DATA2 100 30 rat575.tsp DATA_POP_0

We can change the configuration of the GA by modifying the functions TEnvironment::Init() and TerminationCondition() in env.cpp.

  Example1: Default setting
  fStage = 1;       /* Stage I */
  fFlagC[ 0 ] = 4;  /* Diversity preservation: 1:Greedy, 3:Distance, 4:Entropy */
  fFlagC[ 1 ] = 1;  /* Eset Type: 1:Single-AB, 2:Block2 */

  Example2: Only Stage II is performed using EAX with the Block2 strategy
  fStage = 2;       /* Stage I */
  fFlagC[ 0 ] = 4;  /* Diversity preservation: 1:Greedy, 3:Distance, 4:Entropy */
  fFlagC[ 1 ] = 2;  /* Eset Type: 1:Single-AB, 2:Block2 */

  Example3: The greedy selection is used instead of the entropy-preserving selection.
  fStage = 1;       /* Stage I */
  fFlagC[ 0 ] = 1;  /* Diversity preservation: 1:Greedy, 3:Distance, 4:Entropy */
  fFlagC[ 1 ] = 1;  /* Eset Type: 1:Single-AB, 2:Block2 */


When you wish to run the GA on a TSP instance with a specific definition of the edge length, please modify the function TEvaluator::SetInstance in evaluator.cpp

### PARAMETERS

You may wish to change some parameters of the default GA. I provide some tips on how to change several parameters.d

 - The termination condition (see Section 2.2) can be changed by modifying the function TerminationCondition() in env.cpp.
   
 - In cross.cpp, nearMax = 10 by default. This parameter corresponds to N_{near} in the paper (see Step 5.3 of the EAX algorithm in Section 3.1). If the distance matrix of an input TSP instance dose not follow the 2D Euclidean distance or similar ones (e.g., a random distance matrix), you may be able to get better results by increasing the value of this parameter.

## 2 LKH3

An Effective Implementation of the Lin-Kernighan Traveling Salesman Heuristic

Author: Keld Helsgaun

Code is copied and revised from: [LKH-3 (Keld Helsgaun) (ruc.dk)](http://webhotel4.ruc.dk/~keld/research/LKH-3/)


### INSTRUCTIONS FOR INSTALLATION

To test the installation run the program by typing ./LKH pr2392.par. Then press return. The program should now solve a TSP instance with 2392 nodes.

For testing the installation on an mTSP problem, type ./LKH whizzkids96.par. Then press return.

A two-level tree is used as the default tour representation. A three-level tree representation may be used instead by compiling the source code with the compiler option 

	-DTHREE_LEVEL_TREE

Just edit the first line in SRC/Makefile and execute the commands

	make clean
	make

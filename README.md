# Rubik's Cube Solver

```shell-session
$ g++ -O2 -march=native rubiksolver.cpp -o rubiksolver
$ ./rubiksolver
Make pattern database
corner: 0 1 2 3 4 5 6 7 8 9 10 11
edge1: 0 1 2 3 4 5 6 7 8 9 10 11
edge2: 0 1 2 3 4 5 6 7 8 9 10 11
777517 [ms]
Input cube:
   O--
   X#-
   XOO
###/#./O#XX/
XX/.-/OO#-//
#/X-.X.X.-O-
   .-/
   #..
   O.O
depth: 0, node number: 1
depth: 1, node number: 1
depth: 2, node number: 1
depth: 3, node number: 1
depth: 4, node number: 1
depth: 5, node number: 1
depth: 6, node number: 1
depth: 7, node number: 1
depth: 8, node number: 1
depth: 9, node number: 19
depth: 10, node number: 418
depth: 11, node number: 5293
depth: 12, node number: 79759
depth: 13, node number: 1087591
depth: 14, node number: 14767327
depth: 15, node number: 198212410
depth: 16, node number: 2651352040
depth: 17, node number: 35381609965
depth: 18, node number: 106778784205
21742657 [ms]
18 moves
Solve: B D L B L U2 R' F2 D' B R' B2 U2 D' F D2 R' D
Generate: D' R D2 F' D U2 B2 R B' D F2 R U2 L' B' L' D' B'
Input cube:
   GWB
   YYO
   WGG
ROGRWOYWWRRY
YRRGGRYOBYBG
OBRBBBWROYGG
   YWO
   OWB
   WOB
depth: 0, node number: 1
depth: 1, node number: 1
depth: 2, node number: 1
depth: 3, node number: 1
depth: 4, node number: 1
depth: 5, node number: 1
depth: 6, node number: 1
depth: 7, node number: 1
depth: 8, node number: 1
depth: 9, node number: 19
depth: 10, node number: 568
depth: 11, node number: 7063
depth: 12, node number: 99613
depth: 13, node number: 1278469
depth: 14, node number: 16661782
depth: 15, node number: 216514999
depth: 16, node number: 2826025927
depth: 17, node number: 37005351460
depth: 18, node number: 324194983533
53568666 [ms]
18 moves
Solve: R' D2 F L2 B2 R' D B R F2 U B R U2 D2 L D2 F
Generate: F' D2 L' D2 U2 R' B' U' F2 R' B' D' R B2 L2 F' D2 R
Input cube:
   BUD
   FUU
   BFL
RLLUDDFLBRFU
BLFRFBLRRUBD
FBLUBFRRLDDR
   FRD
   UDD
   ULB
depth: 0, node number: 1
depth: 1, node number: 1
depth: 2, node number: 1
depth: 3, node number: 1
depth: 4, node number: 1
depth: 5, node number: 1
depth: 6, node number: 1
depth: 7, node number: 1
depth: 8, node number: 1
depth: 9, node number: 34
depth: 10, node number: 196
depth: 11, node number: 4219
depth: 12, node number: 65233
depth: 13, node number: 943639
depth: 14, node number: 13189741
depth: 15, node number: 181491331
depth: 16, node number: 2473588348
depth: 17, node number: 33489239401
depth: 18, node number: 39767360365
11381512 [ms]
18 moves
Solve: F2 L2 R D F' U F' B2 U D2 L R' B D B2 D R B2
Generate: B2 R' D' B2 D' B' R L' D2 U' B2 F U' F D' R' L2 F2
Input cube:
   #/#
   X#O
   #-#
X#X-#-O#O/#/
/X-X-O-O/O/X
X.X-.-O.O/./
   .-.
   X.O
   ./.
depth: 0, node number: 1
depth: 1, node number: 1
depth: 2, node number: 1
depth: 3, node number: 1
depth: 4, node number: 1
depth: 5, node number: 1
depth: 6, node number: 1
depth: 7, node number: 1
depth: 8, node number: 1
depth: 9, node number: 1
depth: 10, node number: 259
depth: 11, node number: 5860
depth: 12, node number: 105961
depth: 13, node number: 1581682
depth: 14, node number: 22523503
depth: 15, node number: 298754251
depth: 16, node number: 3990825268
depth: 17, node number: 49035661192
depth: 18, node number: 606119022976
depth: 19, node number: 7503007200421
depth: 20, node number: 3631245242
1355758420 [ms]
20 moves
Solve: F B L F2 L U2 R F2 L U' D' L2 B L' R F2 U2 B2 U L2
Generate: L2 U' B2 U2 F2 R' L B' L2 D U L' F2 R' U2 L' F2 L' B' F'
Input cube:
```

## References

- Richard E. Korf, Finding Optimal Solutions to Rubik's Cube Using Pattern Databases
- [Implementing an Optimal Rubik's Cube Solver using Korf’s Algorithm \| by Ben Botto \| Medium](https://medium.com/@benjamin.botto/implementing-an-optimal-rubiks-cube-solver-using-korf-s-algorithm-bf750b332cf9)

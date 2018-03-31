# graphs :balloon:
This code was created by Wladimir Gramacho and Leonardo de Almeida. This is the implementation of the 1st project of the course Graph: Theory and Applications @ UnB during the 1st semester of 2018.

----
The file karate.gml contains the network of friendships between the 34
members of a karate club at a US university, as described by Wayne Zachary
in 1977.  If you use these data in your work, please cite W. W. Zachary, An
information flow model for conflict and fission in small groups, Journal of
Anthropological Research 33, 452-473 (1977).

-----

## How to run :runner:
To run the code, you must first compile the file `main.cpp`. The recommended compiler is the **g++**. To compile the program, run:

```
g++ main.cpp -std=c++11
```

:bangbang: The `-std=c++11` is mandatory!

The previous command will create the default executable, `a.out`. For this program, you must use the file `karate.gml` as the argument when you run the program. To do so, you must run the command:

```
./a.out karate.gml
```

A simple solver for the game 'iq stixx', hacked together in an afternoon, then
ported to C++, which runs ~28x faster.

compile: g++ main.cpp -o sl

Usage: ./sl AB_CD 1,2,12,_,_
Usage: python solver.py AB_CD 1,2,12,_,_
    rows: as string (each row being one char) from top-left to bottom-left.
    columns: as comma separated list of numbers, from top-left to top-right.
    blank spaces are represented by underscores.

LICENSE: MIT, see LICENSE file

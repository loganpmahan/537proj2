# 537proj2

Programming assignment 2 for CS 537 at UW-Madison

Assignment details: http://pages.cs.wisc.edu/~bart/537/programs/program2.html

# About

We decided to use monitors as our method of sycronization as we thought it would be the cleaner solution. 

The queue structure itself is based off of examples shown in class, using an array with first and last pointers to detect whether the queue is full or empty.

Since the Reader, Munch1, Munch2 and Writer functions were relatively simple and small, we decided to just keep them in the main.c file to keep down on extra source/header files. 

We used strchr() to replace each space with with an "*" and toUpper() to change lowercase to upper.

# Usage

prodcomm < [input.txt] > [output.txt]

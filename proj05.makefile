# Project 5 #

proj05: proj05.student.o
	g++ proj05.student.o /user/cse410/McCullen/Projects/project05.lib.o -o proj05

proj05.student.o: proj05.student.c
	g++ -std=c++11 -Wall -c proj05.student.c

clean:
	rmx -f proj05 proj05.student.o

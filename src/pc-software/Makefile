all: main.o receive.o dcf39.o versacom.o
	gcc -Wall ./*.o -o dcf39 -lcodebananas 

main.o: main.c
	gcc -Wall -c main.c

receive.o: receive.c
	gcc -Wall -c receive.c

dcf39.o: dcf39.c
	gcc -Wall -c dcf39.c

versacom.o: versacom.c
	gcc -Wall -c versacom.c

clean:
	rm ./*.o
	rm ./dcf39

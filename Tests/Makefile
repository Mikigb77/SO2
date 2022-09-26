all: main

cc = gcc
flags = -O3

main: main.o
	$(cc) $(flags) $< -o $@

%.o: %.c
	$(cc) -c $(flags) $< -o $@

.PHONY: clean

clean:
	rm -f ./*.o main

EXE = adj.exe
OBJ = arp.o mac.o main.o adj.o
SRC = arp.c mac.c main.c adj.c

$(EXE) : $(OBJ)
	gcc -o $(EXE) $^

%.o:%.c
	gcc -c $< -o $@

.PHONY:clean
clean:
	rm -rf *.exe *.o

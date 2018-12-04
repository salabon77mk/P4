CC=gcc
SCAN_BUILD_DR = scan-build-out
CFLAGS= -g -Wall -Wextra -O0
DEPS= 537malloc.h mem_tree.h
OBJ= 537malloc.o mem_tree.o 
EXE=output

all: $(OBJ)
	$(CC) -o $(EXE) $(CFLAGS) $(OBJ)

# Include all your .o files in the below rule
obj: 537malloc.o mem_tree.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
.PHONY: clean
	
clean:
	rm *.o $(EXE)
	
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html


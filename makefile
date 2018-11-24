CC=gcc
CFLAGS= -g -Wall -Wextra 
DEPS = 537malloc.h mem_tree.h  
OBJ = driver.o 537malloc.o mem_tree.o 
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
driver: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
	
clean:
	rm *.o driver
	


EXEC    = mem_mg 
INCLUDE = memory_mg.h
SOURCE  = main.c memory_mg.c  

all: 
	gcc -o $(EXEC) $(SOURCE) $(INCLUDE)

clean:
	 rm mem_mg
	 rm *.o

CC := g++
CFLAGS := -c
MKDIR_P := mkdir -p
OUT_DIR := build
OUTPUT := laravel-search-lang
CLEAN := rm -rf *.o

all: compile

compile: main.o ArgumentsReader.o Output.o Parser.o Utils.o
	${MKDIR_P} $(OUT_DIR)
	$(CC) main.o ArgumentsReader.o Output.o Parser.o Utils.o -o $(OUT_DIR)/$(OUTPUT)
	${CLEAN}

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

ArgumentsReader.o: ArgumentsReader.cpp
	$(CC) $(CFLAGS) ArgumentsReader.cpp

Output.o: Output.cpp
	$(CC) $(CFLAGS) Output.cpp

Parser.o: Parser.cpp
	$(CC) $(CFLAGS) Parser.cpp

Utils.o: Utils.cpp
	$(CC) $(CFLAGS) Utils.cpp

clean:
	${CLEAN}


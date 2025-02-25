SRC = $(wildcard src/*.c)
HEADERS := $(wildcard src/include/*.h)
CC=clang
DEBUG = -ggdb -DYYDEBUG=1
CFLAGS := -O3 -ffast-math -I src/include -std=c2x -pedantic -Wall -Wextra -Wshadow -Wunreachable-code -Wno-unused-parameter
BIN := bin/mltt
mltt: bin/mltt
$(BIN): $(SRC) $(HEADERS) | src/parser.tab.c src/include/parser.tab.h src/lex.yy.c
	mkdir -p bin 
	$(CC) $(CFLAGS) $(SRC) $(DEBUG_FLAGS) -o $(BIN)
src/parser.tab.c src/include/parser.tab.h: src/parser.y
	bison -t -d src/parser.y
	mv parser.tab.c src
	mv parser.tab.h src/include
src/lex.yy.c: src/lexer.l src/include/parser.tab.h
	flex -o src/lex.yy.c src/lexer.l
clean:
	rm -rf bin
	rm -f src/parser.tab.c src/lex.yy.c src/include/parser.tab.h

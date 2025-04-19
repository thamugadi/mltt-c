SRC = $(wildcard src/*.c)
HEADERS := $(wildcard src/include/*.h)
CC=clang
DEBUG = -ggdb -DYYDEBUG=1
CFLAGS := -O3 -ffast-math -fno-stack-protector -no-pie  -I src/include -std=c2x -pedantic -Wall -Wextra -Wshadow -Wunreachable-code -Wno-unused-parameter -Wno-unused-label -Wno-constant-logical-operand 

EXPORTED_FUNCTIONS = -s EXPORTED_FUNCTIONS='["_eval_program", "_free_globals", "_init_hashmap", "_free"]'
EXPORTED_RUNTIME_METHODS = -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap", "stringToNewUTF8", "UTF8ToString", "lengthBytesUTF8"]'

EMCCFLAGS := -s ALLOW_MEMORY_GROWTH=1 -s SINGLE_FILE=1 -O3 -ffast-math -I src/include -std=c2x -pedantic -Wall -Wextra -Wshadow -Wunreachable-code -Wno-unused-parameter -s WASM=1 $(EXPORTED_FUNCTIONS) $(EXPORTED_RUNTIME_METHODS)

BIN := bin/mltt
mltt: bin/mltt
$(BIN): $(SRC) $(HEADERS) | src/parser.tab.c src/include/parser.tab.h src/lex.yy.c
	mkdir -p bin 
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)
src/parser.tab.c src/include/parser.tab.h: src/parser.y
	bison -t -d src/parser.y
	mv parser.tab.c src
	mv parser.tab.h src/include
src/lex.yy.c: src/lexer.l src/include/parser.tab.h
	flex -o src/lex.yy.c src/lexer.l
clean:
	rm -rf bin
	rm -f src/parser.tab.c src/lex.yy.c src/include/parser.tab.h

web : bin/mltt.js

bin/mltt.js : $(SRC) $(HEADERS) | src/parser.tab.c src/include/parser.tab.h src/lex.yy.c
	mkdir -p bin
	emcc $(EMCCFLAGS) $(SRC) -o bin/mltt.js 

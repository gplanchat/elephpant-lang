CC=g++
CFLAGS=-W -Wall -std=c++1y -ferror-limit=1 -DBOOST_SPIRIT_USE_PHOENIX_V3
LDFLAGS=-stdlib=libc++
EXEC=rephp
TEST_EXEC=run-tests

ENGINE=engine/engine.o engine/visitors.o engine/parser/ast.o
LANG=lang/standard.o lang/stream.o
TESTS=tests/main.o tests/tests.o tests/scalar.o tests/string.o $(ENGINE) $(LANG)
OBJ=main.o $(ENGINE) $(LANG)


all: $(EXEC)

test: $(TESTS)
	$(CC) -o $(TEST_EXEC) $^ $(LDFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

%.o: %.cpp
	$(CC)  -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(OBJ) $(TESTS)

mrproper: clean
	rm -rf $(EXEC)
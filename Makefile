CC=g++
CFLAGS=-W -Wall -std=c++1y -ferror-limit=10
LDFLAGS=-stdlib=libc++
EXEC=rephp
OBJ=main.o engine/engine.o lang/standard.o lang/stream.o

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $^ $(LDFLAGS)

%.o: %.cpp
	$(CC)  -o $@ -c $< $(CFLAGS)

clean:
	rm -rf $(OBJ)

mrproper: clean
	rm -rf $(EXEC)
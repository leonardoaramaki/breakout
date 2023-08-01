.PHONY = clean

CC=g++
override CXXFLAGS+=-g -std=c++17 -Isrc/ -Isrc/thirdparty
LDFLAGS=-lSDL2 -ldl
SRC=src/thirdparty/glad/glad.c src/*.cpp

prog: $(SRC)
	$(CC) $(CXXFLAGS) $(SRC) -o $@ $(LDFLAGS)

run: prog
	./prog

clean:
	rm -f prog

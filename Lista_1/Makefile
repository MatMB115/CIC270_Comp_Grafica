# Martins's Makefile

CC = g++
CFLAGS = -pedantic-errors -Wall -g
LDFLASGS = -lglut -lGLEW -lGL

FONTES = $(wildcard *.cpp)

OBJETOS = $(FONTES:.cpp=.o)

EXECUTAVEL = main.exe

all: $(EXECUTAVEL)
	./$(EXECUTAVEL)

$(EXECUTAVEL) : $(OBJETOS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLASGS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) $^

clean:
	rm -rf *.o

rmproper: clean
	rm -rf $(EXECUTAVEL)

run: $(EXECUTAVEL)

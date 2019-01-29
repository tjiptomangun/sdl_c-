CXX = g++
SDL_LIB = -lSDL2
SDL_INCLUDE = -I/usr/local/include
#CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE)
CXXFLAGS = -Wall -c -std=c++11 -Wextra -pedantic-errors $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)
EXE = main

all: $(EXE)


$(EXE): main.o

	$(CXX) $< $(LDFLAGS) -o $@

hello.o: main.cpp

	$(CXX) $(CXXFLAGS) $< -o $@

clean:

	rm *.o && rm $(EXE)

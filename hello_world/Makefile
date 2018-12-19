CXX = g++
SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/local/include
CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)
EXE = hello

all: $(EXE)


$(EXE): hello.o

	$(CXX) $< $(LDFLAGS) -o $@

hello.o: hello.cpp

	$(CXX) $(CXXFLAGS) $< -o $@

clean:

	rm *.o && rm $(EXE)

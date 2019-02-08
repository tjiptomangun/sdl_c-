CXX = g++
SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/local/include -I .
CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)
EXE = unmain

all: $(EXE)


$(EXE): unmain.o 
	$(CXX) $< $(LDFLAGS) -o $@

cleanup.o : cleanup.cpp 
	$(CXX) $(CXXFLAGS) $< -o $@
clean:

	rm *.o && rm $(EXE)

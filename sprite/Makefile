CXX = g++
SDL_LIB = -L/usr/local/lib -lSDL2 -lSDL2_image -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/local/include -I .
CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)
EXE = sprite

all: $(EXE)


$(EXE): sprite.o 
	$(CXX) $< $(LDFLAGS) -o $@

clean:

	rm *.o && rm $(EXE)

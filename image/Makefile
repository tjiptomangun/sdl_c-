CXX = g++
SDL_LIB = -L/usr/local/lib -lSDL2 -lSDL2_image -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/local/include -I .
CXXFLAGS = -Wall -c -std=c++11 $(SDL_INCLUDE)
LDFLAGS = $(SDL_LIB)
EXE = image

all: $(EXE)


$(EXE): image.o 
	$(CXX) $< $(LDFLAGS) -o $@

cleanup.o : cleanup.cpp 
	$(CXX) $(CXXFLAGS) $< -o $@
clean:

	rm *.o && rm $(EXE)

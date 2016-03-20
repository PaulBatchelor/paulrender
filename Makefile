
# flags for gprof... rename to a.out  before running "gprof" 
# CXXFLAGS=-ggdb -g3 -pg -O0 

LDFLAGS=-lm

%.o: %.cpp
	g++ -c $(CXXFLAGS) $<

paulrender: main.cpp tgaimage.o model.o
	g++ $(CXXFLAGS) $(LDFLAGS) main.cpp tgaimage.o model.o -o $@

clean:
	rm -rf out.tga paulrender *.o

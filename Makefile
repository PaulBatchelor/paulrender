%.o: %.cpp
	g++ -c $<

paulrender: main.cpp tgaimage.o
	g++ main.cpp tgaimage.o -o $@

clean:
	rm -rf out.tga paulrender *.o

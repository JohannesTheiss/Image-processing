output: main.o Image.o
	g++ main.o Image.o -o output

main.o: main.cpp
	g++ -c main.cpp

Image.o: Image.cpp
	g++ -c Image.cpp -lm

clean:
	rm *.o output
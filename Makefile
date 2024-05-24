all: 
	bear -- g++ -std=c++20 -Ivulkan -I./include/ ./src/*.cpp -o program -lvulkan `pkg-config --libs --cflags glfw3`

clean: 
	rm -f program
	rm -f *.o

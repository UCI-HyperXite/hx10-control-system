all: main

main: main.cpp signal_light.o
	g++ main.cpp signal_light.o -std=c++17 -o main -L/usr/local/lib -lwiringPi -lboost_json -lboost_system -lboost_thread -lpthread -Iinclude

signal_light.o: signal_light.hpp signal_light.cpp
	g++ -c signal_light.cpp -std=c++17 -o signal_light.o -Iinclude

clean:
	rm -f *.o main

CPP = g++

all: formatinformation.bin main.exe

main.exe: main.cpp
	g++ -std=c++11 -O3 -Wall -Werror $< -o $@

formatinformation.exe: formatinformation.cpp
	g++ -std=c++11 -O3 -Wall -Werror -pthread -lpthread $< -o $@

formatinformation.bin: formatinformation.exe
	./$< $@ `nproc || echo 4`

clean:
	rm -f *.exe formatinformation.bin

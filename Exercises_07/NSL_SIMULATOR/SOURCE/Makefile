CC = g++
CFLAGS = -O3 --std=c++14 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -larmadillo

all: simulator.exe 

search: nve_temp_search.exe

simulator.exe : system.o NSL_SIMULATOR.o particle.o random.o
	$(CC) system.o particle.o random.o NSL_SIMULATOR.o -o simulator.exe $(LDFLAGS)

nve_temp_search.exe : system.o nve_temp_search.o particle.o random.o
	$(CC) system.o particle.o random.o nve_temp_search.o -o nve_temp_search.exe $(LDFLAGS)

NSL_SIMULATOR.o : NSL_SIMULATOR.cpp system.h particle.h
	$(CC) -c NSL_SIMULATOR.cpp -o NSL_SIMULATOR.o $(CFLAGS)

nve_temp_search.o : nve_temp_search.cpp system.h particle.h
	$(CC) -c nve_temp_search.cpp -o nve_temp_search.o $(CFLAGS)

system.o : system.cpp system.h random.h particle.h
	$(CC) -c system.cpp -o system.o $(CFLAGS)

particle.o : particle.cpp particle.h
	$(CC) -c particle.cpp -o particle.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

clean :
	rm -f *.o simulator.exe

remove:
	rm -f ../OUTPUT/*.*
	rm -f ../OUTPUT/CONFIG/*.*

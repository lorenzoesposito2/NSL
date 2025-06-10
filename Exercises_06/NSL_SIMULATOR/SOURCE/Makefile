CC = g++
CFLAGS = -O3 --std=c++14 -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -larmadillo

all: simulator.exe equilibration.exe

simulator.exe : system.o NSL_SIMULATOR.o particle.o random.o
	$(CC) system.o particle.o random.o NSL_SIMULATOR.o -o simulator.exe $(LDFLAGS)

equilibration.exe : system.o equilibration.o particle.o random.o
	$(CC) system.o particle.o random.o equilibration.o -o equilibration.exe $(LDFLAGS)

NSL_SIMULATOR.o : NSL_SIMULATOR.cpp system.h particle.h
	$(CC) -c NSL_SIMULATOR.cpp -o NSL_SIMULATOR.o $(CFLAGS)

equilibration.o : equilibration.cpp system.h
	$(CC) -c equilibration.cpp -o equilibration.o $(CFLAGS)

system.o : system.cpp system.h random.h particle.h
	$(CC) -c system.cpp -o system.o $(CFLAGS)

particle.o : particle.cpp particle.h
	$(CC) -c particle.cpp -o particle.o $(CFLAGS)

random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)

clean :
	rm -f *.o simulator.exe equilibration.exe

remove:
	rm -f ../OUTPUT/*.*
	rm -f ../OUTPUT/CONFIG/*.*

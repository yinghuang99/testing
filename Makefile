CC      = mpicc
CFLAGS  = -fopenmp

default: all

all: driver

utility.o:utility.c
	$(CC) -c utility.c

base.o:base.c
	$(CC) -c base.c

sensor.o:sensor.c
	$(CC) -c sensor.c

altimeter.o:altimeter.c
	$(CC) -c altimeter.c

driver:driver.c utility.o base.o sensor.o altimeter.o
	$(CC) $(CFLAGS) driver.c -o driver  utility.o base.o sensor.o altimeter.o

exec:
	mpiexec -np 10 ./driver 3 3

clean:
	rm driver *.o
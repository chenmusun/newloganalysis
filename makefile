CC=g++
CFLAGS= -std=c++11
objects = test.o log_analysis.o  log_analysis_manager.o

all:$(objects)
	g++   -o ieeventanalysis -std=c++11 $(objects) -pthread -lzmq
#-Wl,-dn -std=c++11 -levent -lnettle -lz -luuid -Wl,-dy -lpthread
$(objects): %.o: %.cpp
	$(CC) -c -g $(CFLAGS) $< -o $@
clean:
	rm -f *.o ieeventanalysis
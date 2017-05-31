ifeq ($(DEBUG), true)
	PARAM = -ggdb3 -Wall -std=c++11 -fopenmp
else
	PARAM = -O3 -DNDEBUG -std=c++11 -fopenmp
endif

SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=%.o)
BACKENDOBJ = $(filter-out genmesh.o debug.o main.o, $(OBJ))

all : genmesh debug main

main : .depend main.o $(BACKENDOBJ)
	g++ main.o $(BACKENDOBJ) -o $@ $(PARAM) -lopencv_core -lopencv_imgproc -lopencv_imgcodecs

genmesh : .depend genmesh.o $(BACKENDOBJ)
	g++ genmesh.o $(BACKENDOBJ) -o $@ $(PARAM)

debug : .depend debug.o $(BACKENDOBJ)
	g++ debug.o $(BACKENDOBJ) -o $@ $(PARAM)

.depend : $(SRC)
	rm -f .depend
	g++ $(PARAM) -MM $^ > .depend

-include .depend

%.o : %.cpp
	g++ $< -c $(PARAM)

clean :
	-rm *.o *.gch .depend main genmesh debug

.PHONY : clean

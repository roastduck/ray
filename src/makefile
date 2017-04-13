ifeq ($(DEBUG), true)
	PARAM = -g -Wall -std=c++11
else
	PARAM = -O2 -DNDEBUG -std=c++11
endif

SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=%.o)
BACKENDOBJ = $(filter-out genmesh.o debug.o, $(OBJ))

all : genmesh debug

genmesh : .depend genmesh.o $(BACKENDOBJ)
	g++ genmesh.o $(BACKENDOBJ) -o $@ $(PARAM)

debug : .depend debug.o $(BACKENDOBJ)
	g++ debug.o $(BACKENDOBJ) -o $@ $(PARAM)

.depend : $(SRC)
	rm -f .depend
	g++ $(PARAM) -MM $^ > .depend;

-include .depend

%.o : %.cpp
	g++ $< -c $(PARAM)

clean :
	-rm *.o *.gch .depend main

.PHONY : clean

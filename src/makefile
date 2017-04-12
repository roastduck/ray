ifeq ($(DEBUG), true)
	PARAM = -g -Wall -std=c++11
else
	PARAM = -O2 -DNDEBUG -std=c++11
endif

SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=%.o)

genmesh : .depend $(OBJ)
	g++ $(OBJ) -o $@ $(PARAM)

.depend : $(SRC)
	rm -f .depend
	g++ $(PARAM) -MM $^ > .depend;

-include .depend

%.o : %.cpp
	g++ $< -c $(PARAM)

clean :
	-rm *.o *.gch .depend main

.PHONY : clean

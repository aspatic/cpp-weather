PROJECTHOMEDIR = /home/yche/cpp-weather
PUBINCL = -I$(PROJECTHOMEDIR)/public
PUBCPP = $(PROJECTHOMEDIR)/public/_public.cpp

all:crtsurfdata

crtsurfdata:crtsurfdata.cpp
	g++ -g -o crtsurfdata crtsurfdata.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp crtsurfdata ../bin/.

clean:
	rm crtsurfdata
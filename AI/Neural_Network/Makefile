
CXXFLAGS = -Wall -O3 -I ../include
Nombre = Neural_Network

all: $(Nombre)

$(Nombre): objs/$(Nombre).o
	g++ $(CXXFLAGS) -o $(Nombre) objs/$(Nombre).o 

objs/$(Nombre).o: $(Nombre).cpp
	g++ $(CXXFLAGS) -c -o objs/$(Nombre).o $(Nombre).cpp 

clean: 
	rm objs/$(Nombre).o 
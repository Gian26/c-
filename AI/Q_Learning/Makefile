#qllib_path=../qllib

all: ql

ql: multiAgents.o
	g++ multiAgents.o -o multiAgents

ejemplo1.o: multiAgents.cpp
	g++ -c multiAgents.cpp

#ql.o: $(qllib_path)/ql.cpp
#	g++ -c $(qllib_path)/ql.cpp

clean:
	rm -rf *o ql

all: ql

ql: main_ql_multiagents.o multiAgents.o
	g++ -o ql main_ql_multiagents.o multiAgents.o 

main_ql_multiagents.o: main_ql_multiagents.cpp
	g++ -c main_ql_multiagents.cpp

multiAgents.o: multiAgents.cpp multiAgents.h
	g++ -c multiAgents.cpp

#ql.o: $(qllib_path)/ql.cpp
#	g++ -c $(qllib_path)/ql.cpp

clean:
	rm -rf *.o ql

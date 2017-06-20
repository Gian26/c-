
qlswarm: mainSwarmingAgents.o swarmingAgents.o
	g++ mainSwarmingAgents.o swarmingAgents.o -o qlswarm

mainSwarmingAgents.o: mainSwarmingAgents.cpp
	g++ -std=c++11 -c mainSwarmingAgents.cpp

swarmingAgents.o: swarmingAgents.cpp swarmingAgents.h
	g++ -std=c++11 -c swarmingAgents.cpp

#ql.o: $(qllib_path)/ql.cpp
#	g++ -c $(qllib_path)/ql.cpp

clean:
	rm -rf *.o qlswarm

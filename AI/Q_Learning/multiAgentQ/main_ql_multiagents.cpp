#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <numeric>
#include "multiAgents.h"


/// ALGORITMO Q-LEARNING
const int _SIZEOFSWARM = 3;
const int MAX_EPISODES = 1000;
const int MAX_NUM_STEP = 10;
const unsigned _WID = 10;
const unsigned _LEN = 3;
const unsigned short _numStates = _WID * _LEN;
const unsigned short _numActions = 8;
const float _alpha = 0.5;
const float _gamma = 1.0;
const float _epsilon = 0.10;

using namespace std;

int main(int argc, char const *argv[]) {

  const unsigned short EMPTY = 0,OCCUPIED = 1, VISITED = 2;
  unsigned short CURRENT_STATE_AGENTA  =0,CURRENT_STATE_AGENTB = 10,CURRENT_STATE_AGENTC = 20;
  printf("Create world with dimensions %d x %d\n",_WID,_LEN);
  GridWorld mundo(_WID,_LEN,_numActions);

  printf("Creating 2 agents\n");
   //
  // for (unsigned short i = 0; i < _SIZEOFSWARM; i++) {
  //   //agent a
  //   Agent AgentA(_numActions,_alpha,_gamma,_epsilon,0,0);
  //   unsigned short state = 0;
  //   AgentA.setCurrentState(CURRENT_STATE_AGENTA);
  //   mundo.setCellState(CURRENT_STATE_AGENTA,OCCUPIED);
  //
  //   swarm.push_back(AgentA);
  //
  // }
  //
  // cout << swarm.size()<< endl;

  //agent a
  Agent AgentA(_numActions,_alpha,_gamma,_epsilon,0,0);
   AgentA.setCurrentState(CURRENT_STATE_AGENTA);
  mundo.setCellState(CURRENT_STATE_AGENTA,OCCUPIED);

  //agent b
  Agent AgentB(_numActions,_alpha,_gamma,_epsilon,0,0);
  AgentB.setCurrentState(CURRENT_STATE_AGENTB);
  mundo.setCellState(CURRENT_STATE_AGENTB,OCCUPIED);

  //agent b
  Agent AgentC(_numActions,_alpha,_gamma,_epsilon,0,0);
  AgentB.setCurrentState(CURRENT_STATE_AGENTC);
  mundo.setCellState(CURRENT_STATE_AGENTC,OCCUPIED);

  AgentA.setGoalState(9);
  AgentB.setGoalState(19);
  AgentC.setGoalState(29);

  // mundo.setCellState(9,GOAL);
  // mundo.setCellState(19,GOAL);
  // mundo.setCellState(29,GOAL);

  //creating a vector structure to hold AGENTS
  vector<Agent> swarm;
  int swarm_size = 3;
  swarm.push_back(AgentA);
  swarm.push_back(AgentB);
  swarm.push_back(AgentC);


  // int move = 8;
  //
  // cout<<"AGENT A STATE: "<<CURRENT_STATE_AGENTA<< " DIRECTION "<< move<<"() MOVE " << AgentA.move(mundo.getWorld(),move,_WID,_LEN)<< endl;


  //TRAINING
  cout<< "START TRAINING!"<< endl;
  vector<unsigned short> flagAgents(swarm.size());
  // // iterate through episodes
  for (unsigned short episode = 0; episode < MAX_EPISODES; episode++) {
    //     //iterate through swarm
    // cout << endl << endl << " Episodio " << episode+1  << endl ;
    // cout << endl << "____________________________________________________________________";
    // cout << endl << "\t s \t" << "\t a \t" << "\t s' \t" << "\t r \t";
    // cout << endl << "____________________________________________________________________";
    // printf("\n");

      for (unsigned short step = 0; step < MAX_NUM_STEP; step++) {
        for (unsigned short agent = 0; agent < swarm_size; agent++) {
            //iterate through number of steps possible.
            if(!flagAgents[agent]){
              unsigned short _currentState = swarm[agent].getCurrentState();
              CellSpaces _currentStateObject =mundo.getCellSpace(_currentState);
              unsigned short _currentAction = swarm[agent].epsGreedyAction(mundo.getQVector()[_currentState]);
              mundo.setCellState(_currentState,VISITED);
              unsigned short _nextState = swarm[agent].move(mundo.getWorld(),_currentAction,_WID,_LEN);
              //change state of actual space and next space
              //ask if the new state is occupied or empty

              CellSpaces _nextStateObject =mundo.getCellSpace(_nextState);
              float _reward = swarm[agent].calculateReward(_nextStateObject);

              if( _nextStateObject.getState() != OCCUPIED)
                mundo.setCellState(_nextState,OCCUPIED);
              else{
                  _nextState = _currentState;
              }

              // cout<< "Agent "<<agent << "),  step " << step+1 << ")\t" << _currentState << "\t | \t " << _currentAction << "\t | \t " << _nextState <<  "\t | \t "<< _reward<<"\n"<<endl;

              unsigned short _nextAction = swarm[agent].maxAction(mundo.getQVector()[_nextState]);
              //Calculate Q
              float actualQ = mundo.getQVector()[_currentState][_currentAction];
              float nextQ = mundo.getQVector()[_nextState][_nextAction];
              //sarsa RULE
              actualQ += (_alpha *(_reward + _gamma * (nextQ - actualQ) ) );
              //QLearning RULE
              //actualQ += _reward + _gamma * nextQ;
              // printf(" %f += %f  * %f (%f-%f)\n",actualQ,_alpha,_gamma,nextQ,actualQ );
              mundo.setQ(_currentState,_currentAction,actualQ);

              swarm[agent].setCurrentState(_nextState);
              // swarm[agent]=actualAgent;
              //printf("NEW STATE %d, GOAL STATE %d, result %d \n",swarm[agent].getCurrentState(),swarm[agent].getGoalState(),swarm[agent].getCurrentState() == swarm[agent].getGoalState());

              if (swarm[agent].getCurrentState() == swarm[agent].getGoalState()) {
                  // printf("\n");
                  // printf(" ARRIVED TO GOAL STATE\n");
                  flagAgents[agent]=1;

              }
            }
            else if (accumulate(flagAgents.begin(), flagAgents.end(), 0) == swarm.size()) {
                step=100;
            }
          }
        }
        mundo.resetWorld();
        // mundo.setCellState(9,GOAL);
        // mundo.setCellState(19,GOAL);
        // mundo.setCellState(29,GOAL);
        swarm[0].setCurrentState(0);
        swarm[1].setCurrentState(10);
        swarm[2].setCurrentState(20);
        //reset flags
        for (unsigned short flag = 0; flag < flagAgents.size(); flag++) {
          flagAgents[flag]=0;
        }
      }

  mundo.printQ();

  // printf("%lu, State %d\n",flagAgents.size(),swarm[0].getCurrentState());
// DO TEST
cout << "TESTING Q"<< endl;
cout << endl << "_______________________________________________________________________________";
cout << endl << "____________________\t s \t" << "\t a \t" << "\t s' \t" << "\t r \t";
cout << endl << "________________________________________________________________________________________";
printf("\n");
  for (unsigned short step = 0; step < MAX_NUM_STEP; step++) {
    for (unsigned short agent = 0; agent < swarm_size; agent++) {
        //iterate through number of steps possible.
        if(!flagAgents[agent]){
          // printf("Agent %d) GOAL=%d",agent,swarm[agent].getGoalState() );

          unsigned short _currentState = swarm[agent].getCurrentState();
          CellSpaces _currentStateObject =mundo.getCellSpace(_currentState);
          //unsigned short _currentAction = swarm[agent].epsGreedyAction(mundo.getQVector()[_currentState]);
          unsigned short _currentAction = swarm[agent].maxAction(mundo.getQVector()[_currentState]);
          mundo.setCellState(_currentState,VISITED);
          unsigned short _nextState = swarm[agent].move(mundo.getWorld(),_currentAction,_WID,_LEN);
          //change state of actual space and next space
          CellSpaces _nextStateObject =mundo.getCellSpace(_nextState);
          float _reward = swarm[agent].calculateReward(_nextStateObject);
          if( _nextStateObject.getState() != OCCUPIED)
            mundo.setCellState(_nextState,OCCUPIED);
          else{
              _nextState = _currentState;
          }
          //mundo.setCellState(_nextState,OCCUPIED);
          cout<< "Agent "<<agent << "),  step " << step+1 << ")\t" << _currentState << "\t | \t " << _currentAction << "\t | \t " << _nextState <<  "\t | \t "<< _reward<<endl;

          swarm[agent].setCurrentState(_nextState);
        //  printf("NEW STATE %d, GOAL STATE %d, result %d \n",swarm[agent].getCurrentState(),swarm[agent].getGoalState(),swarm[agent].getCurrentState() == swarm[agent].getGoalState());

          if (swarm[agent].getCurrentState() == swarm[agent].getGoalState()) {
              printf("AGENT %d HAS ARRIVED TO GOAL STATE %d\n",agent,swarm[agent].getGoalState());
              flagAgents[agent]=1;
          }
        }
        else if (accumulate(flagAgents.begin(), flagAgents.end(), 0) == swarm.size()) {
            step=100;
        }
      }
  }

  return 0;
}

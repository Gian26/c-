#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <numeric>
#include <random>
#include "multiAgents.h"



/*
  STATE{
           previousAgentPosition_n (x,y)
           AgentAction_n,AgentAction_n+1,....
           newAgentPosition_n (x,y),newAgentPosition_n+1
           Spaces left to cover {n}
           Reward = ({-1,-25,-75}_n+{-1,-25,-75}_(n+1) + {-1,-25,-75}_(n+2)) + leftOver
       }
  Reward = ({-1,-25,-75}_n+{-1,-25,-75}_(n+1) + {-1,-25,-75}_(n+2)) + leftOver


  move to next state.
  check if there is a state with the same previous positions, obtain with best reward
        else if there is no state with same previous position randomize next step and create a new state.
  if (these positions are in the tree obtain the one with best reward.)

  1  2  3  4  5  6
  7  8  9  10 11 12
  13 14 15 16 17 18



  0  1  2     \  |  /
  3  4  5    <-  0  ->
  6  7  8     /  |  \

  actualQ += (_alpha *(_reward + _gamma * (nextQ - actualQ) ) );

  movimiento echo por agentes
  posición en mundo de cada agente

*/

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


//define cell function
CellSpaces::CellSpaces(const unsigned short _id,const unsigned short _state) :
 id(_id),state(_state){

}
unsigned short CellSpaces::getNumberState(){
  return id;
}
void CellSpaces::setState(const unsigned short _state){
  state = _state;
}

unsigned short CellSpaces::getState(){
  return state;
}

// define gridworld function
GridWorld::GridWorld(const unsigned short _len,const unsigned short _wid):
len(_len),wid(_wid){

  for (unsigned short box = 0; box < len*wid; box++) {
    grid.push_back(CellSpaces(box,0));
  }

  vector<float> tmp(_numActions, 0.0);

  for(unsigned short i=0; i<_numStates; i++)
  {  q_global.push_back(tmp);}

  srand ( unsigned (time(0) ) );
}

vector<CellSpaces> GridWorld::getWorld(){
  return grid;
}

unsigned short GridWorld::getLen(){
  return len;
}

unsigned short GridWorld::getWid(){
  return wid;
}

void GridWorld::printQ() const
{
	for(unsigned short i=0; i<_numStates; i++)
	{
		cout << endl << i+1 << " : \t";
		for(unsigned short j=0; j<_numActions; j++)
		{
			cout << q_global[i][j] << " ";
		}
	}
	cout << endl;
}

vector<std::vector<float> > GridWorld::getQVector(){
  return q_global;
}

CellSpaces GridWorld::getCellSpace(unsigned short _cellId){
  return grid[_cellId];
}

// float GridWorld::randomNumbers(){
//   float random = (float) rand()/RAND_MAX;
//   return random;
// }
double GridWorld::randomNumbers() {
   std::random_device rd; // obtain a random number from hardware
   std::mt19937 eng(rd()); // seed the generator
   std::uniform_real_distribution<> distr(0.0, 1.0); // define the range

   return  distr(eng); // generate numbers
}

void GridWorld::setQ(unsigned short _state, unsigned short _action,float _valor){
  //printf("State(%d), Action(%d), _valor(%f)\n",_state,_action,_valor );
  q_global[_state][_action]=_valor;
}

void GridWorld::setCellState(unsigned short _space,unsigned short _state){
  grid[_space].setState(_state);
}

void GridWorld::resetWorld(){
  for (unsigned short cell = 0; cell < grid.size(); cell++) {
    if (grid[cell].getState()!=3) {
       grid[cell].setState(0);
    }
  }
}

Agent::Agent(const unsigned short _numActions, const float _alpha,const float _gamma,const float _epsilon,unsigned short _currentState, const unsigned short _goalState):numActions(_numActions),alpha(_alpha),gamma(_gamma),epsilon(_epsilon),currentState(_currentState),goalState(_goalState){
  srand(time(0));
}

unsigned short Agent::move(vector<CellSpaces> _grid, unsigned short _movement,unsigned short _wid,unsigned short _len)
{
    unsigned short row = currentState / _wid;
    unsigned short column = currentState % _wid;
    // printf("\n");
    //  printf("ROW = %d COL =%d CURRENT = %d \n", row,column, currentState);
  	switch(_movement)
  	{
      case(0):// Diagonal izquierda Atras
        if(row > 0  && column>0){
          row--;
          column--;
        }
        break;
  		case(1): ///derecha
  			if(row > 0)
  				row--;
  			break;
      case(2):// Diagonal izquierda Adelante
        if(row > 0  && column<(_wid-1)){
          row--;
          column++;
        }
        break;
      case(3): /// atras
    		if(column > 0)
    				column--;
    		break;
      case(4)://no mover

        break;
  		case(5): /// adelante
  			if(column < (_wid-1))
          // {   printf(" row = %hu  (_%d-1)=%d\n",row,_len,(_wid-1));
  				    column++;
          // }
  			break;
      case(6)://diagonal atras derecha
      if(row < (_len-1) && column>0){
        row++;
        column--;
      }
      break;
  		case(7): /// izquierda
  			if(row < (_len-1))
  				row++;
  			break;
      case(8):// Diagonal Adelante
        if(row < (_len-1)  && column<(_wid-1)){
          row++;
          column++;
        }
      break;
  	}
  //  printf("ROW = %d COL =%d CURRENT = %d \n", row,column, (row * _wid) + column);

  return (row * _wid) + column;

}


double Agent::randomNumbers() {
   std::random_device rd; // obtain a random number from hardware
   std::mt19937 eng(rd()); // seed the generator
   std::uniform_real_distribution<> distr(0.0, 1.0); // define the range

   return  distr(eng); // generate numbers
}

unsigned short Agent::maxAction(vector< float >  q){

	unsigned short maxA = 0;
	float maxValue = q[0];

	for(unsigned short j=0; j<numActions; j++){
		if( q[j] > maxValue)
		{
			maxValue = q[j];
			maxA = j;
		}
  }
	return maxA;
}

unsigned short Agent::epsGreedyAction(vector<float> _q){
	unsigned short actionSelected = 0;
  //float random  = (float)rand()/RAND_MAX;
  double random = randomNumbers();
	if( random > epsilon ){
		//explode
  //  printf("Explode, r=%f, epsilonn=%f  \n", random,_epsilon);
  	actionSelected = maxAction(_q);

  }
	else{
		/// explore
    //printf("Explore\n");
		actionSelected = rand() % numActions;
  }

  return actionSelected;
}

void Agent::setCurrentState(unsigned short _currentState){
  currentState = _currentState;
}

void Agent::setGoalState(unsigned short _goalState){
  goalState = _goalState;
}


unsigned short  Agent::getCurrentState(){
  return currentState;
}

unsigned short  Agent::getGoalState(){
  return goalState;
}

float Agent::calculateReward(CellSpaces _currentState){
  float reward=0.0;
  unsigned short _state=_currentState.getState();
  if (_state==0){ // empty
    reward = -1.0;
  }
  else if (_state==1){ //Visited
    reward = -25;
  }
  else if (_state == 2) { //occupied
    reward = -75;
  } else if (_currentState.getNumberState() == this->goalState) { // goal state
    printf("CURRENT %d GOAL %d \n", _currentState.getNumberState(),this->goalState);
    reward = 0;
  }
  return reward;
}



void home(){}
void trainQ(){}
void testWorld(){}


int main(int argc, char const *argv[]) {

  const unsigned short EMPTY = 0,OCCUPIED = 1, VISITED = 2;
  unsigned short CURRENT_STATE_AGENTA  =0,CURRENT_STATE_AGENTB = 10,CURRENT_STATE_AGENTC = 20;
  printf("Create world with dimensions %d x %d\n",_WID,_LEN);
  GridWorld mundo(_WID,_LEN);

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
              //actualQ += reward + _gamma * nextQ;
              // printf(" %f += %f  * %f (%f-%f)\n",actualQ,_alpha,_gamma,nextQ,actualQ );
              mundo.setQ(_currentState,_currentAction,actualQ);//SAVE VALUE TO WORLD ACTION

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

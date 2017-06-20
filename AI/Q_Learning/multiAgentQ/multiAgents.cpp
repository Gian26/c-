#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <numeric>
#include "multiAgents.h"

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
GridWorld::GridWorld(const unsigned short _len,const unsigned short _wid, const unsigned short _numActions ):
len(_len),wid(_wid),numStates(_len*_wid),numActions(_numActions){

  for (unsigned short box = 0; box < numStates; box++) {
    grid.push_back(CellSpaces(box,0));
  }

  vector<float> tmp(numActions, 0.0);

  for(unsigned short i=0; i<numStates; i++)
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
	for(unsigned short i=0; i<numStates; i++)
	{
		cout << endl << i+1 << " : \t";
		for(unsigned short j=0; j<numActions; j++)
		{
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
    //printf("CURRENT %d GOAL %d \n", _currentState.getNumberState(),this->goalState);
    reward = 0;
  }
  return reward;
}



void home(){}
void trainQ(){}
void testWorld(){}

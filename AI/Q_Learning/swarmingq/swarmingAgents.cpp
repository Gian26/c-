  #include <iostream>
#include <vector>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <random>
#include <cmath>        // std::abs
#include <numeric>
#include <map>
#include <string>
#include <utility>
#include <limits>
#include "swarmingAgents.h"

/***********************************/
/***********CELL SPACE**************/
/***********************************/
CellSpace::CellSpace(const unsigned short _id,const unsigned short _situation) :
 id(_id),situation(_situation){

}
unsigned short CellSpace::getNumberSpace(){
  return id;
}
void CellSpace::setSpaceSituation(const unsigned short _situation){
  situation = _situation;
}

unsigned short CellSpace::getSpaceSituation(){
  return situation;
}

/***********************************/
/************GRID WORLD ************/
/***********************************/
GridWorld::GridWorld(const unsigned short _len,const unsigned short _wid, const unsigned short _numActions ):
len(_len),wid(_wid),numStates(_len*_wid),numActions(_numActions){

  for (unsigned short box = 0; box < numStates; box++) {
    grid.push_back(CellSpace(box,EMPTY));
  }

  vector<float> tmp(numActions, 0.0);

  for(unsigned short i=0; i<numStates; i++)
  {  q_global.push_back(tmp);}

  srand ( unsigned (time(0) ) );

  printf("Create world with dimensions %d x %d\n",_wid,_len);

}

vector<CellSpace> GridWorld::getWorld(){
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
		cout << endl << i << " : \t";
		for(unsigned short j=0; j<numActions; j++)
		{
			cout << q_global[i][j] << " ";
		}
	}
	cout << endl;
  // std::cout << "END" << '\n';

}

vector<std::vector<float> > GridWorld::getQVector(){
  return q_global;
}

CellSpace GridWorld::getCellSpace(unsigned short _cellId){
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

void GridWorld::setQ(Agent _agent, unsigned short _nextSpace, unsigned short _currentSpace,
                     unsigned short _currentAction, short _reward){
                       //printf("Calculate REWARD \n");
  unsigned short _nextAction = _agent.maxAction(q_global[_nextSpace]);
  //Calculate Q
  float actualQ = q_global[_currentSpace][_currentAction];
  float nextQ = q_global[_nextSpace][_nextAction];
  //printf(" += %f *(%f  + %f *(%f-%f)\n",_agent.getAlpha(),_reward,_agent.getGamma(),nextQ,actualQ );
  //Q RULE
  actualQ += (_agent.getAlpha() *(_reward + _agent.getGamma() * (nextQ - actualQ) ) );
  //printf("ASpace: (%d), Action: (%d), Reward: (%hd), Q:(%f)\n",_nextSpace,_currentAction,_reward,actualQ);

  q_global[_currentSpace][_currentAction]=actualQ;
}

vector<int> GridWorld::getVisitedVector(){
    vector<int> vistedVector(numStates,0);

    for (unsigned short cell = 0; cell < numStates; cell++) {
      if (grid[cell].getSpaceSituation() !=0){
        vistedVector[cell] = 1;
      }
    }
  return vistedVector;
}

void GridWorld::setCellSituation(unsigned short _space,unsigned short _state){
  grid[_space].setSpaceSituation(_state);
}

void GridWorld::resetWorld(){
  for (unsigned short cell = 0; cell < grid.size(); cell++) {
    grid[cell].setSpaceSituation(EMPTY);
    //
    // if (grid[cell].getSpaceSituation()!=GOAL) {
    //    grid[cell].setSpaceSituation(EMPTY);
    // }
  }
}

/**********************************/
/***************AGENT**************/
/**********************************/
Agent::Agent(const unsigned short _numActions, const float _alpha,const float _gamma,const float _epsilon,unsigned short _currentSpace, const unsigned short _goalSpace):numActions(_numActions),alpha(_alpha),gamma(_gamma),epsilon(_epsilon),currentSpace(_currentSpace),goalSpace(_goalSpace){
  srand(time(0));
  prevSpace = 0;
}

void Agent::setActionTaken(unsigned short _actionTaken){
  actionTaken = _actionTaken;
}

unsigned short Agent::getActionTaken(){
  return actionTaken;
}

const float Agent::getAlpha(){
  return alpha;
}

const float Agent::getGamma(){
  return gamma;
}

// unsigned short Agent::move(vector<CellSpace> _grid, unsigned short _movement,unsigned short _wid,unsigned short _len)
// {
//     unsigned short row = currentSpace / _wid;
//     unsigned short column = currentSpace % _wid;
//     // printf("\n");
//     //  printf("ROW = %d COL =%d CURRENT = %d \n", row,column, currentSpace);
//   	switch(_movement)
//   	{
//       case(0):// Diagonal izquierda Atras
//         if(row > 0  && column>0){
//           row--;
//           column--;
//         }
//         break;
//   		case(1): ///derecha
//   			if(row > 0)
//   				row--;
//   			break;
//       case(2):// Diagonal izquierda Adelante
//         if(row > 0  && column<(_wid-1)){
//           row--;
//           column++;
//         }
//         break;
//       case(3): /// atras
//     		if(column > 0)
//     				column--;
//     		break;
//       case(4)://no mover
//
//         break;
//   		case(5): /// adelante
//   			if(column < (_wid-1))
//           // {   printf(" row = %hu  (_%d-1)=%d\n",row,_len,(_wid-1));
//   				    column++;
//           // }
//   			break;
//       case(6)://diagonal atras derecha
//       if(row < (_len-1) && column>0){
//         row++;
//         column--;
//       }
//       break;
//   		case(7): /// izquierda
//   			if(row < (_len-1))
//   				row++;
//   			break;
//       case(8):// Diagonal Adelante
//         if(row < (_len-1)  && column<(_wid-1)){
//           row++;
//           column++;
//         }
//       break;
//   	}
//   //  printf("ROW = %d COL =%d CURRENT = %d \n", row,column, (row * _wid) + column);
//   return (row * _wid) + column;
// }

unsigned short Agent::move(vector<CellSpace> _grid, unsigned short _movement,unsigned short _wid,unsigned short _len)
{
    unsigned short row = currentSpace / _wid;
    unsigned short column = currentSpace % _wid;
    // printf("\n");
    //  printf("ROW = %d COL =%d CURRENT = %d \n", row,column, currentSpace);
  	switch(_movement)
  	{
  		case(0): ///derecha
  			if(row > 0)
  				row--;
  			break;
      case(1): /// atras
    		if(column > 0)
    				column--;
    		break;
      case(2)://no mover

        break;
  		case(3): /// adelante
  			if(column < (_wid-1))
          // {   printf(" row = %hu  (_%d-1)=%d\n",row,_len,(_wid-1));
  				    column++;
          // }
  			break;
  		case(4): /// izquierda
  			if(row < (_len-1))
  				row++;
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
    //printf("Explode, r=%f, epsilonn=%f  \n", random,epsilon);
  	actionSelected = maxAction(_q);

  }
	else{
		/// explore
    //printf("Explore\n");
		actionSelected = rand() % numActions;
  }

  return actionSelected;
}

void Agent::setCurrentSpaceIndex(unsigned short _currentSpace){
  currentSpace = _currentSpace;
}
unsigned short  Agent::getCurrentSpaceIndex(){
  return currentSpace;
}

void Agent::setPrevSpaceIndex(unsigned short _prevSpace){
  prevSpace = _prevSpace;
}
unsigned short Agent::getPrevSpaceIndex(){
  return  prevSpace;
}

void Agent::setGoalSpace(unsigned short _goalSpace){
  goalSpace = _goalSpace;
}
unsigned short  Agent::getGoalSpace(){
  return goalSpace;
}

float Agent::calculateReward(CellSpace _currentSpace){
  float reward=0.0;
  unsigned short _situation=_currentSpace.getSpaceSituation();
  if (_situation==EMPTY){ // empty
    reward = 0;
  }
  else if (_situation==VISITED){ //Visited
    reward = -25;
  }
  else if (_situation == OCCUPIED) { //occupied
    reward = -75;
  } else if (_currentSpace.getNumberSpace() == this->goalSpace) { // goal state
    //printf("CURRENT %d GOAL %d \n", _currentSpace.getNumberState(),this->goalState);
    reward = 0;
  }
  return reward;
}

void Agent::setReward(float _reward){
  reward = _reward;
}

float Agent::getReward(){
  return reward;
}

/***********************************/
/************Agent Data*************/
/***********************************/
// AgentData::AgentData(unsigned short _agentIndex, short _prevSpaceIndex,
//           unsigned short _actualSpaceIndex, unsigned short _spacesToCover,
//           unsigned short _actionMade, short _rewardObtained):agentIndex(_agentIndex),
//           prevSpaceIndex(_prevSpaceIndex),actualSpaceIndex(_actualSpaceIndex),
//           spacesToCover(_spacesToCover),actionMade(_actionMade),rewardObtained(_rewardObtained)
//           {
//
// }

AgentData::AgentData(unsigned short _agentIndex, short _prevSpaceIndex,
          unsigned short _actualSpaceIndex,
          unsigned short _actionTaken, short _learningValue):agentIndex(_agentIndex),
          prevSpaceIndex(_prevSpaceIndex),actualSpaceIndex(_actualSpaceIndex),
          actionTaken(_actionTaken),learningValue(_learningValue)
          {


}
unsigned short AgentData::getAgentIndex(){
    return agentIndex;
}

unsigned short AgentData::getPrevSpace(){
    return prevSpaceIndex;
}

unsigned short AgentData::getActualSpace(){
    return actualSpaceIndex;
}

unsigned short AgentData::getActionTaken(){
    return actionTaken;
}

short AgentData::getLearningValue(){
  return learningValue;
}

/***********************************/
/**************State****************/
/***********************************/
void State::addActionsQ(ActionsQ _actions){
    actionsQVector.push_back(_actions);
}

// void State::setStateValue(float _qValue){
//     qValue = _qValue;
// }
// void State::setSpacesToCover(short sTC){
//   spacesToCover = sTC;
// }
void State::setVisitedVector(vector<int> _spacesVisited){
  spacesVisited = _spacesVisited;
}

// short State::getSpacesToCover(){
//   return spacesToCover;
// }
// float State::getStateValue(){
//     return qValue;
// }
vector<ActionsQ> State::getActionsQ(){
  return actionsQVector;
}

vector<int> State::getVisitedVector(){
  return spacesVisited;
}

unsigned long long int convertBinaryToDecimal(unsigned long long int n)
{
    int decimalNumber = 0, i = 0, remainder;
    while (n!=0)
    {
        remainder = n%10;
        n /= 10;
        decimalNumber += remainder*pow(2,i);
        ++i;
    }
    return decimalNumber;
}

/***********************************/
/***************ActionsQ***************/
/***********************************/

float ActionsQ::getQValue()const{
  return qValue;
}

vector<int> ActionsQ::getAcciones(){
  return acciones;
}

/***********************************/
/***************Swarm***************/
/***********************************/

Swarm::stateMap Swarm::sMap=[]{
    stateMap map;
    return map;
}();

// short blocked = 2;

template <class T>
bool Swarm::compareVectors(vector<T> a, vector<T> b)
{
    // std::sort(a.begin(), a.end());
    // std::sort(b.begin(), b.end());
    return (a == b);
}

Swarm::Swarm(unsigned short _sizeOfSwarm, unsigned short _spacesToCover,
            const float _alpha,
            const float _gamma, const float _epsilon,
            unsigned short _numActions,unsigned short _world_len,unsigned short _world_wid):swarmSize(_sizeOfSwarm),spacesToCover(_spacesToCover),
            alpha(_alpha),gamma(_gamma),epsilon(_epsilon),world_len(_world_len),world_wid(_world_wid){

            spacesToCover -=_sizeOfSwarm;//+BLOCKED;
            prevspacesToCover = 0;
     for (unsigned short i = 0; i < _sizeOfSwarm; i++) {
       //agent
       Agent agent(_numActions,_alpha,_gamma,_epsilon,0,0);
       swarm.push_back(agent);
     }
}

void Swarm::printStates(){
// output the size of the map
	// cout << "Size of Map: " <<sMap.size() << '\n';
  // stateMap::iterator iter;
	// for (iter = sMap.begin(); iter != sMap.end(); ++iter) {
	// 	cout <<"STATE: "<< iter->first <<" Reward: "
	// 	<< iter->second.getStateValue()<< "\n";
  //   for (short dataIn = 0; dataIn < iter->second.getAgentDataV().size(); dataIn++) {
  //     cout << "Agent: "<<  iter->second.getAgentDataV()[dataIn].getAgentIndex()<< " Actual Space "<<iter->second.getAgentDataV()[dataIn].getPrevSpace()<<" Action "<< iter->second.getAgentDataV()[dataIn].getActionTaken()<<" New Space "<< iter->second.getAgentDataV()[dataIn].getActualSpace()<< '\n';
  //   }
  //   std::cout << " " << '\n';
  // }
}

void Swarm::getPath(GridWorld _world){
  vector<int> vistedVector(world_len*world_wid,1);
  std::cout << "" << '\n';
  string visited = "";
  while (_world.getVisitedVector() != vistedVector){
    // for(unsigned short index = 0; index  < _world.getVisitedVector().size(); index++) {
    //   std::cout << _world.getVisitedVector()[index] << ' ';
    //   visited += to_string(_world.getVisitedVector()[index]);
    // }

    std::cout << " " << '\n';
    string idString= ""; //Concatenation of spaces Index
    float reward=0;

    string v="";
    for (short index = 0; index < _world.getVisitedVector().size(); index++) {
      // std::cout << _world.getVisitedVector()[index] << ' ';
      v += to_string(_world.getVisitedVector()[index]);
    }

    std::cout << v << '\n';
    unsigned long long int a = convertBinaryToDecimal(stoull(v));
    // cout<< a <<" "<<endl;

    for (short agentIndex = 0; agentIndex < swarmSize; agentIndex++) {
      idString += to_string(swarm[agentIndex].getCurrentSpaceIndex()); //concatenate previous index to form hash ID
    }

    idString += to_string(a);

    std::cout << idString << '\n';

    // search
    stateMap::iterator iter = sMap.find(idString);

    // moveUseState(_world,iter->second);
  }
}

// void Swarm::cleanMap(){
//   stateMap.clear();
// }
//
// void Swarm::updateState(State_stateElement){
//
// }
//
// void Swarm::searchState(){}

double randomNumbers() {
  std::random_device rd; // obtain a random number from hardware
  std::mt19937 eng(rd()); // seed the generator
  std::uniform_real_distribution<> distr(0.0, 1.0); // define the range
  return  distr(eng); // generate numbers
}

unsigned short Swarm::ManhattanDistance(unsigned short _x1,unsigned short _x2,unsigned short _y1,unsigned short _y2){
  return abs(_x1 - _y1) + abs(_x2 - _y2);
}

void Swarm::CoordinateConversion(unsigned short _pos, unsigned short &_x1,unsigned short &_x2){
  _x1 = _pos / world_wid;
  _x2 = _pos % world_wid;
}

unsigned short Swarm::DistanceBetweenNeighbours(unsigned short _pos1,unsigned short _pos2){
  unsigned short pos1x,pos1y,pos2x,pos2y;
  CoordinateConversion(_pos1,pos1x,pos1y);
  CoordinateConversion(_pos2,pos2x,pos2y);
  return ManhattanDistance(pos1x,pos1y,pos2x,pos2y)-1;
}



void Swarm::chooseActions(vector<int> &actions){
  for (short i = 0; i < swarmSize; i++) {
    int actionSelected = rand() % 5;
    actions.push_back(actionSelected);
  }
}



//save state is the backing up of the configuration the agent is in
void Swarm::saveState(GridWorld _world){
  std::cout << "Saving STATE" << '\n';
  State state;
  string idStringNxt="",idString= ""; //Concatenation of spaces Index
  float reward=0;
  vector<int> accionesNow;

  //crear el id a guardar
  for (short agentIndex = 0; agentIndex < swarmSize; agentIndex++) {
    // AgentData data(agentIndex, swarm[agentIndex].getPrevSpaceIndex(),swarm[agentIndex].getCurrentSpaceIndex(),swarm[agentIndex].getActionTaken(), swarm[agentIndex].getReward());
    // cout<< "Prev S" << swarm[agentIndex].getPrevSpaceIndex()<<  " Action " <<swarm[agentIndex].getActionTaken() << " Current S: "<< swarm[agentIndex].getCurrentSpaceIndex()<< " Reward " <<swarm[agentIndex].getReward()<<endl;
    // state.addAgentData(data); //agentData Vector
    
    accionesNow.push_back(swarm[agentIndex].getActionTaken()); // acciones de los agentes
    reward += swarm[agentIndex].getReward(); //sum individual rewards
    idString += to_string(swarm[agentIndex].getPrevSpaceIndex()); //concatenate previous index to form hash ID
  }
  string v="";
  for (short index = 0; index < _world.getVisitedVector().size(); index++) {
    // std::cout << _world.getVisitedVector()[index] << ' ';
    v += to_string(_world.getVisitedVector()[index]);
  }

  unsigned long long int a = convertBinaryToDecimal(stoull(v));
  cout<< a <<" "<<endl;
  idString += to_string(a); //concatenate previous index to form hash ID

  //get next position of agents with newly chosen actions
  //this generates the id needed
  for (short agentIndex = 0; agentIndex < swarmSize; agentIndex++) {
    // _nextSpace = swarm[agentIndex].move(_world.getWorld(),nextActions[agentIndex],_world.getWid(),_world.getLen());
    idStringNxt += to_string(swarm[agentIndex].getCurrentSpaceIndex());
  }
  float actualQ = 0.0;
  float nextQ = 0.0;

  // ActionsQ newQobject;
  // //buscar en el map la q que sigue
  // stateMap::iterator iter = sMap.find(idStringNxt);
  // if( iter != sMap.end() ) // if it doesnt get to the end it exist
  // {
  //   // retrieve max vector action
  //   newQobject = std::max_element( iter->second.getActionsQ().begin(), iter->second.getActionsQ().end(),ActionsQ::ByQValue() );
  //   nextQ = newQobject.qValue; // prints max q
  // }
  //
  // ActionsQ actualQobject;
  // // buscar en el mapa la q anterior si es que existe la q;
  // stateMap::iterator iter = sMap.find(idString);
  // if( iter != sMap.end() ) // if it doesnt get to the end it exist
  // {
  //   // retrieve max vector action
  //   actualQobject = std::max_element( iter->second.getActionsQ().begin(), iter->second.getActionsQ().end(),ActionsQ::ByQValue() );
  //   actualQ = actualQobject.qValue; // prints max q
  // }

  //unsigned short _nextState = maxAction(q_global[_nextSpace]);
  //Calculate Q
  // float actualQ = q_global[_currentSpace][_currentAction];
  // float nextQ = q_global[_nextSpace][_nextAction];
  //Q RULE
  // actualQ += (_agent.getAlpha() *(_reward + _agent.getGamma() * (nextQ - actualQ) ) );
  // ActionsQ actionsq(acciones,qvalue);
  // actualQobject.qValue += (0.5 *(reward + 1* (nextQ - actualQ) ) );
  /****

  ****/

  // unsigned short distance = DistanceBetweenNeighbours(swarm[0].getCurrentSpaceIndex(),swarm[1].getCurrentSpaceIndex());
  // cout  << "Distance from Agent A to Agent B: "<< distance << endl;
  // reward = reward - (prevspacesToCover);//-(distance*10);
  // state.setStateValue(reward);
  // state.setSpacesToCover(spacesToCover);
  // state.setVisitedVector(_world.getVisitedVector());
  //
  // string v="";
  // for (short index = 0; index < _world.getVisitedVector().size(); index++) {
  //   // std::cout << _world.getVisitedVector()[index] << ' ';
  //   v += to_string(_world.getVisitedVector()[index]);
  // }
  // std::cout << v << '\n';
  // unsigned long long int a = convertBinaryToDecimal(stoull(v));
  // // cout<< a <<" "<<endl;
  // idString += to_string(a); //concatenate previous index to form hash ID
  //
  // // find idString, if exists verify reward, if better reward replace, else keep, if doesnt exist keep.
  // // 	// find will return an iterator to the matching element if it is found
  // // 	// or to the end of the map if the key is not found
  // // cout<< " RE "<< reward<< endl;
  // //reward -=spacesToCover;
	// //string country(idString);
  //
	// stateMap::iterator iter = sMap.find(idString);
	// if( iter != sMap.end() ) // if it doesnt get to the end it exist
	// {
  //   std::cout << "Now "<<iter->second.getStateValue()<< " Possible Reward "<<reward <<'\n';
  //   if (state.getVisitedVector()==iter->second.getVisitedVector()) {
  //     std::cout << "IF THE SAME" << '\n';
  //     std::cout << "check "<< reward << " > "<<iter->second.getStateValue()<<'\n';
  //     if(reward>iter->second.getStateValue())
  //     {
  //       //std::cout << "Change Entire State to better state" << '\n';
  //       iter->second =state;
  //       iter->second.setStateValue(reward);
  //     }
  //   }else{
  //     std::cout << "ELSE different" << '\n';
  //     std::cout << "check "<< reward << " > "<<iter->second.getStateValue()<<'\n';
  //     if(reward>iter->second.getStateValue())
  //     {
  //       //std::cout << "Change Entire State to better state" << '\n';
  //       iter->second =state;
  //       iter->second.setStateValue(reward);
  //     }
  //   }
  //
  // }
	// else
  // {
  //   sMap.insert(pair<string,State>(idString,state));
  //   cout << "created new key" << '\n';
  // }
  // std::cout << "ID State: " << idString << "  QValue "<<iter->second.getStateValue( )<< '\n';
  // std::cout << "SAVED" << '\n';
}

//
void Swarm::moveUseState(GridWorld &_world,State _state){
 cout<< "STATE USE "<<endl;

  // obtain max action for that state
  auto max_score = std::max_element( _state.getActionsQ().begin(), _state.getActionsQ().end(),ActionsQ::ByQValue() );

  for (short agentIndex = 0; agentIndex < swarmSize; agentIndex++) {
    Agent agent = swarm[agentIndex];
    //Agent takes an action
    unsigned short _currentSpace = agent.getCurrentSpaceIndex();
    swarm[agentIndex].setPrevSpaceIndex(_currentSpace);//Prev Space assignment
    CellSpace _currentSpaceObject =_world.getCellSpace(_currentSpace);//Obtaining object
    //unsigned short _currentAction = swarm[agentIndex].epsGreedyAction(_world.getQVector()[_currentSpace]);
    unsigned short _currentAction = max_score->getAcciones()[agentIndex];//_state.getAgentDataV()[agentIndex].getActionTaken();//obtener acción
    swarm[agentIndex].setActionTaken(_currentAction);//set action to agent
    //unsigned short _nextSpace =  _state.getAgentDataV()[agentIndex].getActualSpace(); //obtain next space
    unsigned short _nextSpace = swarm[agentIndex].move(_world.getWorld(),_currentAction,_world.getWid(),_world.getLen());
  //   //swarm[agentIndex].move(_world.getWorld(),_currentAction,_world.getWid(),_world.getLen());
    CellSpace _nextSpaceObject = _world.getCellSpace(_nextSpace);
    //is the space empty or has been visted?
    if( _nextSpaceObject.getSpaceSituation() == VISITED | _nextSpaceObject.getSpaceSituation() == EMPTY){
      if (spacesToCover > 0 && _nextSpaceObject.getSpaceSituation() == EMPTY) {
        spacesToCover -=1;  //subtract spaces to cover
      }
      _world.setCellSituation(_nextSpace,OCCUPIED);
      _world.setCellSituation(_currentSpace,VISITED);//
      swarm[agentIndex].setCurrentSpaceIndex(_nextSpace);
      //cout<< "CAN MOVE TO SPACE -> Changd OCCUPIED"<< endl;
    }
    else{
        _nextSpace = _currentSpace;
      //cout<< "CAN NOT MOVE TO SPACE"<< endl;
    }
    //Calculate Reward
  //   float _reward = swarm[agentIndex].calculateReward(_nextSpaceObject); // reward if able to make movement
  //   swarm[agentIndex].setReward(_reward); //save reward
  //   cout<< "Agent: "<< agentIndex<<" Actual Pos: "<< _currentSpace<< " Action: "<< _currentAction<< " Next Pos "<< _nextSpace<< " Reward " << _reward << " Spaces to cover "<< spacesToCover<<endl;
  //
  //   //Save and calculate q
  //   _world.setQ(agent,_nextSpace,_currentSpace,_currentAction,_reward);
  }
}

void Swarm::move1by1(GridWorld &_world){
  std::cout << "move 1 by 1" << '\n';
  for (short agentIndex = 0; agentIndex < swarmSize; agentIndex++) {
    Agent agent = swarm[agentIndex];

    //obtain where the agent is in the world
    unsigned short _currentSpace = swarm[agentIndex].getCurrentSpaceIndex();
    swarm[agentIndex].setPrevSpaceIndex(_currentSpace);
    CellSpace _currentSpaceObject =_world.getCellSpace(_currentSpace);
    //choos an action
    unsigned short _currentAction = swarm[agentIndex].epsGreedyAction(_world.getQVector()[_currentSpace]);
    swarm[agentIndex].setActionTaken(_currentAction);
    //perform the action to obtain the next space
    unsigned short _nextSpace = swarm[agentIndex].move(_world.getWorld(),_currentAction,_world.getWid(),_world.getLen());
    CellSpace _nextSpaceObject = _world.getCellSpace(_nextSpace);
    cout<< "AGENT "<< agentIndex<< " Space NOW "<< _currentSpace << " Current Situation "<<_currentSpaceObject.getSpaceSituation()<<" Action "<< _currentAction<< " Space NOW "<< _nextSpace << " Current Situation "<<_nextSpaceObject.getSpaceSituation()<<endl;
    //is the space empty or has been visted?
    if( _nextSpaceObject.getSpaceSituation() == VISITED | _nextSpaceObject.getSpaceSituation() == EMPTY){
      // if(_nextSpaceObject.getSpaceSituation() == VISITED){
      //   cout << "VISTED"<<endl;
      // }
      // if(_nextSpaceObject.getSpaceSituation() == EMPTY){
      //
      // }

      if (spacesToCover > 0 && _nextSpaceObject.getSpaceSituation() == EMPTY) {
        spacesToCover -=1;  //subtract spaces to cover
         cout << "EMPTY"<<endl;
      }
      _world.setCellSituation(_nextSpace,OCCUPIED);
      _world.setCellSituation(_currentSpace,VISITED);//
      swarm[agentIndex].setCurrentSpaceIndex(_nextSpace);
      //cout<< "CAN MOVE TO SPACE -> Changd OCCUPIED"<< endl;
    }// end if

    if(_nextSpaceObject.getSpaceSituation() == OCCUPIED){
      // if(_nextSpaceObject.getSpaceSituation() == OCCUPIED){
      //   cout << "OCCUPIED"<<endl;
      // }
        _nextSpace = _currentSpace;
      //cout<< "CAN NOT MOVE TO SPACE"<< endl;
    }
    //Calculate Reward
    // float _reward = swarm[agentIndex].calculateReward(_nextSpaceObject); // reward if able to make movement
    // swarm[agentIndex].setReward(_reward); //save reward
    // cout<< "Agent: "<< agentIndex<<" Actual Pos: "<< _currentSpace<< " Action: "<< _currentAction<< " Next Pos "<< _nextSpace<< " Reward " << _reward << " Spaces to cover "<< spacesToCover<<endl;
    // //Save and calculate q
    // _world.setQ(agent,_nextSpace,_currentSpace,_currentAction,_reward);
    //  cout << "   "<< endl;
  }
}

void Swarm::moveSwarm(GridWorld &_world){
  prevspacesToCover=spacesToCover; //saves swarm now.
  std::cout << "move swarm..." << '\n';
  double random = randomNumbers();
  //find actual state in the prev space and verify if there is a good reward.
  //if there is none perform the following 1 by 1.
  string idString="";
  for (unsigned short agentIndex = 0; agentIndex < swarmSize; agentIndex++) {
    idString += to_string(swarm[agentIndex].getCurrentSpaceIndex()); //concatenate previous index to form hash ID
  }

  string v="";
  for (short index = 0; index < _world.getVisitedVector().size(); index++) {
    // std::cout << _world.getVisitedVector()[index] << ' ';
    v += to_string(_world.getVisitedVector()[index]);
  }

  std::cout << v << '\n';
  unsigned long long int a = convertBinaryToDecimal(stoull(v));
  // cout<< a <<" "<<endl;
  // idString +=to_string(spacesToCover);
  idString += to_string(a);
  // std::cout << " Random "<<random << " Epsilon "<< epsilon << '\n';
  if( random < epsilon && !sMap.empty() ){
    std::cout << "BUSCAR " << idString<<'\n';
    stateMap::iterator iter = sMap.find(idString);
  	if( iter != sMap.end() ) // if iter is different to the end it exist
  	{
      printf("Explode\n");
      moveUseState(_world,iter->second);
      return;
    }
    std::cout << "NO EXISTE " << idString<<'\n';
  }
  // explore
  printf("Explore\n");
  //Move swarm by state or 1 by 1
  move1by1(_world);
}

unsigned short Swarm::getSpacesToCover(){
  return spacesToCover;
}

void Swarm::setSpacesToCover(unsigned short _spacesToCover){
  spacesToCover = _spacesToCover;
}
unsigned short Swarm::getSwarmSize(){
    return swarmSize;
}

 vector<Agent>Swarm::getSwarm(){
  return swarm;
}
void Swarm::setCurrentAgentSpaceIndex(unsigned short AGENTINDEX, unsigned short CURRENT_SPACE_AGENT){
  swarm[AGENTINDEX].setCurrentSpaceIndex(CURRENT_SPACE_AGENT);
}

void Swarm::setPrevAgentSpaceIndex(unsigned short AGENTINDEX, unsigned short PREV_SPACE_AGENT){
  swarm[AGENTINDEX].setPrevSpaceIndex(PREV_SPACE_AGENT);
}

Agent Swarm::getSwarmAgent(unsigned short _agentIndex){
  return swarm[_agentIndex];
}

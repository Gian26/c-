#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <numeric>
#include <algorithm>
#include "swarmingAgents.h"
// basic file operations
#include <fstream>
#include <math.h>

/*
http://stackoverflow.com/questions/6848828/reinforcement-learning-differences-between-qlearning-and-sarsatd

terminar animacion
obstaculos
checar mundos de 3x3

Revisar una pregunta mas, por quien esta ocupado el estado al momento de preguntar si se mantiene.*

de que trata la plataforma
plantear casos de uso como las preguntas: .
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

  0  1  2  3  4  5  6  7  8  9
  10 11 12 13 14 15 16 17 18 19
  20 21 22 23 24 25 26 27 28 29

  0  1  2  3
  4  5  6  7
  8  9  10 11
  12 13 14 15

  0  1  2     \  |  /
  3  4  5    <-  0  ->
  6  7  8     /  |  \

  actualQ += (_alpha *(_reward + _gamma * (nextQ - actualQ) ) );

  movimiento echo por agentes
  posición en mundo de cada agente

*/

/// ALGORITMO Q-LEARNING
const int _SIZEOFSWARM = 1;
const int _MAX_EPISODES =  1000;
const int _MAX_NUM_STEP = 100;
const unsigned _WID = 5;
const unsigned _LEN = 5;
const unsigned short _SIZEOFWORLD = _WID * _LEN;
// const unsigned short _NUMACTIONS = 9;
const unsigned short _NUMACTIONS = 5;
const float _ALPHA = 0.5;
const float _GAMMA = 1.0;
const float _EPSILON = 0.1;

using namespace std;

int b,d=0,e=0,c=0;

int decimal_binario(){
  int numero, dividendo, resto, divisor = 2;
  string binario = "";
  cout << "Ingrese el numero decimal a convertir a sistema binario" << endl;
  cin >> numero;
  dividendo = numero;
  while(dividendo >= divisor){ //Mientras el dividendo sea mayor o igual que el divisor, es decir, mayor o igual que 2.
  resto = dividendo % 2;
   if(resto == 1)
    binario = "1" + binario; //Si el resto es igual a 1 concatenamos 1 a la variable string llamada binario
   else
    binario = "0" + binario; // Sino concatemanos 0
   //Es importante este orden de concatenación (primero el bit y luego el valor de la variable) esto para que nuestro número
   //en sistema binario esté ordenado correctamente.
    dividendo = dividendo/divisor; // Actualizamos el valor del dividendo dividiendolo entre 2.
  }
   if(dividendo == 1)
    binario = "1" + binario; // Por último sí el valor final del dividendo es 1 concatenamos 1 sino concatenamos 0.
   else
    binario = "0" + binario;
   cout << "En sistema binario " << numero<< " se escribe " << binario << endl;

   return stoi(binario);
}

// int convertBinaryToDecimal(long long n)
// {
//     int decimalNumber = 0, i = 0, remainder;
//     while (n!=0)
//     {
//         remainder = n%10;
//         n /= 10;
//         decimalNumber += remainder*pow(2,i);
//         ++i;
//     }
//     return decimalNumber;
// }

int main(int argc, char const *argv[]) {

  // int a = decimal_binario();
  // cout<< "Binario "<< convertBinaryToDecimal(a)<< endl;

  unsigned short CURRENT_SPACE_AGENTA  =0,
                 CURRENT_SPACE_AGENTB = 4,
                 CURRENT_SPACE_AGENTC = 20;

  //creating the world
  GridWorld world(_WID,_LEN,_NUMACTIONS);
  //Creating the swarm of agents
  Swarm swarm(_SIZEOFSWARM,_SIZEOFWORLD,_ALPHA,_GAMMA,_EPSILON,_NUMACTIONS,_WID,_LEN);

  //Agent a
  swarm.setCurrentAgentSpaceIndex(0,CURRENT_SPACE_AGENTA);
  world.setCellSituation(CURRENT_SPACE_AGENTA,OCCUPIED);
  //Agent b
  //  swarm.setCurrentAgentSpaceIndex(1,CURRENT_SPACE_AGENTB);
  //  world.setCellSituation(CURRENT_SPACE_AGENTB,OCCUPIED);

  //
  // world.setCellSituation(2,OCCUPIED);
  // world.setCellSituation(5,OCCUPIED);


  // swarm.setCurrentAgentSpaceIndex(2,CURRENT_SPACE_AGENTC);
  // world.setCellSituation(CURRENT_SPACE_AGENTC,OCCUPIED);

  cout << swarm.getSwarmSize()<<endl;
   // There will not be a goal space,
  // but instead the goal is to cover the entire map

  // create the state
  //make the movements for each agent
  // for n each agent
  vector<int> vistedVector(_SIZEOFWORLD,1);
  //
  for (short episode = 0; episode <_MAX_EPISODES; episode++) {
    cout<< "Episode "<< episode<<endl;
    std::cout <<  '\n';
    for (short step = 0; step < _MAX_NUM_STEP; step++) {
      std::cout << " " << '\n';
      std::cout << "Step: "<< step << '\n';
      swarm.moveSwarm(world); //move all agents
      swarm.saveState(world);
      //cout << "Spaces to cover "<< swarm.getSpacesToCover() << endl;
      std::cout << "Spaces to Cover "<< swarm.getSpacesToCover() << '\n';
      // if (swarm.getSpacesToCover() == 0) {
      //   //cout<< "Enter"<< swarm.getSpacesToCover()<<endl;
      //   step = _MAX_NUM_STEP;
      // }
      // int sum_of_elems = 0;
      // for(std::vector<int>::iterator it = world.getVisitedVector().begin(); it != world.getVisitedVector().end(); ++it)
      //     sum_of_elems += *it;
      // // int sum=std::accumulate(world.getVisitedVector().begin(), world.getVisitedVector().end(),0) ;
      // std::cout << "SUM "<<sum_of_elems << '\n';
      if (world.getVisitedVector()== vistedVector) {
        step = _MAX_NUM_STEP;
      }
    }
    world.resetWorld();

    swarm.setCurrentAgentSpaceIndex(0,CURRENT_SPACE_AGENTA);
    swarm.setPrevAgentSpaceIndex(0,CURRENT_SPACE_AGENTA);
    world.setCellSituation(CURRENT_SPACE_AGENTA,OCCUPIED);

  //   // world.setCellSituation(2,OCCUPIED);
  //   // world.setCellSituation(5,OCCUPIED);
  //   // //
  //   // swarm.setCurrentAgentSpaceIndex(1,CURRENT_SPACE_AGENTB);
  //   // swarm.setPrevAgentSpaceIndex(1,CURRENT_SPACE_AGENTB);
  //   // world.setCellSituation(CURRENT_SPACE_AGENTB,OCCUPIED);

    swarm.setSpacesToCover(_SIZEOFWORLD-_SIZEOFSWARM);//-BLOCKED);

    std::cout << " " << '\n';
  }
  std::cout << " " << '\n';

  // after training

  swarm.printStates();
  for (size_t i = 0; i < 10; i++) {
    std::cout << "**********************************" << '\n';
  }
  swarm.getPath(world);

  //
  // world.printQ();
  //
  // ofstream myfile;
  // myfile.open ("example.txt",ios::in |ios::out|ios::app);
  // std::vector<State> path = swarm.getPath();
  // for (size_t index = 0; index < path.size(); index++) {
  //   string agentStr="";
  //   for (short dataIn = 0; dataIn < path[index].getAgentDataV().size(); dataIn++) {
  //     agentStr = to_string(path[index].getAgentDataV()[dataIn].getAgentIndex())+" "+to_string(path[index].getAgentDataV()[dataIn].getPrevSpace())+" "+to_string(path[index].getAgentDataV()[dataIn].getActionTaken())+" "+to_string(path[index].getAgentDataV()[dataIn].getActualSpace())+" "+to_string(path[index].getSpacesToCover());
  //     myfile << agentStr<<"\n";
  //   }
  //
  // }
  //   myfile.close();

 // swarm.cleanMap();
//   //TRAINING
//   cout<< "START TRAINING!"<< endl;
//   vector<unsigned short> flagAgents(swarm.size());
//   // // iterate through episodes
//   for (unsigned short episode = 0; episode < MAX_EPISODES; episode++) {
//     //     //iterate through swarm
//     // cout << endl << endl << " Episodio " << episode+1  << endl ;
//     // cout << endl << "____________________________________________________________________";
//     // cout << endl << "\t s \t" << "\t a \t" << "\t s' \t" << "\t r \t";
//     // cout << endl << "____________________________________________________________________";
//     // printf("\n");
//
//       for (unsigned short step = 0; step < MAX_NUM_STEP; step++) {
//         for (unsigned short agent = 0; agent < swarm_size; agent++) {
//             //iterate through number of steps possible.
//             if(!flagAgents[agent]){
//               unsigned short _currentState = swarm[agent].getCurrentState();
//               CellSpaces _currentStateObject =mundo.getCellSpace(_currentState);
//               unsigned short _currentAction = swarm[agent].epsGreedyAction(mundo.getQVector()[_currentState]);
//               mundo.setCellState(_currentState,VISITED);
//               unsigned short _nextState = swarm[agent].move(mundo.getWorld(),_currentAction,_WID,_LEN);
//               //change state of actual space and next space
//               //ask if the new state is occupied or empty
//
//               CellSpaces _nextStateObject =mundo.getCellSpace(_nextState);
//               float _reward = swarm[agent].calculateReward(_nextStateObject);
//
//               if( _nextStateObject.getState() != OCCUPIED)
//                 mundo.setCellState(_nextState,OCCUPIED);
//               else{
//                   _nextState = _currentState;
//               }
//
//               // cout<< "Agent "<<agent << "),  step " << step+1 << ")\t" << _currentState << "\t | \t " << _currentAction << "\t | \t " << _nextState <<  "\t | \t "<< _reward<<"\n"<<endl;
//
//               unsigned short _nextAction = swarm[agent].maxAction(mundo.getQVector()[_nextState]);
//               //Calculate Q
//               float actualQ = mundo.getQVector()[_currentState][_currentAction];
//               float nextQ = mundo.getQVector()[_nextState][_nextAction];
//               //sarsa RULE
//               actualQ += (_alpha *(_reward + _gamma * (nextQ - actualQ) ) );
//               //QLearning RULE
//               //actualQ += _reward + _gamma * nextQ;
//               // printf(" %f += %f  * %f (%f-%f)\n",actualQ,_alpha,_gamma,nextQ,actualQ );
//               mundo.setQ(_currentState,_currentAction,actualQ);
//
//               swarm[agent].setCurrentState(_nextState);
//               // swarm[agent]=actualAgent;
//               //printf("NEW STATE %d, GOAL STATE %d, result %d \n",swarm[agent].getCurrentState(),swarm[agent].getGoalState(),swarm[agent].getCurrentState() == swarm[agent].getGoalState());
//
//               if (swarm[agent].getCurrentState() == swarm[agent].getGoalState()) {
//                   // printf("\n");
//                   // printf(" ARRIVED TO GOAL STATE\n");
//                   flagAgents[agent]=1;
//
//               }
//             }
//             else if (accumulate(flagAgents.begin(), flagAgents.end(), 0) == swarm.size()) {
//                 step=100;
//             }
//           }
//         }
//         mundo.resetWorld();
//         // mundo.setCellState(9,GOAL);
//         // mundo.setCellState(19,GOAL);
//         // mundo.setCellState(29,GOAL);
//         swarm[0].setCurrentState(0);
//         swarm[1].setCurrentState(10);
//         swarm[2].setCurrentState(20);
//         //reset flags
//         for (unsigned short flag = 0; flag < flagAgents.size(); flag++) {
//           flagAgents[flag]=0;
//         }
//       }
//

//
//   // printf("%lu, State %d\n",flagAgents.size(),swarm[0].getCurrentState());
// // DO TEST
// cout << "TESTING Q"<< endl;
// cout << endl << "_______________________________________________________________________________";
// cout << endl << "____________________\t s \t" << "\t a \t" << "\t s' \t" << "\t r \t";
// cout << endl << "________________________________________________________________________________________";
// printf("\n");
//   for (unsigned short step = 0; step < MAX_NUM_STEP; step++) {
//     for (unsigned short agent = 0; agent < swarm_size; agent++) {
//         //iterate through number of steps possible.
//         if(!flagAgents[agent]){
//           // printf("Agent %d) GOAL=%d",agent,swarm[agent].getGoalState() );
//
//           unsigned short _currentState = swarm[agent].getCurrentState();
//           CellSpaces _currentStateObject =mundo.getCellSpace(_currentState);
//           //unsigned short _currentAction = swarm[agent].epsGreedyAction(mundo.getQVector()[_currentState]);
//           unsigned short _currentAction = swarm[agent].maxAction(mundo.getQVector()[_currentState]);
//           mundo.setCellState(_currentState,VISITED);
//           unsigned short _nextState = swarm[agent].move(mundo.getWorld(),_currentAction,_WID,_LEN);
//           //change state of actual space and next space
//           CellSpaces _nextStateObject =mundo.getCellSpace(_nextState);
//           float _reward = swarm[agent].calculateReward(_nextStateObject);
//           if( _nextStateObject.getState() != OCCUPIED)
//             mundo.setCellState(_nextState,OCCUPIED);
//           else{
//               _nextState = _currentState;
//           }
//           //mundo.setCellState(_nextState,OCCUPIED);
//           cout<< "Agent "<<agent << "),  step " << step+1 << ")\t" << _currentState << "\t | \t " << _currentAction << "\t | \t " << _nextState <<  "\t | \t "<< _reward<<endl;
//
//           swarm[agent].setCurrentState(_nextState);
//         //  printf("NEW STATE %d, GOAL STATE %d, result %d \n",swarm[agent].getCurrentState(),swarm[agent].getGoalState(),swarm[agent].getCurrentState() == swarm[agent].getGoalState());
//
//           if (swarm[agent].getCurrentState() == swarm[agent].getGoalState()) {
//               printf("AGENT %d HAS ARRIVED TO GOAL STATE %d\n",agent,swarm[agent].getGoalState());
//               flagAgents[agent]=1;
//           }
//         }
//         else if (accumulate(flagAgents.begin(), flagAgents.end(), 0) == swarm.size()) {
//             step=100;
//         }
//       }
//   }

  return 0;
}

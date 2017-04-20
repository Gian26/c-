#ifndef MULTIAGETNS_H
#define MULTIAGETNS_H

#include <vector>
using namespace std;

class CellSpaces{
  protected:
    const unsigned short id; //row*numcolumn + column
    unsigned short state; //empty,visited, occupied,goal
  public:
    CellSpaces(const unsigned short _id,const unsigned short _state );
    void setState(const unsigned short _state);
    unsigned short getState();
    unsigned short getNumberState();
};

class GridWorld{
  protected:
    const unsigned short len;
    const unsigned short wid;
    const unsigned short numStates;
    const unsigned short numActions;
    vector<CellSpaces> grid;
    vector<vector<float> >  q_global;


  public:
    GridWorld(const unsigned short _len,const unsigned short _wid,const unsigned short _numActions );
    double randomNumbers();
    vector<CellSpaces> getWorld();
    unsigned short getLen();
    unsigned short getWid();
    void printQ() const;
    void setQ(unsigned short _state, unsigned short _action, float _valor);
    vector<vector<float> >  getQVector();
    CellSpaces getCellSpace(unsigned short _cellId);
    void setCellState(unsigned short _space,unsigned short _state);
    void resetWorld();
};


class Agent{
  protected:
    const unsigned short numActions;
    const float alpha;
    const float gamma;
    const float epsilon;
    unsigned short goalState;
    unsigned short currentState;

  public:
    Agent(const unsigned short _numActions, const float _alpha,const float _gamma,const float _epsilon,unsigned short _currentState, const unsigned short _goalState);
	  void Qlearning(unsigned short _state, unsigned short _action, float reward, unsigned short _state_Prime, unsigned short _action_Prime);
 		unsigned short maxAction(vector<float> q);
    double randomNumbers();
    unsigned short move(vector<CellSpaces> grid, unsigned short _movement,unsigned short wid,unsigned short _len);
		unsigned short epsGreedyAction(vector<float> q);
    void setCurrentState(unsigned short _currentState);
    void setGoalState(unsigned short _goalState);
    float calculateReward(CellSpaces _currentState);
    unsigned short getCurrentState();
    unsigned short getGoalState();


    //MOVEMENTS FOR THE AGENT
    unsigned short moveLeft();
    unsigned short moveRight();
    unsigned short moveForward();
    unsigned short moveBackward();
    unsigned short DiagonalLeftForward();
    unsigned short DiagonalRightForward();
    unsigned short DiagonalRightBackward();
    unsigned short DiagonalLeftBackward();
};

#endif

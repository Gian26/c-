#ifndef MULTIAGETNS_H
#define MULTIAGETNS_H

#include <vector>
#include <map>
#include <string>
#include <utility>
using namespace std;

const unsigned short BLOCKED=2, EMPTY = 0,OCCUPIED = 1, VISITED = 2,GOAL=3;

class CellSpace{
  protected:
    const unsigned short id; //row*numcolumn + column
    unsigned short situation; //empty,visited, occupied,goal
  public:
    CellSpace(const unsigned short _id,const unsigned short _situation );
    void setSpaceSituation(const unsigned short _situation);
    unsigned short getSpaceSituation();
    unsigned short getNumberSpace();
};


class Agent{
  protected:
    const unsigned short numActions;
    const float alpha;
    const float gamma;
    const float epsilon;
    unsigned short goalSpace;
    unsigned short currentSpace;
    unsigned short prevSpace;
    unsigned short actionTaken;
    float reward;
  public:
    Agent(const unsigned short _numActions, const float _alpha,const float _gamma,const float _epsilon,unsigned short _currentSpace, const unsigned short _goalSpace);
	  void Qlearning(unsigned short _state, unsigned short _action, float reward, unsigned short _state_Prime, unsigned short _action_Prime);
 		unsigned short maxAction(vector<float> q);
    double randomNumbers();
    unsigned short move(vector<CellSpace> grid, unsigned short _movement,unsigned short wid,unsigned short _len);
		unsigned short epsGreedyAction(vector<float> q);
    void setCurrentSpaceIndex(unsigned short _currentSpace);
    void setPrevSpaceIndex(unsigned short _prevSpace);
    void setActionTaken(unsigned short _actionTaken);
    void setGoalSpace(unsigned short _goalSpace);
    void setReward(float reward);
    float calculateReward(CellSpace _currentState);
    float getReward();
    unsigned short getCurrentSpaceIndex();
    unsigned short getPrevSpaceIndex();
    unsigned short getActionTaken();
    unsigned short getGoalSpace();
    const float getAlpha();
    const float getGamma();


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

class GridWorld{
  protected:
    const unsigned short len;
    const unsigned short wid;
    const unsigned short numStates;
    const unsigned short numActions;
    vector<CellSpace> grid;
    vector<vector<float> >  q_global;

  public:
    GridWorld(const unsigned short _len,const unsigned short _wid,const unsigned short _numActions );
    double randomNumbers();
    vector<CellSpace> getWorld();
    unsigned short getLen();
    unsigned short getWid();
    void printQ() const;
    void setQ(Agent _agent, unsigned short _nextSpace, unsigned short _currentSpace,
                         unsigned short _currentAction, short _reward);
    vector<vector<float> >  getQVector();
    CellSpace getCellSpace(unsigned short _cellId);
    void setCellSituation(unsigned short _space,unsigned short _state);
    void resetWorld();
    vector<int> getVisitedVector();
};


class AgentData{
  protected:
    unsigned short agentIndex;
    unsigned short prevSpaceIndex;
    unsigned short actualSpaceIndex;
    unsigned short actionTaken;
    short learningValue;
  public:
    AgentData(unsigned short _agentIndex, short _prevSpaceIndex,unsigned short _actualSpaceIndex,  unsigned short actionTaken, short learningValue);
    // AgentData(unsigned short _agentIndex, short _prevSpaceIndex,unsigned short _actualSpaceIndex, unsigned shortspacesToCover, unsigned short actionMade, short rewardObtained);
    // void setPrevSpace(unsigned short _prevSpaceIndex);
    // void setActualSpace(unsigned short _actualSpaceIndex);
    // void setActionMade(unsigned short _actionMade);
    // void setRewardObtained(short _rewardObtained);
    unsigned short getAgentIndex();
    unsigned short getPrevSpace();
    unsigned short getActualSpace();
    unsigned short getActionTaken();
    short getLearningValue();
};

class ActionsQ{
  protected:
    vector<int> acciones;
    float qValue;
  public:
    struct ByQValue
    {
      bool operator ()( const ActionsQ &a, const ActionsQ &b ) const
      {
          return a.getQValue() < b.getQValue();
      }
    };
    ActionsQ();
    float getQValue()const;
    vector<int> getAcciones();

};

class State{
  protected:
    // vector<AgentData> agentDataVector;
    vector<ActionsQ>actionsQVector;
    // short spacesToCover;
    // float  qValue;
    vector<int> spacesVisited;
  public:
    void addActionsQ(ActionsQ _actions);
    vector<ActionsQ> getActionsQ();
    void setStateValue(float _reward);
    void setSpacesToCover(short );
    void setVisitedVector(std::vector<int> v);
    short getSpacesToCover();
    float getStateValue();
    vector<int> getVisitedVector();
};

class Swarm{
  using stateMap = map<string,State>;

  protected:
    vector<Agent> swarm;
    static stateMap sMap;
    unsigned short prevspacesToCover;
    unsigned short spacesToCover;
    unsigned short swarmSize;
    const float alpha;
    const float gamma;
    const float epsilon;
    unsigned short world_len;
    unsigned short world_wid;

  public:
    Swarm(unsigned short _sizeOfSwarm, unsigned short _spacesToCover, const float _alpha, const float _gamma, const float _epsilon,unsigned short _numActions,unsigned short _world_len,unsigned short _world_wid);
    void saveState(GridWorld);
    void updateState(State _stateElement);
    bool searchState(State _stateElement);
    void chooseActions(vector<int> &);
    void moveSwarm(GridWorld &_world);
    void move1by1(GridWorld &);
    void moveUseState(GridWorld &_world,State _state);
    void printStates();
    void getPath(GridWorld _world);
    unsigned short getSpacesToCover();
    void setSpacesToCover(unsigned short _spacesToCover);
    Agent getSwarmAgent(unsigned short agentIndex);
    unsigned short getSwarmSize();
    vector<Agent> getSwarm();
    void setCurrentAgentSpaceIndex(unsigned short AGENTINDEX, unsigned short CURRENT_SPACE_AGENT);
    void setPrevAgentSpaceIndex(unsigned short AGENTINDEX, unsigned short PREV_SPACE_AGENT);
    void CoordinateConversion(unsigned short _pos1, unsigned short &x1,unsigned short &x2);
    unsigned short DistanceBetweenNeighbours(unsigned short _pos1,unsigned short _pos2);
    unsigned short ManhattanDistance(unsigned short _x1,unsigned short _x2,unsigned short _y1,unsigned short _y2);
    template <class T>
    bool compareVectors(vector<T> a, vector<T> b);
};
#endif

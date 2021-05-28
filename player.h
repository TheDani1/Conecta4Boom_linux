#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);
      bool pathFinding(Environment actual_, Player jugador_, int 0,int PROFUNDIDAD_ALFABETA,Environment::ActionType accion, double alpha, double beta);
    private:
      int jugador_;
      Environment actual_;
};
#endif

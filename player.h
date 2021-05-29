#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);
      bool pathFinding(Environment actual, Player jugador, int profundidad,int PROFUNDIDAD_ALFABETA,Environment::ActionType accion, double alpha, double beta);
      int numJugador();
    private:
      int jugador_;
      Environment actual_;
};
#endif

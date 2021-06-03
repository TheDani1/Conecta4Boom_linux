#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <list>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf = 9999999999.0, menosinf = -9999999999.0;

// Constructor
Player::Player(int jug)
{
   jugador_ = jug;
}


// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment &env)
{
   actual_ = env;
}

double Puntuacion(int jugador, const Environment &estado)
{
   double suma = 0;

   for (int i = 0; i < 7; i++)
      for (int j = 0; j < 7; j++)
      {
         if (estado.See_Casilla(i, j) == jugador)
         {
            if (j < 3)
               suma += j;
            else
               suma += (6 - j);
         }
      }

   return suma;
}

// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador)
{
   int ganador = estado.RevisarTablero();

   if (ganador == jugador)
      return 99999999.0; // Gana el jugador que pide la valoracion
   else if (ganador != 0)
      return -99999999.0; // Pierde el jugador que pide la valoracion
   else if (estado.Get_Casillas_Libres() == 0)
      return 0; // Hay un empate global y se ha rellenado completamente el tablero
   else
      return Puntuacion(jugador, estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar
pair<int , int >sucesivosTablero(const Environment &tablero, int jugador){

   list<pair<int, int>> posiciones_sucesivas_v;
   list<pair<int, int>> posiciones_sucesivas_h;

   list<pair<int, int>>::iterator it;

   //ANALISIS
   for(int c = 0; c < 7; c++){
      for(int i = 0; i < 7; i++ ){


         if(tablero.See_Casilla(i,c) == jugador){

            posiciones_sucesivas_v.push_back(make_pair(i,c));

         }else{

            posiciones_sucesivas_v.clear();

         }
      
      }

   }

   for(int i = 0; i < 7; i++){
      for(int j = 0; j < 7; j++ ){

         if(tablero.See_Casilla(i,j) == jugador){

            posiciones_sucesivas_h.push_back(make_pair(i,j));

         }else{

            posiciones_sucesivas_h.clear();

         }
      
      }

   }

   for(it = posiciones_sucesivas_v.begin(); it != posiciones_sucesivas_v.end(); it++){

      cout << (*it).first << " " << (*it).second << endl;

   }

   for(it = posiciones_sucesivas_h.begin(); it != posiciones_sucesivas_h.end(); it++){

      cout << (*it).first << " "<<(*it).second << endl;

   }

}


// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador){

   int ganador = estado.RevisarTablero();

   if (ganador == jugador)
      return 99999999.0; // Gana el jugador que pide la valoracion
   else if (ganador != 0)
      return -99999999.0; // Pierde el jugador que pide la valoracion
   else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
	{
		sucesivosTablero(estado, jugador);
	}


}

// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j)
{
   j = 0;
   for (int i = 0; i < 8; i++)
   {
      if (aplicables[i])
      {
         opciones[j] = i;
         j++;
      }
   }
}

double poda_AlfaBeta(Environment actual, int jugador, int depth,int profundidad_max,Environment::ActionType& accion, double alpha, double beta){


   if (depth == profundidad_max or actual.JuegoTerminado()){

      return Valoracion(actual, jugador);

   }

   Environment vectorE[8];
   int n_act = actual.GenerateAllMoves(vectorE);

   double maxEval, minEval, eval;

   for(int i = 0; i < n_act && beta > alpha; i++){

      eval = poda_AlfaBeta(vectorE[i], jugador, depth+1, profundidad_max,accion, alpha, beta);

      if(depth == 0) cout << "Accion " << i << ": " << eval<<endl;

      if(actual.JugadorActivo() == jugador){

         if(alpha < eval){

            alpha = eval;
            accion = static_cast<Environment::ActionType>(vectorE[i].Last_Action(jugador));

         }

      }else{ beta = min(beta,eval); }

   }

   if(actual.JugadorActivo() == jugador){

      //MAX
      return alpha;

   }else{

      return beta;

   }

}

// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think()
{
   const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
   const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

   Environment::ActionType accion; // acci�n que se va a devolver
   bool aplicables[8];             // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                                   // aplicables[0]==true si PUT1 es aplicable
                                   // aplicables[1]==true si PUT2 es aplicable
                                   // aplicables[2]==true si PUT3 es aplicable
                                   // aplicables[3]==true si PUT4 es aplicable
                                   // aplicables[4]==true si PUT5 es aplicable
                                   // aplicables[5]==true si PUT6 es aplicable
                                   // aplicables[6]==true si PUT7 es aplicable
                                   // aplicables[7]==true si BOOM es aplicable

   double valor;       // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
   double alpha, beta; // Cotas de la poda AlfaBeta

   int n_act; //Acciones posibles en el estado actual

   n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
   int opciones[10];

   // Muestra por la consola las acciones aplicable para el jugador activo
   //actual_.PintaTablero();
   cout << " Acciones aplicables ";
   (jugador_ == 1) ? cout << "Verde: " : cout << "Azul: ";
   for (int t = 0; t < 8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr(static_cast<Environment::ActionType>(t));
   cout << endl;

   //--------------------- COMENTAR Desde aqui
   /*cout << "\n\t";
   int n_opciones = 0;
   JuegoAleatorio(aplicables, opciones, n_opciones);

   if (n_act == 0)
   {
      (jugador_ == 1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
   }
   else if (n_act == 1)
   {
      (jugador_ == 1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " Solo se puede realizar la accion "
           << actual_.ActionStr(static_cast<Environment::ActionType>(opciones[0])) << endl;
      accion = static_cast<Environment::ActionType>(opciones[0]);
   }
   else
   { // Hay que elegir entre varias posibles acciones
      int aleatorio = rand() % n_opciones;
      cout << " -> " << actual_.ActionStr(static_cast<Environment::ActionType>(opciones[aleatorio])) << endl;
      accion = static_cast<Environment::ActionType>(opciones[aleatorio]);
   }

   */
   //--------------------- COMENTAR Hasta aqui

   //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------

   // Opcion: Poda AlfaBeta
   // NOTA: La parametrizacion es solo orientativa
   valor = poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, menosinf, masinf);
   cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;



   return accion;
}

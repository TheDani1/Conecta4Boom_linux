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
double diagonalConnect(const Environment &estado, int jugador){

   double resultado_test = 0.0;

   //TENEMOS 2 CRITERIOS DE EVALUACION HORIZONTAL

   //NUMERO DE FICHAS
   int nfichas_m = 0;
   int nfichas_o = 0;

   //NUMERO DE FICHAS SEGUIDAS (MINIMO 1)
   int cons_m = 1;
   int cons_o = 1;

   bool consecucion_o = false;
   bool consecucion_propia = false;

   //NUMERO DE FICHASv2
   int nfichas_m2 = 0;
   int nfichas_o2 = 0;

   //NUMERO DE FICHAS SEGUIDAS (MINIMO 1)v2
   int cons_m2 = 1;
   int cons_o2 = 1;

   bool consecucion_o2 = false;
   bool consecucion_propia2 = false;

   nfichas_o = 0;
    nfichas_m = 0;

    nfichas_o2 = 0;
    nfichas_m2 = 0;

    // recorremos las diagonales principales

    for (int i = 0; i < 7; i++){

       // diagonal principal de izquierda a derecha
        if (estado.See_Casilla(i, i) == jugador || estado.See_Casilla(i, i) == jugador+3){
            consecucion_o = false;
            cons_o = 1;

            if (consecucion_propia){
                cons_m++;
                nfichas_m += 4 * cons_m;
            } else {
                nfichas_m += 2;
            }

            consecucion_propia = true;

        } else if (estado.See_Casilla(i, i) != 0){
            consecucion_propia = false;
            cons_m = 1;

            if (consecucion_o){
                cons_o++;
                nfichas_o += 4 * cons_o;
            } else{
                nfichas_o += 2;
            }

            consecucion_o = true;

        } else {
            cons_o = 1;
            cons_m = 1;
            consecucion_propia = false;
            consecucion_o = false;
        }



        // diagonal principal de derecha a izquierda

        if (estado.See_Casilla(6-i, 6-i) == jugador || estado.See_Casilla(6-i, 6-i) == jugador+3){
            consecucion_o2 = false;
            nfichas_o2 = 1;

            if (consecucion_propia2){
                cons_m2++;
                nfichas_m2 += 4 * cons_m2;
            } else {
                nfichas_m2 += 2;
            }

            consecucion_propia2 = true;

        } else if (estado.See_Casilla(6-i, 6-i) != 0){
            consecucion_propia2 = false;
            cons_m2 = 1;

            if (consecucion_o2){
                nfichas_o2++;
                nfichas_o2 += 4 * nfichas_o2;
            } else{
                nfichas_o2 += 2;
            }

            consecucion_o2 = true;

        } else {
            nfichas_o2 = 1;
            cons_m2 = 1;
            consecucion_propia2 = false;
            consecucion_o2 = false;
        }

        resultado_test -= nfichas_m;
        resultado_test += nfichas_o;


        resultado_test -= nfichas_m2;
        resultado_test += nfichas_o2;

    }


    // diagonal de izquierda a derecha
    for (int i = 1; i < 4; i++){
        nfichas_o = 0;
        nfichas_m = 0;

        nfichas_o2 = 0;
        nfichas_m2 = 0;


        for (int j = 0; j < 7 - i; j++){
           // parte inferior de la diagonal principal
            if (estado.See_Casilla(i + j, j) == jugador || estado.See_Casilla(i + j, j) == jugador+3){
                consecucion_o = false;
                cons_o = 1;

                if (consecucion_propia){
                    cons_m++;
                    nfichas_m += 4 * cons_m;
                } else {
                    nfichas_m += 2;
                }

                consecucion_propia = true;

            } else if (estado.See_Casilla(i + j, j) != 0){
                consecucion_propia = false;
                cons_m = 1;

                if (consecucion_o){
                    cons_o++;
                    nfichas_o += 4 * cons_o;
                } else{
                    nfichas_o += 2;
                }

                consecucion_o = true;

            } else {
                cons_o = 1;
                cons_m = 1;
                consecucion_propia = false;
                consecucion_o = false;
            }



            // parte superior de la diagonal principal

            if (estado.See_Casilla(j, i + j) == jugador || estado.See_Casilla(j, i + j) == jugador+3){
                consecucion_o2 = false;
                nfichas_o2 = 1;

                if (consecucion_propia2){
                    cons_m2++;
                    nfichas_m2 += 4 * cons_m2;
                } else {
                    nfichas_m2 += 2;
                }

                consecucion_propia2 = true;

            } else if (estado.See_Casilla(j, i + j) != 0){
                consecucion_propia2 = false;
                cons_m2 = 1;

                if (consecucion_o2){
                    nfichas_o2++;
                    nfichas_o2 += 4 * nfichas_o2;
                } else{
                    nfichas_o2 += 2;
                }

                consecucion_o2 = true;

            } else {
                nfichas_o2 = 1;
                cons_m2 = 1;
                consecucion_propia2 = false;
                consecucion_o2 = false;
            }


        }

        resultado_test -= nfichas_m;
        resultado_test += nfichas_o;


        resultado_test -= nfichas_m2;
        resultado_test += nfichas_o2;

    }




    // diagonal de derecha a izquierda
    for (int i = 1; i < 4; i++){
        nfichas_o = 0;
        nfichas_m = 0;

        nfichas_o2 = 0;
        nfichas_m2 = 0;


        for (int j = 0; j < 7 - i; j++){
            if (estado.See_Casilla( 6 - i - j, 6 - j) == jugador || estado.See_Casilla( 6 - i - j, 6 - j) == jugador+3){
                consecucion_o = false;
                cons_o = 1;

                if (consecucion_propia){
                    cons_m++;
                    nfichas_m += 4 * cons_m;
                } else {
                    nfichas_m += 2;
                }

                consecucion_propia = true;

            } else if (estado.See_Casilla(6 - i - j, 6 - j) != 0){
                consecucion_propia = false;
                cons_m = 1;

                if (consecucion_o){
                    cons_o++;
                    nfichas_o += 4 * cons_o;
                } else{
                    nfichas_o += 2;
                }

                consecucion_o = true;

            } else {
                cons_o = 1;
                cons_m = 1;
                consecucion_propia = false;
                consecucion_o = false;
            }





            if (estado.See_Casilla( 6 - j, 6 - i - j) == jugador || estado.See_Casilla(6 - j, 6 - i - j) == jugador+3){
                consecucion_o2 = false;
                nfichas_o2 = 1;

                if (consecucion_propia2){
                    cons_m2++;
                    nfichas_m2 += 4 * cons_m2;
                } else {
                    nfichas_m2 += 2;
                }

                consecucion_propia2 = true;

            } else if (estado.See_Casilla(6 - j, 6 - i - j) != 0){
                consecucion_propia2 = false;
                cons_m2 = 1;

                if (consecucion_o2){
                    nfichas_o2++;
                    nfichas_o2 += 4 * nfichas_o2;
                } else{
                    nfichas_o2 += 2;
                }

                consecucion_o2 = true;

            } else {
                nfichas_o2 = 1;
                cons_m2 = 1;
                consecucion_propia2 = false;
                consecucion_o2 = false;
            }


        }

        resultado_test -= nfichas_m;
        resultado_test += nfichas_o;


        resultado_test -= nfichas_m2;
        resultado_test += nfichas_o2;

    }




    return resultado_test;

}

double horizontalConnect(const Environment &estado, int jugador){

   double resultado_test;

   //TENEMOS 2 CRITERIOS DE EVALUACION HORIZONTAL

   //NUMERO DE FICHAS
   int nfichas_m = 0;
   int nfichas_o = 0;

   //NUMERO DE FICHAS SEGUIDAS (MINIMO 1)
   int cons_m = 1;
   int cons_o = 1;

   bool consecucion_o = false;
   bool consecucion_propia = false;

   for(int i = 0; i < 7; i++){

      nfichas_m = 0;
      nfichas_o = 0;

      for(int j = 0; i < 7; i++){

         // SI HAY UNA FICHA MIA O UNA BOMBAA
         if(estado.See_Casilla(i,j) == jugador or estado.See_Casilla(i,j) == jugador+3){
            consecucion_o = false;
            cons_o = 1;

            if(consecucion_propia){

               cons_m++;
               nfichas_m += 10 * cons_m;

            }else{

               nfichas_m += 5;

            }

            consecucion_propia = true;

         }else if(estado.See_Casilla(i,j) != 0){ // si son fichas del oponente hago lo mismo
            consecucion_propia = false;
            cons_m = 1;

            if(consecucion_o){

               cons_o++;
               nfichas_o += 10 * cons_o;

            }else{

               nfichas_o += 5;

            }

            consecucion_o = true;

         }else{ // AQUI NO HAY NINGUNA CASILLA

            cons_m = 1;
            cons_o = 1;
            consecucion_o = false;
            consecucion_propia = false;
         }


      }

      resultado_test -= nfichas_m;
      resultado_test += nfichas_o;

   }

   return resultado_test;
   //MAXIMIZAMOS QUE EL OPONENTE TENGA FICHAS

}

double verticalConnect(const Environment &estado, int jugador){

   double resultado_test;

   //TENEMOS 2 CRITERIOS DE EVALUACION HORIZONTAL

   //NUMERO DE FICHAS
   int nfichas_m = 0;
   int nfichas_o = 0;

   //NUMERO DE FICHAS SEGUIDAS
   int cons_m = 0;
   int cons_o = 0;

   bool consecucion_o = false;
   bool consecucion_propia = false;

   for(int f = 0; f < 7; f++){

      nfichas_m = 0;
      nfichas_o = 0;

      for(int c = 0; c < 7; c++){

         if( estado.See_Casilla(c,f) == jugador or estado.See_Casilla(c,f) == jugador+3 ){

            consecucion_o = false;
            cons_o = 1;

            if(consecucion_propia){

               cons_m;
               nfichas_m += 10 * cons_m;

            }else{

               nfichas_m += 5;

            }

            consecucion_propia = true;

         }else if(estado.See_Casilla(c,f) != 0){

            consecucion_propia = false;
            cons_m = 1;

            if(consecucion_o){

               cons_o++;
               nfichas_o += 10*cons_o;

            }else{

               nfichas_o += 5;

            }

            consecucion_o = true;

         }else{

            cons_m = 1;
            cons_o = 1;
            consecucion_o = false;
            consecucion_propia = false;

         }

      }

      resultado_test -= nfichas_m;
      resultado_test += nfichas_o;

   }

   return resultado_test;
   
}

double chequeoCompleto(const Environment &estado, int jugador){

   double total = 0.0;

   total += verticalConnect(estado, jugador);
   total += horizontalConnect(estado, jugador);
   total += diagonalConnect(estado, jugador);

   return total;

}


// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador, int depth){

   int ganador = estado.RevisarTablero();

   if (ganador == jugador)
      return 99999999.0 + (10000.0 * depth); // Gana el jugador que pide la valoracion
   else if (ganador != 0)
      return -99999999.0 - (10000.0 * depth); // Pierde el jugador que pide la valoracion
   else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
	{
		
      return chequeoCompleto(estado,jugador);
      
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

      return Valoracion(actual, jugador, depth);

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
   if (actual_.See_Casilla(0,0) == 0 &&
	  actual_.See_Casilla(0,1) == 0 &&
	  actual_.See_Casilla(0,2) == 0 &&
	  actual_.See_Casilla(0,3) == 0 &&
	  actual_.See_Casilla(0,4) == 0 &&
	  actual_.See_Casilla(0,5) == 0 &&
	  actual_.See_Casilla(0,6) == 0) 
	{
	  cout << "ES LA PRIMERA TIRADA" << endl;
	  return Environment::PUT4;

	}
   // Opcion: Poda AlfaBeta
   // NOTA: La parametrizacion es solo orientativa
   valor = poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, menosinf, masinf);
   cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;



   return accion;
}

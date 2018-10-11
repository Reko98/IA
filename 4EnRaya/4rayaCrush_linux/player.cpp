/*******************************************************
GREGORIO VIDOY FAJARDO IA PRACTICA 3
*******************************************************/


#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;


// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar




// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}

//Comprobar horizontal

int ComprobarHorizontal(const Environment &estado, int fil, int col, int jugador,int nconsecutivas){
  int res = 0;


  for (int i=col; i<7 && i < nconsecutivas; i++) {
    if (estado.See_Casilla(fil,i) == (jugador%3)){ //Comparar con Modulo 3 para la bomba tambien
      res++;
    }else{
      //break;
      return i;
    }
  }

  if (res >= nconsecutivas){
    res = nconsecutivas;
  }
  else{
    res = 0;
  }

  return res;
}

//Comprobar vertical

int ComprobarVertical(const Environment &estado, int fil, int col, int jugador, int nconsecutivas){
  int res = 0;
//cout << "\nvertical" << endl;

  for (int i=fil; i<7 && i< nconsecutivas; i++) {
    if (estado.See_Casilla(i,col) == (jugador%3)){ //Comparar con Modulo 3 para la bomba tambien
      res++;
    }else{
      //break;
      return i;
    }
  }

  if (res >= nconsecutivas){
    res = nconsecutivas;
  }else{
    res = 0;
  }

  return res;
}

int ComprobarDiagonalAscen(const Environment &estado, int fil, int col, int jugador, int nconsecutivas) {

  int res = 0;
  int j = col;
  
  for (int i=fil; ((i<7) && (j<7 ));i++,j++) {
    //cout << "\n************************ " << endl;
    //cout << "fila vale = " << i << endl;
    //cout << "columna vale = " << j << endl;
    if (estado.See_Casilla(i,j) == (jugador%3)){ //Comparar con Modulo 3 para la bomba tambien
      res++;
    }else{
      //break;
      return i;
    }
  }

  if (res >= nconsecutivas){
    res = nconsecutivas;
  }else{
    res = 0;
  }

  return res;

}

int ComprobarDiagonalDescen(const Environment &estado, int fil, int col, int jugador, int nconsecutivas) {

  int res = 0;

  int j = col;

  for (int i=fil; ((i>=0) && (j <7));i--,j++) {
    
    if (estado.See_Casilla(i,j) == (jugador%3)){ //Comparar con Modulo 3 para la bomba tambien
      res++;
    }else{
      //break;
      return i;
    }
  }

  if (res >= nconsecutivas){
    res = nconsecutivas;
  }else{
    res = 0;
  }

  return res;

}

int ComprobarProximas(const Environment &estado, int jugador, int nconsecutivas){
  int res = 0;
  int cont = 0;
  for(int i = 0; i < 7; i++){
    for (int j = 0; j < 7; j++){
      if(estado.See_Casilla(i,j) == (jugador%3)){ //Comparar con Modulo 3 para la bomba tambien
        //cout <<"cont vale= " << cont << endl;
        //cont++;
        //cout << "Horizontales= "<<ComprobarHorizontal(estado,i,j,jugador,nconsecutivas) << endl;
        res += ComprobarHorizontal(estado,i,j,jugador,nconsecutivas);
        //cout << "Verticlaes "<<ComprobarVertical(estado,i,j,jugador,nconsecutivas) << endl ;
        res += ComprobarVertical(estado,i,j,jugador,nconsecutivas);
        //cout << "DiagonalAscendente= " <<ComprobarDiagonalAscen(estado,i,j,jugador,nconsecutivas) << endl;       
        //res += 
        res += ComprobarDiagonalAscen(estado,i,j,jugador,nconsecutivas); 
        //cout << "DiagonalDescen= " <<ComprobarDiagonalDescen(estado,i,j,jugador,nconsecutivas);
        res += ComprobarDiagonalDescen(estado,i,j,jugador,nconsecutivas); 
      
      } 
    } 
  }
  return res;
}

// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador){
  double res;
  int cont=0;
  int contC=0;
  int contrario = 1;
  //otra manera de asignar valores
  /*int dos = 0;
  int tres = 0;
  int dosC = 0;
  int tresC = 0;
    */ 
  

  int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero

  if (jugador == 1) 
    contrario = 2;

  for (int i = 2; i < 4; i++)
  {
    cont += ComprobarProximas(estado,jugador,i);
    contC += ComprobarProximas(estado,contrario,i);
  }

  //cout << "\ncont vale: "<< cont << endl;
 // cout << "contC vale: "<< contC << endl;

  //if(cont == contC){
    //res = -10000;
  //}else{
    //res = ((cont * 10000) - (contC * 10000));
    //res = cont - contC;
  //}
  res =cont - contC;
  //otra manera de asignar valores
 /* dos = ComprobarProximas(estado,jugador,2);
  tres = ComprobarProximas(estado,jugador,3);
 

  dosC= ComprobarProximas(estado,jugador,2);
  tresC= ComprobarProximas(estado,jugador,3);


  cont = (dos*10)+(tres*100);
  contC = (dosC*10)+(tresC*10000);
  */

  //res = cont - contC;
  return res;
}


//Funcion minMax

double MiniMax (const Environment &tablero, int jug, int prof, int limite, Environment::ActionType & accion){
  const double INF = 100000000.0, mINF = - 100000000.0, gano = 99999999, pierdo = -99999999;
  Environment::ActionType accion_anterior;
  bool opciones[7];
  double mejor,aux;
  if((prof==limite)|| tablero.JuegoTerminado()){//Condicio de parada
    return Valoracion(tablero,jug);
  } 
  else{ // Proceso recursivo de calculo de MiniMax
    int n_act = tablero.possible_actions(opciones);//Determino cuantas acciones se pueden realizar

    //Inicializo el mejor valor en funcion del nivel

    if(prof%2==0){
      mejor = mINF; //Inicializar un nivel MAX
    }else{
      mejor = INF; //Inicializar un nivel MIN
    }

    int ult_act=-1;
    Environment hijo = tablero.GenerateNextMove(ult_act);//Genero el siguiente tablero

    while(!(hijo==tablero)){// Condicion de que sigo pudiendo aplicar nuevas acciones al tablero
      aux = MiniMax(hijo,jug,prof+1,limite,accion_anterior); //Evaluo el valor del hijo

      if(prof%2==0){ //Juega MAX
        if(aux > mejor){
          mejor=aux;
          accion = static_cast <Environment::ActionType > (ult_act);
        }
      }else{ // Juega MIN
        if(aux < mejor){
          mejor=aux;
          accion = static_cast <Environment::ActionType > (ult_act);
        }
      }
      hijo = tablero.GenerateNextMove(ult_act); //Genero el siguiente descendiente del tablero
    }
    return mejor;
  }

}



//Funcion  Poda alfabeta

double Poda_AlfaBeta (const Environment &tablero, int jug, int prof, int limite, Environment::ActionType & accion,int alfa, int beta){
  const double INF = 100000000.0, mINF = - 100000000.0, gano = 99999999, pierdo = -99999999;
  Environment::ActionType accion_anterior;
  double mejor,aux;
  if(prof==limite || tablero.JuegoTerminado()){//Condicion de parada
    return Valoracion(tablero,jug);
  } 
  else{ // Proceso recursivo de calculo de podaAlfaBeta
    int ult_act=-1;
    Environment hijo = tablero.GenerateNextMove(ult_act);//Genero el siguiente tablero

    while(!(hijo==tablero)){// Condicion de que sigo pudiendo aplicar nuevas acciones al tablero
      aux = Poda_AlfaBeta(hijo,jug,prof+1,limite,accion_anterior,alfa,beta); //Evaluo el valor del hijo
      if(prof == 0){
        //cout << "accion " << ult_act << ": " << aux << endl;
      }
      if(prof%2==0){ //Juega MAX
        if(aux > alfa){
          alfa=aux;
          accion = static_cast <Environment::ActionType > (ult_act);
        }
        if(alfa >= beta){ //Poda
          break; 
        }
      }else{ // Juega MIN
        if(aux < beta){
          beta=aux;
          accion = static_cast <Environment::ActionType > (ult_act);
        }
      if(alfa >= beta){ //Poda
          break; 
        }
      }
      hijo = tablero.GenerateNextMove(ult_act); //Genero el siguiente descendiente del tablero
    }
    if(prof%2==0){
      return alfa;
    }else{
      return beta;
    }
  }
}







// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta
    static bool turno = 0;

    //if(turno == 0){
      //cout << "primera" << endl;
      int cont = 0;
      for (int i = 0; i < 7; i++){
        if(actual_.See_Casilla(0,i) == 0){
          cont++;
        }
      } 
      
      if (cont == 7){
        //cout << "centro" << endl;
        //turno = 1;
        return Environment::PUT4;
      }
    
    
    

    Environment::ActionType accion; // acción que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf;
    double beta = masinf; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;

/*
    //--------------------- COMENTAR Desde aqui
    cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }
*/
    //--------------------- COMENTAR Hasta aqui


    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------


    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    valor = Poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, alpha, beta);
    
    //valor = MiniMax(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion);
    cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

    //int aux = ComprobarProximas(actual_, jugador_,2);
    //cout << "\nvaloracion"<< aux << endl;

    return accion;
}


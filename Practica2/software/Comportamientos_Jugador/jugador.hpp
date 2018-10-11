#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H
#include <list>
#include "comportamientos/comportamiento.hpp"
using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Inicializar Variables de Estado
        fil = col = 99;
        brujula = 0; // 0: Norte  , 1:Este, 2:Sur, 3:Oeste
        ultimaAccion = actIDLE;
        tengo_regalo = false;
        estoy_ejecutando_plan =false;
        error_plan =false;
        orientado = false;
        buscar_puerta = false;
        buscar_gps = false;
        tengo_llaves = false;
        tengo_bikini = false;
        tengo_zapatillas = false;
        buscar_objeto = false;
        planifico = false;
        tiempo = 0;
        atascado = 1;
        capacidad_mochila=0;
        acumulador = 1;
        desatascador = 31;
        for(int i=0; i<200; i++)
            for(int j=0; j<200; j++)
                mapaAuxiliar[i][j]=0;
        for(int i=0; i<200; i++)
            for(int j=0; j<200; j++)
                mapaAuxiliar2[i][j]=0;
        for(int i=0; i<200; i++)
            for(int j=0; j<200; j++)
                mapaIntermedio[i][j] = '?';
        for(int i=0; i<100; i++)
            for(int j=0; j<100; j++)
                mapaObjetos[i][j] = 0;


    }

    void Reinicio();

    void orientacion(Sensores sensores);

    char eleccion();

    bool puede_avanzar(Sensores sensores);

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);

    int interact(Action accion, int valor);


    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}



  private:

    struct estado{
    	int fila;
    	int columna;
    	int orientacion;
    	estado(int fil, int col):fila(fil), columna(col), orientacion(0){}
    };

    struct nodo{
        estado e;
        nodo *padre;
        int distancia;
        nodo(estado es, nodo *pad):e(es), padre(pad){}
        nodo(estado es, nodo *pad, int dist):e(es), padre(pad), distancia(dist){}
        int getFil(){return e.fila;}
        int getCol(){return e.columna;}
    };

    struct CompareDist {
        bool operator()(nodo const & n1, nodo const & n2) {
            return n1.distancia > n2.distancia;
        }
    };


    // Declarar Variables de Estado
    int fil, col, brujula, fil_ant, col_ant, tiempo, capacidad_mochila, acumulador, atascado, desatascador;
    int dx[4] = {0, 0, 1, -1 };
    int dy[4] = {1, -1, 0, 0 };
    Action ultimaAccion;
    bool orientado, buscar_puerta, buscar_gps, buscar_objeto, tengo_llaves, tengo_zapatillas, tengo_bikini, tengo_regalo, estoy_ejecutando_plan, error_plan, planifico;

    list <Action> plan;

    bool pathFinding(const estado &origen, const estado &destino, list<Action> &plan, Sensores sensores);

    estado rey_cercano();
    estado regalo_cercano(Sensores sensores);

    int calcularDistancia(estado e1, estado e2);

    int mapaAuxiliar[200][200];
    int mapaAuxiliar2[200][200];
    char mapaIntermedio[200][200];
    int mapaObjetos[100][100];
    vector<estado> vectorReyes;


};


#endif

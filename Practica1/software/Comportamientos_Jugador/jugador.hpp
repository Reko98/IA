#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Inicializar Variables de Estado
        brujula = 0; // 0:Norte, 1:Este, 2:Sur, 3:Oeste
        ultimaAccion = actIDLE;
        fil = 99;
        col = 99;
        orientado = false;
        buscar_puerta = false;
        buscar_gps = false;
        tengo_llaves = false;
        tengo_bikini = false;
        tengo_zapatillas = false;
        buscar_objeto = false;
        tiempo = 0;
        atascado = 1;
        capacidad_mochila=0;
        acumulador = 1;
        for(int i=0; i<200; i++)
            for(int j=0; j<200; j++)
                mapaAuxiliar[i][j]=0;
        for(int i=0; i<200; i++)
            for(int j=0; j<200; j++)
                mapaAuxiliar2[i][j]=0;
    }

    void Reinicio();

    void orientacion(Sensores sensores);

    char eleccion();

    bool puede_avanzar(Sensores sensores);

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores
                 );

    int interact(Action accion, int valor);


    ComportamientoJugador * clone(){return new ComportamientoJugador(*this);}

  private:
    // Declarar Variables de Estado
    int fil, col, fil_ant, col_ant, brujula, tiempo, capacidad_mochila, acumulador, atascado;
    Action ultimaAccion;
    bool orientado, buscar_puerta, buscar_gps, buscar_objeto, tengo_llaves, tengo_zapatillas, tengo_bikini;
    int mapaAuxiliar[200][200];
    int mapaAuxiliar2[200][200];

};


#endif

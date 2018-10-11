#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <stdlib.h>
#include <list>
using namespace std;





Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;
    tiempo++;
    cout << tiempo;

    //En el caso de morir
    if(sensores.reset)
        Reinicio();


    if(fil == fil_ant && col == col_ant)
        atascado++;
    else
        atascado = 0;

    fil_ant = fil;
    col_ant = col;
	// Actualizar mi información del mundo
	switch(ultimaAccion)
	{
        case actFORWARD: // Avancé
            if(!sensores.colision)
            {
                switch(brujula)
                {
                    case 0: // Mirando Norte
                        fil--;
                        break;
                    case 1: // Mirando Este
                        col++;
                        break;
                    case 2: // Mirando Sur
                        fil++;
                        break;
                    case 3: // Mirando Oeste
                        col--;
                        break;
                }
            mapaAuxiliar[fil][col] = acumulador;
            acumulador++;
            atascado = 0;
            }
            break;
        case actTURN_L: // Giré a la izquierda
            brujula = (brujula+3)%4;
            break;
        case actTURN_R: // Giré a la derecha
            brujula = (brujula+1)%4;
            break;
	}

	if (sensores.terreno[0] == 'K' && !orientado)
        orientacion(sensores);


    if (orientado)
    {
        mapaResultado[fil][col] = sensores.terreno[0];
        int k = 1;
        switch(brujula)
        {
            case 0:
                for(int i=1; i<4; i++)
                {
                    for(int j=-i; j<=i; j++)
                    {
                        mapaResultado[fil-i][col+j] = sensores.terreno[k];
                        k++;
                    }
                }
                break;
            case 1: // Mirando Este
                for(int i=1; i<4; i++)
                {
                    for(int j=-i; j<=(i); j++)
                    {
                        mapaResultado[fil+j][col+i] = sensores.terreno[k];
                        k++;
                    }
                }
                break;
            case 2: // Mirando Sur
                for(int i=1; i<4; i++)
                {
                    for(int j=-i; j<=(i); j++)
                    {
                        mapaResultado[fil+i][col-j] = sensores.terreno[k];
                        k++;
                    }
                }
                break;
            case 3: // Mirando Oeste
                for(int i=1;i<4;i++)
                {
                    for(int j=-i;j<=(i);j++)
                    {
                        mapaResultado[fil-j][col-i]=sensores.terreno[k];
                        k++;
                    }
                }
                break;
        }
    }

	// Siguiente acción a realizar
	if(atascado > 20)
    {
        if(puede_avanzar(sensores))
            accion = actFORWARD;
        else
            accion = actTURN_L;
    }

    else if(!orientado && (sensores.terreno[2] == 'K' || sensores.terreno[6] == 'K' || sensores.terreno[12] == 'K') && puede_avanzar(sensores))
    {
        accion = actFORWARD;
        buscar_gps = false;
    }

    else if(!orientado && (sensores.terreno[1] == 'K' || sensores.terreno[3] == 'K' || buscar_gps))
    {
        if (buscar_gps)
            accion = actTURN_L;
        else if(brujula == 0 || brujula == 2)
        {
            accion = actFORWARD;
            buscar_gps = true;
        }
        else
            {
                if(sensores.terreno[1] == 'K')
                    accion = actTURN_L;
                else if(sensores.terreno[3] == 'K')
                    accion = actTURN_R;
            }

    }
    else if ((sensores.terreno[1] == 'D' || sensores.terreno[2] == 'D' || sensores.terreno[3] == 'D' || buscar_puerta) && (sensores.mochila == '3' || sensores.objetoActivo == '3' || tengo_llaves))
    {
        if(sensores.objetoActivo != '3' )
        {
            if(ultimaAccion == actPUSH || ultimaAccion == actTHROW)
            {
                accion = actPOP;
                capacidad_mochila--;
            }
            else if (ultimaAccion == actPOP)
            {
                accion = actPUSH;
                capacidad_mochila++;
            }
            else if(capacidad_mochila>=4)
            {
                accion = actTHROW;
                if(sensores.objetoActivo == '1')
                    tengo_bikini = false;
                if(sensores.objetoActivo == '2')
                    tengo_zapatillas = false;
                if(sensores.objetoActivo == '3')
                    tengo_llaves = false;
            }
            else
            {
                accion = actPUSH;
                capacidad_mochila ++;
            }
        }
        else if (ultimaAccion == actGIVE)
            accion = actFORWARD;
        else if (sensores.terreno[2] == 'D')
        {
            accion = actGIVE;
            buscar_puerta = false;
        }
        else if (buscar_puerta)
            accion = actTURN_L;
        else
        {
            if(brujula == 0 || brujula == 2)
            {
                accion = actFORWARD;
                buscar_puerta = true;
            }
            else
            {
                if(sensores.terreno[1] == 'D')
                    accion = actTURN_L;
                else if(sensores.terreno[3] == 'D')
                    accion = actTURN_R;
            }
        }
    }
    else if(sensores.superficie[2] == 'l' && sensores.objetoActivo == '0')
    {
            accion = actGIVE;
    }
	else if(puede_avanzar(sensores) && (sensores.superficie[6] == '0' || sensores.superficie[6] == '1' || sensores.superficie[6] == '2' || sensores.superficie[6] == '3'
             || sensores.superficie[12] == '0' || sensores.superficie[12] == '1' || sensores.superficie[12] == '2' || sensores.superficie[12] == '3'))
    {
        accion = actFORWARD;
    }
    else if (sensores.superficie[2] == '0' || sensores.superficie[2] == '1' || sensores.superficie[2] == '2' || sensores.superficie[2] == '3')
    {
        buscar_objeto = false;
        if (sensores.superficie[2] == '1' && capacidad_mochila<4)
            tengo_bikini = true;
        if (sensores.superficie[2] == '2' && capacidad_mochila<4)
            tengo_zapatillas = true;
        if (sensores.superficie[2] == '3' && capacidad_mochila<4)
            tengo_llaves = true;
        if (sensores.objetoActivo == '_')
            accion = actPICKUP;
        else if(capacidad_mochila<4)
        {
            accion = actPUSH;
            capacidad_mochila++;
        }
        else
            accion = actTURN_L;

    }
    else if (sensores.superficie[1] == '0' || sensores.superficie[1] == '1' || sensores.superficie[1] == '2' || sensores.superficie[1] == '3' ||
             sensores.superficie[3] == '0' || sensores.superficie[3] == '1' || sensores.superficie[3] == '2' || sensores.superficie[3] == '3' || buscar_objeto)
    {
        if (buscar_objeto)
            accion = actTURN_L;
        else if(brujula == 0 || brujula == 2)
        {
            if(puede_avanzar(sensores))
            {
                accion = actFORWARD;
                buscar_objeto = true;
            }
            else
                accion = actTURN_L;
        }
        else
            {
                if(sensores.superficie[1] == '0' || sensores.superficie[1] == '1' || sensores.superficie[1] == '2' || sensores.superficie[1] == '3')
                    accion = actTURN_L;
                else if (sensores.superficie[3] == '0' || sensores.superficie[3] == '1' || sensores.superficie[3] == '2' || sensores.superficie[3] == '3')
                    accion = actTURN_R;
            }
    }


    else if ((sensores.objetoActivo == '2' || sensores.mochila == '2' || tengo_zapatillas) && sensores.terreno[2] == 'B' && sensores.terreno[0] != 'A')
    {
        if(sensores.objetoActivo != '2' )
        {
            if(ultimaAccion == actPUSH || ultimaAccion == actTHROW)
            {
                accion = actPOP;
                capacidad_mochila--;
            }
            else if (ultimaAccion == actPOP)
            {
                accion = actPUSH;
                capacidad_mochila++;
            }
            else if(capacidad_mochila>=4)
            {
                accion = actTHROW;
                if(sensores.objetoActivo == '1')
                    tengo_bikini = false;
                if(sensores.objetoActivo == '2')
                    tengo_zapatillas = false;
                if(sensores.objetoActivo == '3')
                    tengo_llaves = false;
            }

            else
            {
                accion = actPUSH;
                capacidad_mochila++;
            }
        }
        else
            accion = actFORWARD;
    }
    else if ((sensores.objetoActivo == '1' || sensores.mochila == '1' || tengo_bikini) && sensores.terreno[2] == 'A' && sensores.terreno[0] != 'B' )
    {
        if(sensores.objetoActivo != '1' )
        {
            if(ultimaAccion == actPUSH || ultimaAccion == actTHROW)
            {
                accion = actPOP;
                capacidad_mochila--;
            }
            else if (ultimaAccion == actPOP)
            {
                accion = actPUSH;
                capacidad_mochila++;
            }
            else if(capacidad_mochila>=4)
            {
                accion = actTHROW;
                if(sensores.objetoActivo == '1')
                    tengo_bikini = false;
                if(sensores.objetoActivo == '2')
                    tengo_zapatillas = false;
                if(sensores.objetoActivo == '3')
                    tengo_llaves = false;
            }
            else
            {
                accion = actPUSH;
                capacidad_mochila++;
            }
        }
        else
            accion = actFORWARD;
    }

    else
    {
        if(eleccion() == 'D')
            accion = actTURN_R;
        else if (eleccion() == 'I')
            accion = actTURN_L;
        else if (eleccion() == 'F' && puede_avanzar(sensores))
            accion = actFORWARD;
        else
        {
            accion = actIDLE;
        }

    }

    cout << endl;
    /*
    for(int i=0; i<30; i++){
        for(int j=0; j<30; j++){
            cout << mapaAuxiliar[i][j] << " ";
        }
        cout << endl;
    }
    */
    ultimaAccion = accion;
	return accion;
}

void ComportamientoJugador::orientacion(Sensores sensores)
{
    fil = sensores.mensajeF;
    col = sensores.mensajeC;
    orientado = true;
    for(int i=0; i<200; i++)
        for(int j=0; j<200; j++)
            mapaAuxiliar[i][j] = mapaAuxiliar2[i][j];
}

void ComportamientoJugador::Reinicio()
{
    brujula = 0; // 0:Norte, 1:Este, 2:Sur, 3:Oeste
    ultimaAccion = actIDLE;
    fil = 99;
    col = 99;
    orientado = false;
    tengo_llaves = false;
    tengo_bikini = false;
    tengo_zapatillas = false;
    buscar_gps = false;
    buscar_puerta = false;
    buscar_objeto = false;
    capacidad_mochila = 0;
    for(int i=0; i<200; i++)
        for(int j=0; j<200; j++)
            mapaAuxiliar2[i][j] = mapaAuxiliar[i][j];
    for(int i=0; i<200; i++)
            for(int j=0; j<200; j++)
                mapaAuxiliar[i][j] = 0;
}

bool ComportamientoJugador::puede_avanzar(Sensores sensores)
{
    if((sensores.terreno[2] == 'T' || sensores.terreno[2] == 'S' || sensores.terreno[2] == 'K') && sensores.superficie[2] == '_')
    {
        return true;
    }
    else if(sensores.objetoActivo == '1' && sensores.terreno[2] == 'A' && sensores.superficie[2] == '_')
        return true;
    else if(sensores.objetoActivo == '2' && sensores.terreno[2] == 'B' && sensores.superficie[2] == '_')
        return true;
    else
    {
        if(sensores.superficie[2] != 'l' && sensores.superficie[2] != 'a')
        {
            switch (brujula)
            {
                case 0:
                    mapaAuxiliar[fil-1][col] = -1;
                    break;
                case 1:
                    mapaAuxiliar[fil][col+1] = -1;
                    break;
                case 2:
                    mapaAuxiliar[fil+1][col] = -1;
                    break;
                case 3:
                    mapaAuxiliar[fil][col-1] = -1;
                    break;
                }
        }
        return false;
    }
}

char ComportamientoJugador::eleccion(){
    list<int> aux;
    if(mapaAuxiliar[fil-1][col] != -1)
        aux.push_back(mapaAuxiliar[fil-1][col]);
    if(mapaAuxiliar[fil][col+1] != -1)
        aux.push_back(mapaAuxiliar[fil][col+1]);
    if(mapaAuxiliar[fil+1][col] != -1)
        aux.push_back(mapaAuxiliar[fil+1][col]);
    if(mapaAuxiliar[fil][col-1] != -1)
        aux.push_back(mapaAuxiliar[fil][col-1]);
    aux.sort();
    switch(brujula)
    {
        case 0:
            if(aux.front() == mapaAuxiliar[fil-1][col])
                return 'F';
            else if(aux.front() == mapaAuxiliar[fil][col+1])
                return 'D';
            else if(aux.front() == mapaAuxiliar[fil][col-1])
                return 'I';
            else if(aux.front() == mapaAuxiliar[fil+1][col])
                    return 'D';
            break;

        case 1:
            if(aux.front() == mapaAuxiliar[fil][col+1])
                return 'F';
            else if(aux.front() == mapaAuxiliar[fil+1][col])
                return 'D';
            else if(aux.front() == mapaAuxiliar[fil-1][col])
                return 'I';
            else if(aux.front() == mapaAuxiliar[fil][col-1])
                return 'D';
            break;

        case 2:
            if(aux.front() == mapaAuxiliar[fil+1][col])
                return 'F';
            else if(aux.front() == mapaAuxiliar[fil][col-1])
                return 'D';
            else if(aux.front() == mapaAuxiliar[fil][col+1])
                return 'I';
            else if(aux.front() == mapaAuxiliar[fil-1][col])
                return 'D';
            break;

        case 3:
            if(aux.front() == mapaAuxiliar[fil][col-1])
                return 'F';
            else if(aux.front() == mapaAuxiliar[fil-1][col])
                return 'D';
            else if(aux.front() == mapaAuxiliar[fil+1][col])
                return 'I';
            else if(aux.front() == mapaAuxiliar[fil][col+1])
                return 'D';
            break;
    }

    aux.clear();
}


int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}

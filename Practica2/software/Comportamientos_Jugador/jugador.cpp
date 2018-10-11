#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <windows.h>
using namespace std;


bool ComportamientoJugador::pathFinding(const estado &origen, const estado &destino, list<Action> &plan, Sensores sensores){

    nodo inicio(origen, NULL);
    priority_queue<nodo, vector<nodo>, CompareDist> abiertos;
    list<nodo> cerrados;
    list<nodo> recorrido;
    abiertos.push(inicio);

    cout << "INICIO:" << inicio.getFil() << " " << inicio.getCol() << endl;
    cout << "DESTINO:" << destino.fila << " " << destino.columna << endl;

    bool visitado[200][200];
    for(int i=0; i<200; i++)
        for(int j=0; j<200; j++)
            visitado[i][j] = false;

    int limit = 0;

    while(!abiertos.empty())
    {
        limit++;
        nodo actual = abiertos.top();
        nodo actual2 = nodo(estado(actual.getFil(),actual.getCol()), actual.padre, actual.distancia);
        //cout << endl << actual2.padre << " " << actual2.getFil() << " " << actual2.getCol() << " " <<  actual2.distancia << endl;
        cerrados.push_back(actual2);
        abiertos.pop();
        //Sleep(1);

        visitado[actual2.getFil()][actual2.getCol()] = true;

        if(actual.getFil() == destino.fila && actual.getCol() == destino.columna)
        {
            //cout << "SOLUCION :";
            //cout << endl << actual.padre << " " << actual.getFil() << " " << actual.getCol() << endl;
            break;
        }

        if(limit > 1000000)
            return false;

        for(int i=0; i<4; i++)
        {
            int fil_h = dx[i] + actual2.getFil();
            int col_h = dy[i] + actual2.getCol();

            //cout << "entro";
            if((mapaResultado[fil_h][col_h] == 'T' || mapaResultado[fil_h][col_h] == 'S' || mapaResultado[fil_h][col_h] == 'K') && !visitado[fil_h][col_h]
               && (mapaObjetos[fil_h][col_h] == 0 || (fil_h==destino.fila && col_h==destino.columna)))
            {
                nodo hijo(estado(fil_h,col_h), &cerrados.back(), calcularDistancia(estado(fil_h,col_h),destino));
                abiertos.push(hijo);
                //cout << "entro al for";
            }
        }
    }

    if(abiertos.empty())
    {
        //cout << "FALLO" << endl;
        return false;
    }



    int fil_r = fil;
    int col_r = col;
    int brujula_r = brujula;
    nodo aux = cerrados.back();
    while(aux.padre != NULL)
    {
        recorrido.push_back(aux);
        aux = *aux.padre;
    }


    while(!recorrido.empty())
    {
        //cout << "CALCULANDO: " << recorrido.back().getFil() << " " << recorrido.back().getCol() << endl;
        switch (brujula_r)
        {
            case 0:
                if(recorrido.back().getFil() == fil_r)
                {
                    if(recorrido.back().getCol() == col_r-1)
                    {
                        plan.push_back(actTURN_L);
                        plan.push_back(actFORWARD);
                        brujula_r = (brujula_r+3)%4;
                        col_r--;
                    }
                    else if(recorrido.back().getCol() == col_r+1)
                    {
                        plan.push_back(actTURN_R);
                        plan.push_back(actFORWARD);
                        brujula_r = (brujula_r+1)%4;
                        col_r++;
                    }
                }
                if(recorrido.back().getCol() == col_r)
                {
                    if(recorrido.back().getFil() == fil_r-1)
                    {
                        plan.push_back(actFORWARD);
                        fil_r--;
                    }
                    else if(recorrido.back().getFil() == fil_r+1)
                    {
                        plan.push_back(actTURN_R);
                        brujula_r = (brujula_r+1)%4;
                        plan.push_back(actTURN_R);
                        brujula_r = (brujula_r+1)%4;
                        plan.push_back(actFORWARD);
                        fil_r++;
                    }
                }
                break;
            case 1:
                if(recorrido.back().getFil() == fil_r)
                {
                    if(recorrido.back().getCol() == col_r-1)
                    {
                        plan.push_back(actTURN_R);
                        brujula_r = (brujula_r+1)%4;
                        plan.push_back(actTURN_R);
                        brujula_r = (brujula_r+1)%4;
                        plan.push_back(actFORWARD);
                        col_r--;
                    }
                    else if(recorrido.back().getCol() == col_r+1)
                    {
                        plan.push_back(actFORWARD);
                        col_r++;
                    }
                }
                if(recorrido.back().getCol() == col_r)
                {
                    if(recorrido.back().getFil() == fil_r-1)
                    {
                        plan.push_back(actTURN_L);
                        brujula_r = (brujula_r+3)%4;
                        plan.push_back(actFORWARD);
                        fil_r--;
                    }
                    else if(recorrido.back().getFil() == fil_r+1)
                    {
                        plan.push_back(actTURN_R);
                        brujula_r = (brujula_r+1)%4;
                        plan.push_back(actFORWARD);
                        fil_r++;
                    }
                }
                break;
            case 2:
                if(recorrido.back().getFil() == fil_r)
                {
                    if(recorrido.back().getCol() == col_r-1)
                    {
                        plan.push_back(actTURN_R);
                        brujula_r = (brujula_r+1)%4;
                        plan.push_back(actFORWARD);
                        col_r--;
                    }
                    else if(recorrido.back().getCol() == col_r+1)
                    {
                        plan.push_back(actTURN_L);
                        brujula_r = (brujula_r+3)%4;
                        plan.push_back(actFORWARD);
                        col_r++;
                    }
                }
                if(recorrido.back().getCol() == col_r)
                {
                    if(recorrido.back().getFil() == fil_r-1)
                    {
                        plan.push_back(actTURN_R);
                        brujula_r = (brujula_r+1)%4;
                        plan.push_back(actTURN_R);
                        brujula_r = (brujula_r+1)%4;
                        plan.push_back(actFORWARD);
                        fil_r--;
                    }
                    else if(recorrido.back().getFil() == fil_r+1)
                    {
                        plan.push_back(actFORWARD);
                        fil_r++;
                    }
                }
                break;
            case 3:
                if(recorrido.back().getFil() == fil_r)
                {
                    if(recorrido.back().getCol() == col_r-1)
                    {
                        plan.push_back(actFORWARD);
                        col_r--;
                    }
                    else if(recorrido.back().getCol() == col_r+1)
                    {
                        plan.push_back(actTURN_R);
                        brujula_r = (brujula_r+1)%4;
                        plan.push_back(actTURN_R);
                        brujula_r = (brujula_r+1)%4;
                        plan.push_back(actFORWARD);
                        col_r++;
                    }
                }
                if(recorrido.back().getCol() == col_r)
                {
                    if(recorrido.back().getFil() == fil_r-1)
                    {
                        plan.push_back(actTURN_R);
                        brujula_r = (brujula_r+1)%4;
                        plan.push_back(actFORWARD);
                        fil_r--;
                    }
                    else if(recorrido.back().getFil() == fil_r+1)
                    {
                        plan.push_back(actTURN_L);
                        brujula_r = (brujula_r+3)%4;
                        plan.push_back(actFORWARD);
                        fil_r++;
                    }
                }
                break;
        }
        //cout << "COMPROBAR: " << fil_r << " " << col_r << " " << brujula_r << endl;
        recorrido.pop_back();
    }
    plan.pop_back();
    if(tengo_regalo)
        plan.push_back(actGIVE);
    else
    {
        if (sensores.objetoActivo == '_')
            plan.push_back(actPICKUP);
        else
        {
            plan.push_back(actTHROW);
            plan.push_back(actPICKUP);
        }

    }


    cout << "PLAN COMPLETADO" << endl;

    return true;
}


bool hayObstaculoDelante(const vector<unsigned char> & terreno, const vector<unsigned char> & superficie){
	if ( (terreno[2]=='T' or terreno[2]=='S' or terreno[2]=='K') and
				superficie[2]=='_'){
					return false;
	}
	else{
	 return true;
 }
}

void PintaPlan(list<Action> plan){
    /*
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
	*/
}


Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;
    tiempo++;
    desatascador++;

    if(tiempo % 1000 == 0)
        cout << tiempo << endl;
    //cout << endl << fil << " " << col << endl;
    //En el caso de morir
    if(sensores.reset)
        Reinicio();


    if(fil == fil_ant && col == col_ant)
        atascado++;
    else
        atascado = 0;

	// Actualización de la información del mundo
	switch (ultimaAccion) {
		case actFORWARD:
		  if (!sensores.colision){
				switch (brujula) {
					case 0: // Norte
					  fil--;
						break;
					case 1: // Este
					  col++;
						break;
					case 2: // Sur
						fil++;
						break;
					case 3: // Oeste
						col--;
						break;
				}
                mapaAuxiliar[fil][col] = acumulador;
                acumulador++;
                atascado = 0;
			}
			break;
		case actTURN_L:
			brujula = (brujula+3)%4;
		  break;
		case actTURN_R:
			brujula = (brujula+1)%4;
		  break;
        case actPUTDOWN:
            atascado=30;
          break;
	}

    fil_ant = fil;
    col_ant = col;

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
                        if(sensores.superficie[k] == 'r')
                            vectorReyes.push_back(estado(fil-i,col+j));
                        if(sensores.superficie[k] == '0' || sensores.superficie[k] == '1' || sensores.superficie[k] == '2' || sensores.superficie[k] == '3' || sensores.superficie[k] == '4')
                            mapaObjetos[fil-i][col+j] = 1;
                        else
                            mapaObjetos[fil-i][col+j] = 0;
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
                        if(sensores.superficie[k] == 'r')
                            vectorReyes.push_back(estado(fil+j,col+i));
                        if(sensores.superficie[k] == '0' || sensores.superficie[k] == '1' || sensores.superficie[k] == '2' || sensores.superficie[k] == '3' || sensores.superficie[k] == '4')
                            mapaObjetos[fil+j][col+i] = 1;
                        else
                            mapaObjetos[fil+j][col+i] = 0;
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
                        if(sensores.superficie[k] == 'r')
                            vectorReyes.push_back(estado(fil+i,col-j));
                        if(sensores.superficie[k] == '0' || sensores.superficie[k] == '1' || sensores.superficie[k] == '2' || sensores.superficie[k] == '3' || sensores.superficie[k] == '4')
                            mapaObjetos[fil+i][col-j] = 1;
                        else
                            mapaObjetos[fil+i][col-j] = 0;
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
                        if(sensores.superficie[k] == 'r')
                            vectorReyes.push_back(estado(fil-j,col-i));
                        if(sensores.superficie[k] == '0' || sensores.superficie[k] == '1' || sensores.superficie[k] == '2' || sensores.superficie[k] == '3' || sensores.superficie[k] == '4')
                            mapaObjetos[fil-j][col-i] = 1;
                        else
                            mapaObjetos[fil-j][col-i] = 0;
                        k++;
                    }
                }
                break;
        }
    }
    else
    {
        mapaIntermedio[fil][col] = sensores.terreno[0];
        int k = 1;
        switch(brujula)
        {
            case 0:
                for(int i=1; i<4; i++)
                {
                    for(int j=-i; j<=i; j++)
                    {
                        mapaIntermedio[fil-i][col+j] = sensores.terreno[k];
                        k++;
                    }
                }
                break;
            case 1: // Mirando Este
                for(int i=1; i<4; i++)
                {
                    for(int j=-i; j<=(i); j++)
                    {
                        mapaIntermedio[fil+j][col+i] = sensores.terreno[k];
                        k++;
                    }
                }
                break;
            case 2: // Mirando Sur
                for(int i=1; i<4; i++)
                {
                    for(int j=-i; j<=(i); j++)
                    {
                        mapaIntermedio[fil+i][col-j] = sensores.terreno[k];
                        k++;
                    }
                }
                break;
            case 3: // Mirando Oeste
                for(int i=1;i<4;i++)
                {
                    for(int j=-i;j<=(i);j++)
                    {
                        mapaIntermedio[fil-j][col-i]=sensores.terreno[k];
                        k++;
                    }
                }
                break;
        }
    }


    if(atascado > 20)
    {
        if(puede_avanzar(sensores))
            accion = actFORWARD;
        else
            accion = actTURN_L;
    }
    else if(sensores.vida < 30)
    {
        if(sensores.objetoActivo != '_')
            accion = actPUTDOWN;
        else
            accion = actPOP;
    }

	else if (error_plan){
        //cout << "PLAN ABORTADO" << endl;
		estoy_ejecutando_plan = false;
		plan.clear();
		error_plan=false;
		tengo_regalo=false;

	}


	else if (estoy_ejecutando_plan and plan.front() == actFORWARD and hayObstaculoDelante(sensores.terreno, sensores.superficie)){
		if (sensores.superficie[2] == 'a' or sensores.superficie[2] == 'l'){
			plan.push_front(actIDLE);
			error_plan = false;
		}
		else {
			error_plan=true;
		}
	}

	else if(!estoy_ejecutando_plan && orientado && !tengo_regalo && planifico && vectorReyes.size() > 0 && desatascador > 100 && sensores.regalos.size() > 0)
    {
        cout << "AQUI";
        estoy_ejecutando_plan = pathFinding(estado(fil,col), regalo_cercano(sensores), plan, sensores);
        if(!estoy_ejecutando_plan)
        {
            desatascador=0;
        }
    }

    else if(!estoy_ejecutando_plan && orientado && tengo_regalo)
    {
        //if((abs(fil - rey_cercano().fila) < 35) && (abs(col - rey_cercano().columna) < 35))
            estoy_ejecutando_plan = pathFinding(estado(fil,col), rey_cercano(), plan, sensores);
		if(!estoy_ejecutando_plan)
        {
            tengo_regalo = false;
            atascado = 30;
        }
    }


    else if (estoy_ejecutando_plan and !error_plan){
		accion = plan.front();
		plan.erase(plan.begin());
		PintaPlan(plan);
		if (plan.size()==0){
		    if(tengo_regalo)
                tengo_regalo = false;
            else
                tengo_regalo = true;
			estoy_ejecutando_plan = false;
		}
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
                if(sensores.objetoActivo == '4')
                    tengo_regalo = false;
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
	else if(puede_avanzar(sensores) && !tengo_regalo && (sensores.superficie[6] == '0' || sensores.superficie[6] == '1' || sensores.superficie[6] == '2' || sensores.superficie[6] == '3'
            || sensores.superficie[6] == '4' || sensores.superficie[12] == '0' || sensores.superficie[12] == '1' || sensores.superficie[12] == '2' || sensores.superficie[12] == '3')
            || sensores.superficie[12] == '4')
    {
        accion = actFORWARD;
    }
    else if ((sensores.superficie[2] == '0' || sensores.superficie[2] == '1' || sensores.superficie[2] == '2' || sensores.superficie[2] == '3' || sensores.superficie[2] == '4') && !tengo_regalo)
    {
        if((sensores.terreno[0] == 'T' || sensores.terreno[0] == 'S' || sensores.terreno[0] == 'K') && sensores.objetoActivo == '_')
        {
            accion = actPICKUP;
            if (sensores.superficie[2] == '1')
                tengo_bikini = true;
            if (sensores.superficie[2] == '2')
                tengo_zapatillas = true;
            if (sensores.superficie[2] == '3')
                tengo_llaves = true;
            if (sensores.superficie[2] == '4')
                tengo_regalo = true;

        }
        else if((sensores.terreno[0] == 'T' || sensores.terreno[0] == 'S' || sensores.terreno[0] == 'K') && capacidad_mochila < 4)
        {
            accion = actPUSH;
            capacidad_mochila++;
        }
        else if((sensores.terreno[0] == 'T' || sensores.terreno[0] == 'S' || sensores.terreno[0] == 'K') && sensores.superficie[2] == '4')
            accion = actTHROW;
        else
        {
            if(puede_avanzar(sensores))
                accion = actFORWARD;
            else
                accion = actTURN_L;
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
                if(sensores.objetoActivo == '4')
                    tengo_regalo = true;
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
                if(sensores.objetoActivo == '4')
                    tengo_regalo = true;
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

    /*
    cout << endl;

    for(int i=0; i<30; i++){
        for(int j=0; j<30; j++){
            cout << mapaObjetos[i][j] << " ";
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
    for(int i=0; i<100; i++)
        for(int j=0; j<100; j++)
            if(mapaIntermedio[i+fil_ant-fil][j+col_ant-col] != '?')
                mapaResultado[i][j] = mapaIntermedio[i+fil_ant-fil][j+col_ant-col];
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
    tengo_regalo = false;
    buscar_gps = false;
    buscar_puerta = false;
    buscar_objeto = false;
    if(tiempo > 11000)
        planifico = true;
    capacidad_mochila = 0;
    for(int i=0; i<200; i++)
        for(int j=0; j<200; j++)
            mapaAuxiliar2[i][j] = mapaAuxiliar[i][j];
    for(int i=0; i<200; i++)
            for(int j=0; j<200; j++)
                mapaAuxiliar[i][j] = 0;
    for(int i=0; i<200; i++)
            for(int j=0; j<200; j++)
                mapaIntermedio[i][j] = '?';
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
        if(sensores.superficie[2] != 'l' && sensores.superficie[2] != 'a' )
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


ComportamientoJugador::estado ComportamientoJugador::rey_cercano()
{
    estado menor(1000,1000);
    for (int i=0; i<vectorReyes.size(); i++)
    {
        int aux1 = abs(fil - vectorReyes[i].fila) + abs(col - vectorReyes[i].columna);
        int aux2 = abs(fil - menor.fila) + abs(col - menor.columna);
        if(calcularDistancia(estado(fil,col), estado(vectorReyes[i].fila,vectorReyes[i].columna)) < calcularDistancia(estado(fil,col), menor))
        {
            menor.fila = vectorReyes[i].fila;
            menor.columna = vectorReyes[i].columna;
        }
    }
    return menor;
}

ComportamientoJugador::estado ComportamientoJugador::regalo_cercano(Sensores sensores)
{
    estado menor(1000,1000);
    for (int i=0; i<sensores.regalos.size(); i++)
    {
        if(calcularDistancia(estado(fil,col), estado(sensores.regalos[i].first,sensores.regalos[i].second)) < calcularDistancia(estado(fil,col), menor))
        {
            menor.fila = sensores.regalos[i].first;
            menor.columna = sensores.regalos[i].second;
        }
    }
    return menor;
}

int ComportamientoJugador::calcularDistancia(estado e1, estado e2)
{
    int resultado = 0;
    int fil_aux = e1.fila;
    int col_aux = e1.columna;
    while(fil_aux != e2.fila)
    {
        if(fil_aux > e2.fila)
            fil_aux--;
        else if (fil_aux < e2.fila)
            fil_aux++;
        resultado++;

    }
    while(col_aux != e2.columna)
    {
        if(col_aux > e2.columna)
            col_aux--;
        else if (col_aux < e2.columna)
            col_aux++;
        resultado++;

    }
    return resultado;
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

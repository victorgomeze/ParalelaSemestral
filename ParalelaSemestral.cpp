#include "mpi.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
using std::cin;
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
#include <string>
using std::string;
using std::getline;
#include <sstream>
using std::stringstream;
#include <math.h>
#include <ctime>
#include <math.h>
#include <cmath> 
#define earthRadiusKm 6371.0

using namespace std;

struct Equipo{
	string nombre;
	string nombre_est;
	float latitud;
	float longitud; 
};

struct partido{
	string visita;
	string local;
	string fecha;
};

void leer_archivo(Equipo V[])// agregar lo que recibirá y donde se guardará
{
   ifstream is("equipos1ra.csv");
   
   if (is){
      string linea;
      int i=0;
      // Mientras se pueda leer una linea del archivo ...
      while (getline(is, linea) && i<16){
         stringstream ss(linea);
         
          // Obtenemos el nombre y descartamos el ';'
         
         getline(ss, V[i].nombre, ';');
         cout << "Nombre: " << V[i].nombre << endl;
         
         // Obtenemos el nombre del estadio, este es el resto de la linea
         
         getline(ss, V[i].nombre_est,';');
         cout << "Apellido: " << V[i].nombre_est << endl;

         // Obtenemos la coordenada 1
         
         ss >> V[i].latitud;
         cout << "No: " << V[i].latitud << endl;
         
         // Descartamos el caracter ';' a continuacion del numero
         char ch;
         ss >> ch;
         
        
         ss >> V[i].longitud;
         cout << "No: " << V[i].longitud << endl;
        
         
         cout << endl;
	i++;
      }
      
      is.close();
   }
   
   
}

// Esta funcion convierte grados decimales a radianes
double deg2rad(float deg) {
  return (deg * M_PI / 180)
  ;
}

// esta funcion convierte radianes a grados decimales
//
double rad2deg(double rad) {
  return (rad * 180 / M_PI);
}

/**
 * @param lat1d Latitud del primer punto en grados
 * @param lon1d Longitud del primer punto en grados
 * @param lat2d Latitud del segundo punto en grados
 * @param lon2d Longitud del segundo punto en grados
 * @return la distancia entre 2 puntos en kilometros
 */
float distanceEarth(float lat1d, float lon1d, float lat2d, float lon2d) {
  float lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}


int main(int argc, char *argv[]){

Equipo Equipos[16]; // Vector del tipo Equipo
leer_archivo( Equipos); // funcion que lee el archivo y lo separa
float MD[16][16]; //matriz de distancias
for (int i=0; i<16;i++){
 //cout << "Nombre equipo = "<< Equipos[i].nombre <<endl;
// cout << "Nombre estadio = "<< Equipos[i].nombre_est <<endl;
 //cout << "latitud = "<< Equipos[i].latitud <<endl;
 //cout << "longitud = "<< Equipos[i].longitud <<endl;
	for (int j=i; j<16 ; j++){
		if ( i==j){
			MD[i][j]=0;
		}
		else{
			MD[i][j]= distanceEarth (Equipos[i].latitud, Equipos[i].longitud, Equipos[j].latitud, Equipos[j].longitud);
			MD[j][i]= MD[i][j];
		}
	

}







}

for (int i=0; i<16;i++){
	for (int j=0; j<16 ; j++){
		cout<<"Distancia entre el estadio "<<Equipos[i].nombre_est<<" y el estadio "<<Equipos[j].nombre_est<<" es = "<< MD[i][j]<< endl;
}
}



}

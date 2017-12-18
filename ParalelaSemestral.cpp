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

void leer_archivo()// agregar lo que recibirá y donde se guardará
{
   ifstream is("nombrearchivo");
   
   if (is){
      string linea;
      
      // Mientras se pueda leer una linea del archivo ...
      while (getline(is, linea)){
         stringstream ss(linea);
         
          // Obtenemos el nombre y descartamos el ';'
         string nombre_equipo;
         getline(ss, nombre_equipo, ';');
         cout << "Nombre: " << nombre_equipo << endl;
         
         // Obtenemos el nombre del estadio, este es el resto de la linea
         string nombre_estadio;
         getline(ss, nombre_estadio);
         cout << "Apellido: " << nombre_estadio << endl;

         // Obtenemos la coordenada 1
         float coordenada1;
         ss >> coordenada1;
         cout << "No: " << coordenada1 << endl;
         
         // Descartamos el caracter ';' a continuacion del numero
         char ch;
         ss >> ch;
         
         float coordenada2;
         ss >> coordenada2;
         cout << "No: " << coordenada2 << endl;
        
         
         cout << endl;
      }
      
      is.close();
   }
   
   return 0;
}
// This function converts decimal degrees to radians
// Esta funcion convierte grados decimales a radianes
double deg2rad(double deg) {
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
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

void ObtenerCoordenadas(float matriz[5000][3]){
  FILE *fichero;
  fichero = fopen("puntos","r");
  if (fichero==NULL){
    printf( "Error al abrir el fichero.\n" );
    system("pause");
    exit (EXIT_FAILURE);
  }
  int i = 0;
  while (1){
    if (feof(fichero))
      break;
    fscanf (fichero, "%f %f %f\n", &matriz[i][0], &matriz[i][1], &matriz[i][2]);
    i++;
   }
   fclose(fichero);
}


int main(int argc, char *argv[]){

}

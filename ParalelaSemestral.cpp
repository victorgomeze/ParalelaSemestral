#include "mpi.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <math.h>
#include <ctime>
#include <math.h>
#include <cmath> 
#define earthRadiusKm 6371.0

using namespace std;

// This function converts decimal degrees to radians
// Esta funcion convierte grados decimales a radianes
double deg2rad(double deg) {
  return (deg * M_PI / 180);
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

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <fstream>
#include <cstring>
#include <vector>
//#include <mpi.h>
#define auxMAX 99999

using namespace std;

typedef char cadena[80];
char *ptr;
double MDC[16][16];//MAtriz de distancias copia
double MDB[16][16];// matriz de distancias, para bloqueos
vector<int> ubi;
vector<int> ubi2;
struct punto
{
  float lat;
  float lon;
};

struct equipo{
  string nombre;
  string nombre_estadio;
  punto coord;
};

struct indices{
  int i,j;
};
int num_est;
vector<equipo> equipos;

bool hab[16];
double **distancias;

double deg2rad(double dato)
{
  float const pi = 3.14159265359;
  return dato = dato*(pi/180);
}



double Dpuntos(float lat1, float long1, float lat2, float long2)//los calculos son obtenidos en Kilometros
{
  float r_t = 6378.7;
  //Se realiza conversion de grados decimales a radianes
  lat1 = deg2rad(lat1);
  lat2 = deg2rad(lat2);
  long1 = deg2rad(long1);
  long2 = deg2rad(long2);

  return  r_t*acos(((sin(lat1))*(sin(lat2)))+(((cos(lat1))*(cos(lat2))*(cos(long2 - long1)))));
}

void llena_matriz_dis() //LLenado de las matrices a utilizar con distancias.
{
  for(int i=0; i<num_est; i++)
  {
    for(int j=0; j<num_est; j++)
    {
      if(i==j){
        distancias[i][j] = auxMAX;
        MDC[i][j] = auxMAX;

      }

      else{
      distancias[i][j] = Dpuntos(equipos[i].coord.lat, equipos[i].coord.lon, equipos[j].coord.lat, equipos[j].coord.lon);
      MDC[i][j] = Dpuntos(equipos[i].coord.lat, equipos[i].coord.lon, equipos[j].coord.lat, equipos[j].coord.lon);
       }


    }
  }
}



int buscar(vector<int> vec, int t) //nos permite buscar un numero dentro del vector, si lo encuentra retorna la posicion de este, si no, nos retornara un -1
{
  int i = 0;
  while (i < vec.size())
  {
    if (vec[i] == t)
      return i;
      else
        i++;

  }
  return -1;
}

void llena_matriz_dis2(vector<int> vec){ // esta funcion copia de la matriz original pero sin reemplazar la diagonal ni los -1 obtenidos
  int i=0;
  for(int j=0; j<num_est; j++)
  {
    for(int i=0; i<num_est; i++)
    {
      if(buscar(ubi,j)%2!=0)// SI la POSICION EL NUMERO(J) CONTENIDO EN EL VECTOR ES IMPAR(VISITA) ENTRA
      {
    	  if(i==j){
      		MDC[i][j] = auxMAX;
        }
  	    if(MDC[i][j]!=auxMAX)
        {
          MDC[i][j] = distancias[i][ubi[buscar(ubi,j)-1]];
        }
      }
      else if(MDC[i][j]!=auxMAX) //SI ES PAR ENTRA
      {
        MDC[i][j] = distancias[i][ubi[buscar(ubi,j)]];
      }
	   }
   }
}

  int leer_equipo(cadena archivo){
    ifstream fs;
    fs.open(archivo);
    if (!fs) cout<<"Error al abrir el fichero a utilizar.\n";
    else {
    int cont=0;
    cadena palabra;
    cout<<"Realizando lectura del archivo seleccionado..."<<endl;
    while(!fs.eof())
    {
      int cont2=0;
      fs.getline(palabra,80,'\n');
      equipo aux;
      
      ptr = strtok(palabra,";");
      while(ptr != NULL)
      {
        if (cont2==0){
        //aqui se optiene el nombre del equipo
        aux.nombre = ptr;
        
        }
        if (cont2==1){
        //aqui obtenemos el nombre del estadio
        aux.nombre_estadio = ptr;
       

        }
        if (cont2==2){
        //se obtiene la latitud
        aux.coord.lat = atof(ptr);
       

        }
        if (cont2==3){
        //se obtiene la longitud
        aux.coord.lon = atof(ptr);
        
        equipos.insert(equipos.begin()+cont,aux);
        cont++;
        }
        ptr = strtok(NULL, ";");
        cont2++;
      }
    }
    return cont;
  }
}


void mostrarDistancia(int i, int j){
  cout<<"La distancia entre "<<equipos[i].nombre<<" y "<<equipos[j].nombre<<" es: "<<distancias[i][j]<<endl;
}

void mostrarEquipo(equipo e){
  cout<<e.nombre<<";"<<e.nombre_estadio<<";"<<e.coord.lat<<";"<<e.coord.lon<<endl;
}

//bool crearExcel(){
//  cout<<"Creando excel de prueba"<<endl;
//  lxw_workbook  *workbook  = workbook_new("prueba.xlsx");
//  lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
//  lxw_format *format = workbook_add_format(workbook);
//  format_set_bold(format);
//  int cont=0;
//  for(int i=0;i<16;i++){
//    for(int j=i+1;j<16;j++){
//      char cadena[30];
//      sprintf(cadena,"%s","Equipo 1");
//      worksheet_write_string(worksheet, cont, 0,cadena, NULL);
//      worksheet_write_string(worksheet, cont, 1,"Fecha", format);
 //     sprintf(cadena,"%s","Equipo 2");
//      worksheet_write_string(worksheet, cont, 2,cadena, NULL);
//      cont++;
//    }
//  }
//  cout<<"Excel creado."<<endl;
//  return workbook_close(workbook);
//}

void mostrarFecha(int i,int j){
  cout<<equipos[i].nombre<<" V/S "<<equipos[j].nombre<<endl;
}

indices minimoPorFila(){
  int menor=auxMAX,cont=0,menor_cont=auxMAX,indice_menor,menores[16];
  indices indice;
  indice.i=-1;
  indice.j=-1;
  for(int i=0;i<16;i++) menores[i]=0;
  for(int i=0;i<num_est;i++){
    for(int j=0;j<num_est;j++){
      if(MDC[i][j]<menor) menores[i]++;
    }
  }
  for(int i=0;i<16;i++){
    if(hab[i] && menores[i]<menor_cont){
      menor_cont=menores[i];
      indice_menor=i;
    }
  }
  for(int i=0;i<num_est;i++){
    if(MDC[indice_menor][i]<menor){
      menor=MDC[indice_menor][i];
      indice.i=indice_menor;
      indice.j=i;
    }
  }
  MDC[indice.i][indice.j]=auxMAX;
  hab[indice.i]=false;
  hab[indice.j]=false;
  
  return indice;
}


void hab_Equipos(){
  for(int i=0;i<16;i++) hab[i]=true;
}

indices minimoPorMatriz(){
  int menor=auxMAX;
  indices indice;
  indice.i=-1;
  indice.j=-1;
  for(int i=0;i<num_est-1;i++){
    for(int j=1;j<num_est;j++){
      if(hab[i] && hab[j]){
        if(MDC[i][j]<menor){
          menor=MDC[i][j];
          indice.i=i;
          indice.j=j;
        }
      }
    }
  }
  hab[indice.i]=false;
  hab[indice.j]=false;
  MDC[indice.i][indice.j]=auxMAX;
  return indice;
}

indices randomIndices(){
  indices indice;
  srand(time(NULL));
  indice.i=rand()%num_est;
  indice.j=rand()%num_est;
  while(indice.i==indice.j)
  {
    indice.j=rand()%num_est;
  }
  cout<<indice.i<<" "<<indice.j<<endl;
  return indice;
}

void copiarMatriz(double original[][16], double copia[][16]){
  for(int i=0;i<16;i++){
    for(int j=0;j<16;j++){
      copia[i][j]=original[i][j];
    }
  }
}

void kernel()
{
  indices indice;
  for(int k=0;k<30;k++){ // RECORRIDO DE CADA FECHA
    bool bloqueado=false;
    A: hab_Equipos();
    std::vector<int>().swap(ubi);
    cout<<"FECHA "<<k+1<<endl;
    copiarMatriz(MDC,MDB);
    for(int y=0;y<8;y++) // PARTIDOS POR FECHA
    {
      
      if(!bloqueado) indice=minimoPorMatriz();
      else indice=minimoPorFila();
      //cout<<"MATRIZ SIN COPIAR"<<endl;
      for (int j=0;j<16;j++)
      {
        for (int i=0;i<16;i++)
        {
         // printf("| %f |",MDC[i][j]);
        }
      printf("\n");
      }
      if(indice.i==-1){
        //cout<<endl<<endl<<endl<<"FALLA BLOQUEO"<<endl<<"*********************************************************************************************"<<endl<<endl<<endl;
        copiarMatriz(MDB,MDC);
        indice=minimoPorFila();
        bloqueado=true;
        goto A;
      }
      ubi.push_back(indice.i);
      ubi.push_back(indice.j);
      MDC[indice.i][indice.j]=auxMAX;
      cout<<equipos[indice.i].nombre<<" V/S "<<equipos[indice.j].nombre<<endl;
    }
   
    llena_matriz_dis2(ubi);
  }
}







int main(int argc, char *argv[])
{
  hab_Equipos();
  num_est = leer_equipo(argv[1]);


  //asignacion de memoria a la matriz dinamica distancias
  distancias = new double*[num_est];
  for(int i=0; i<num_est; i++)
  {
    distancias[i] = new double[num_est];
  }
  //fin de asignacion de memoria

  cout<<"Lineas en el fichero: "<<num_est<<endl;

  //Equipos almacenados
  for(int i=0; i<num_est; i++)
  {
    mostrarEquipo(equipos[i]);
  }

  //Calcula y guarda las distancias en la matriz de distancia
  	llena_matriz_dis();
	kernel();
	//crearExcel();
	randomIndices();
  //Liberacion de memoria 
  for(int i = 0; i < num_est; i++)
  {
    delete[] distancias[i];
  }
  delete[] distancias;
  return 0;
}

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <fstream>
#include <cstring>
#include <vector>
#include <xlsxwriter.h>
#include <mpi.h>
#include <ctime>
#define INFINITO 99999

using namespace std;
typedef char cadena[80];
char *ptr;
unsigned TINICIO, TFINAL;


int hi;
int jota;

double MDC[16][16];
double MDB[16][16];
int ubi[16];


struct punto
{
  float lat;
  float lon;
};

struct equipo{
  string nombre;
  string estadio;
  punto coordenada;
};

struct partido
{
  string local;
  string visita;
  int fecha;
};

struct indices{
  int i,j;
};

int numero_estadios;

vector<equipo> equipos;

vector<partido> partidos;

bool habilitado[16];
//distancia dinamica

double **distancias;

void agrega_partido(string equipo_local, string equipo_visita, int fecha_partido)
{
  partido p;

  p.local = equipo_local;
  p.visita = equipo_visita;
  p.fecha = fecha_partido;

  partidos.push_back(p);
}

double en_radianes(double dato)
{
  float const pi = 3.14159265359;
  return dato = dato*(pi/180);
}


double distancia_long_lat(float lat1, float long1, float lat2, float long2)//los calculos son en km
{
  float radio_tierra = 6378.7;


  //conversion a radianes
  lat1 = en_radianes(lat1);
  lat2 = en_radianes(lat2);
  long1 = en_radianes(long1);
  long2 = en_radianes(long2);

  return radio_tierra*acos(((sin(lat1))*(sin(lat2)))+(((cos(lat1))*(cos(lat2))*(cos(long2 - long1)))));
}

void llenado_matriz_distancia() //FUNCION QUE LLENA 2 MATRICES IGUALES CON LAS DISTANCIAS
{
  for(int i=0; i<numero_estadios; i++)
  {
    for(int j=0; j<numero_estadios; j++)
    {
      if(i==j){
        distancias[i][j] = INFINITO;
        MDC[i][j] = INFINITO;

      }

      else{
      distancias[i][j] = distancia_long_lat(equipos[i].coordenada.lat, equipos[i].coordenada.lon, equipos[j].coordenada.lat, equipos[j].coordenada.lon);
      MDC[i][j] = distancia_long_lat(equipos[i].coordenada.lat, equipos[i].coordenada.lon, equipos[j].coordenada.lat, equipos[j].coordenada.lon);
       }


    }
  }
}


void cambio_filas() // FUNCION QUE COPIA FILAS DESDE MATRIZ ORGINAL SIN CAMBIAR LA DIAGONAL NI LOS -1
{
  int i=0;
  for(int i=0; i<numero_estadios; i++)
  {
    for(int j=0; j<numero_estadios; j++)
    {
      if(i!=ubi[i])// SI la POSICION EL NUMERO(J) CONTENIDO EN EL VECTOR ES IMPAR(VISITA) ENTRA
      {
        if(i==j){
          MDC[i][j] = INFINITO;
        }
        if(MDC[i][j]!=INFINITO)
        {
          MDC[i][j] = distancias[ubi[i]][j];
        }
      }
     }
   }
}

  int leer_equipo(cadena archivo){
    ifstream fs;
    fs.open(archivo);
    if (!fs) cout<<"El fichero no existe o no se logro leer.\n";
    else {
    int cont=0;
    cadena palabra;
    cout<<"Leyendo el fichero de equipos..."<<endl;
    while(!fs.eof())
    {
      int cont2=0;
      fs.getline(palabra,80,'\n');
      equipo aux;
      //cout<<"Dato sin separar: "<<palabra<<endl;
      ptr = strtok(palabra,";");
      while(ptr != NULL)
      {
        if (cont2==0){
        //COLUMNA 1 - NOMBRE EQUIPO
        aux.nombre = ptr;
        //cout <<"Dato separado:"<< ptr << endl;
        }
        if (cont2==1){
        //COLUMNA 2 - NOMBRE ESTADIO
        aux.estadio = ptr;
        //cout <<"Dato separado:"<< ptr << endl;

        }
        if (cont2==2){
        //COLUMNA 3 -  COORDENADA X
        aux.coordenada.lat = atof(ptr);
        //cout <<"Dato separado:"<< ptr << endl;

        }
        if (cont2==3){
        //COLUMNA 4 - COORDENADA Y
        aux.coordenada.lon = atof(ptr);
        //cout <<"Dato separado:"<< ptr << endl;
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

bool crearExcel(cadena arg){
  cout<<"Comenzando a crear Excel del fixture del Campeonato"<<endl;
  string ruta;
  //crea ruta para guardar el archivo
  if (arg!=NULL)
    ruta = strcat(arg,"/FixtureCampeonato.xlsx");
  else
    ruta = "Fixture Campeonato Nacional.xlsx";

  lxw_workbook  *workbook  = workbook_new(ruta.c_str()); //Se crea el fichero excel
  lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL); //Se agrega una hoja al fichero
  lxw_format *bold = workbook_add_format(workbook);
  lxw_format *bold_local = workbook_add_format(workbook);
  lxw_format *bold_fecha = workbook_add_format(workbook);
  lxw_format *local = workbook_add_format(workbook);
  lxw_format *fecha = workbook_add_format(workbook);
  format_set_bold(bold);
  format_set_bold(bold_local);
  format_set_bold(bold_fecha);
  format_set_align(local,LXW_ALIGN_RIGHT);
  format_set_align(fecha,LXW_ALIGN_CENTER);
  format_set_align(bold_local,LXW_ALIGN_RIGHT);
  format_set_align(bold_fecha,LXW_ALIGN_CENTER);
  int cont=1;
  //formato de columnas
  worksheet_set_column(worksheet, 0, 0, 30, local);
  worksheet_set_column(worksheet, 1, 1, 20, fecha);
  worksheet_set_column(worksheet, 2, 2, 30, NULL);
  //nombres de columnas
  worksheet_write_string(worksheet, 0, 0, "Equipo Local", bold_local);
  worksheet_write_string(worksheet, 0, 1, "Fecha", bold_fecha);
  worksheet_write_string(worksheet, 0, 2, "Equipo Visita", bold);

  for(int i=0; i<partidos.size(); i++)
  {
      char cadena[4];

      worksheet_write_string(worksheet, cont, 0, partidos[i].local.c_str(), NULL);

      sprintf(cadena,"%d",partidos[i].fecha);//entero a string
      worksheet_write_string(worksheet, cont, 1, cadena, NULL);

      worksheet_write_string(worksheet, cont, 2, partidos[i].visita.c_str(), NULL);
      cont++;
  }
  cout<<"Excel ha sido creado correctamente."<<endl;
  return workbook_close(workbook);
}

indices minimoPorFila(){


  indices indice;

  for(int i=0;i<numero_estadios;i++)
  {
    for(int j=0;j<numero_estadios;j++)
    {
      if(habilitado[i] && habilitado[j])
      {
        if(MDC[i][j]!=INFINITO)
        {
          indice.i = i;
          indice.j = j;

          MDC[indice.i][indice.j]=INFINITO;
          habilitado[indice.i]=false;
          habilitado[indice.j]=false;
  
          return indice;
        }
      }
    }
  }
  
  MDC[indice.i][indice.j]=INFINITO;
  habilitado[indice.i]=false;
  habilitado[indice.j]=false;
  
  return indice;
}

void habilitarEquipos(){
  for(int i=0;i<numero_estadios;i++) habilitado[i]=true;
}

indices minimopor_matriz(){
  int menor=INFINITO;
  indices indice;
  indice.i=-1;
  indice.j=-1;
  for(int i=0;i<numero_estadios;i++){
    for(int j=0;j<numero_estadios;j++){
      if(habilitado[i] && habilitado[j]){
        if(MDC[i][j]<=menor){
          menor=MDC[i][j];
          indice.i=i;
          indice.j=j;
        }
      }
    }
  }
  if(indice.i != -1 && indice.j != -1)
  {
    habilitado[indice.i]=false;
    habilitado[indice.j]=false;
    MDC[indice.i][indice.j]=INFINITO;
  }
  return indice;
}

indices randomIndices(){
  indices indice;
  srand(time(NULL));
  indice.i=rand()%numero_estadios;
  indice.j=rand()%numero_estadios;
  while(indice.i==indice.j)
  {
    indice.j=rand()%numero_estadios;
  }
  cout<<indice.i<<" "<<indice.j<<endl;
  return indice;
}

void copiarMatriz(double original[][16], double copia[][16]){
  for(int i=0;i<numero_estadios;i++){
    for(int j=0;j<numero_estadios;j++){
      copia[i][j]=original[i][j];
    }
  }
}

void kernel()
{
  indices indice;
  for(int k=0;k<(2*(numero_estadios-1));k++){ // RECORRIDO DE CADA FECHA
    int bloqueado = 0, deshabilita = 0;
    habilitarEquipos();
    for(int i=0; i<numero_estadios;i++)
    {
      ubi[i]=i;
    }
    copiarMatriz(MDC,MDB);
    
    INICIO:
    for(int y=0;y<(numero_estadios/2);y++) // PARTIDOS POR FECHA
    {
      deshabilita++;
      if(bloqueado==1)
      {
        habilitarEquipos();
      }
        if(bloqueado == 0)
        { 
          indice=minimopor_matriz();
        }

        else 
        {
          indice=minimoPorFila();
        }

      if(distancias[indice.i][indice.j]==INFINITO)
      {
        
        copiarMatriz(MDB,MDC);

        //indice=minimoPorFila();
        bloqueado++;
        
        if(deshabilita == 1)
        {
          goto INICIO;
        }
      }

      ubi[indice.i]=indice.j;
      MDC[indice.i][indice.j]=INFINITO;
      
      agrega_partido(equipos[indice.i].nombre, equipos[indice.j].nombre, k+1);
    }
    
    cambio_filas();
    
  }
}

int main(int argc, char *argv[])
{
TINICIO=clock();
  switch(argc){ //Revisa la cantidad de argumentos ingresados al ejecutar el programa
    case 1:
      cout<<"No se ha indicado fichero con los equipos."<<endl;
      break;
    case 2:
      cout<<"No se ha indicado ruta de salida para el fichero Excel."<<endl;
      cout<<"Se utilizara la misma ruta del programa."<<endl;
    case 3:
      {
        habilitarEquipos();
        numero_estadios = leer_equipo(argv[1]);


        //asignacion de memoria a la matriz dinamica distancias
        distancias = new double*[numero_estadios];
        for(int i=0; i<numero_estadios; i++)
        {
          distancias[i] = new double[numero_estadios];
        }
            //fin de asignacion de memoria

               
        //Calcula y guarda las distancias en la matriz de distancia
        llenado_matriz_distancia();

        //crea el fixture
        kernel();

        //por si algun dia se implementa
        //randomIndices();

        crearExcel(argv[2]);

        //se libera la memoria usada por la matriz dinamica
        for(int i = 0; i < numero_estadios; i++)
        {
          delete[] distancias[i];
        }
        delete[] distancias;
      }
      break;
      default:
      cout<<"Demasiados argumentos."<<endl;
      break;
  }
 TFINAL=clock();
 double time = (double(TFINAL-TINICIO)/CLOCKS_PER_SEC);
 printf("El tiempo de ejecucion es: %f\n" ,time);
  return 0;
}

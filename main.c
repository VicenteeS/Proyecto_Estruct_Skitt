#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"
#include "list.h"
#include "treemap.h"

void mostrarMenu();
void cargar_Ejercicios(FILE *, HashMap* , HashMap* );
void *get_csv_field(char * tmp, int k);
HashMap *cargarcsv (FILE *);


typedef struct{
  char nombre[50];
  char edad[50];
  char altura[50]; 
  char peso[50];
  char nivel_exp[50];
  TreeMap *Rutina_fav; 
}Persona;

typedef struct{
  char nombre[50];
  char G_mus [50];
  char espacio [50];
  char tiempo[50];
  char nivel_exp[50];
  List *L_ejercicios;
}Rutina;

typedef struct{
  char nombre[50];
  char link[100];
  char G_mus[50];
  char nivel_exp[50];
}Ejercicio;

int main()
{
  
  
  return 0;
}

void mostrarMenu()
{
  
  return;
}

void cargar_Ejercicios (FILE *file, HashMap* mapaTSup, HashMap* mapaTInf)
{
  file = fopen ("ejercicios.csv", "r");
  Ejercicio *e;
  void* auxi;
  Pair *auxPair;
  List* auxList, *listEj;
  char line[1024];
  int l, contador;
  contador = 0;

  fgets(line, 1023, file);

  while(fgets(line, 1023, file) != NULL)
  {
    for(l=0; l<4; l++)
    {
      auxi = get_csv_field(line, l);

      switch (l)
      {
        case 0: 
          strcpy(e->nombre, auxi);
          contador++;
        break;
        case 1: 
          strcpy(e->link, auxi);
          contador++;
        break;
        case 2:
          strcpy(e->G_mus,auxi);
          contador++;
        break;
        case 3:
          strcpy(e->nivel_exp, auxi);
          contador++;
        break;
      }
      if(contador == 4)
      {
        contador = 0;

        if(strcmp(e->G_mus, "Tren inferior") == 0)
        {
          if(searchMap(mapaTInf,e->nivel_exp) == NULL)
          {
            listEj = createList();
            pushFront(listEj, e);
            
            insertMap(mapaTInf, e->nivel_exp, listEj);
          }
          else
          {
            auxPair = searchMap(mapaTInf, e->nivel_exp);
            auxList = auxPair->value;

            pushFront(auxList, e);
            auxPair->value = auxList;
          }
        }
        else
        {
          if(searchMap(mapaTSup,e->nivel_exp) == NULL)
          {
            listEj = createList();
            pushFront(listEj, e);
            
            insertMap(mapaTSup, e->nivel_exp, listEj);
          }
          else
          {
            auxPair = searchMap(mapaTSup, e->nivel_exp);
            auxList = auxPair->value;

            pushFront(auxList, e);
            auxPair->value = auxList;
          }
        }
      }
    }
  }
}
HashMap *cargarcsv (FILE *file)
{
  HashMap *Usuarios;
  file = fopen ("cargar.csv", "r");
  Persona *e;
  void* auxi;
  List* auxList;
  char line[1024];
  int l, contador;
  contador = 0;

  fgets(line, 1023, file);

  while(fgets(line, 1023, file) != NULL)
  {
    for (l = 0 ;l < 5 ; l++)
      {
        auxi = get_csv_field(line, l);
        switch (l)
        {
          case 0: 
            strcpy(e->nombre, auxi);
            contador++;
          break;
          case 1: 
            strcpy(e->edad, auxi);
            contador++;
          break;
          case 2:
            strcpy(e->altura,auxi);
            contador++;
          break;
          case 3:
            strcpy(e->peso, auxi);
            contador++;
          break;
          case 4:
            strcpy(e->nivel_exp, auxi);
            contador++;
          break; 
        }
        if( contador== 5)
        {
          contador=0;
          if(searchMap(Usuarios,e->nombre)== NULL)
          {
             insertMap(Usuarios, e->nombre, e);
          }       
        }
      }
  }
  return Usuarios;
}
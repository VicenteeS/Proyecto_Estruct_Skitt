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


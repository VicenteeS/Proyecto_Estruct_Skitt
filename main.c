// https://github.com/VicenteeS/Proyecto_Estruct_Skitt.git
#include "hashmap.h"
#include "list.h"
#include "treemap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char nombre[50];
  char edad[50];
  char altura[50];
  char peso[50];
  char nivel_exp[50];
  TreeMap *Rutina_fav;
} Persona;

typedef struct {
  char nombre[50];
  char G_mus[50];
  char tiempo[50];
  char nivel_exp[50];
  List *L_ejercicios;
} Rutina;

typedef struct {
  char nombre[50];
  char link[100];
  char G_mus[50];
  char nivel_exp[50];
} Ejercicio;

void mostrar_Mapa(HashMap*, char*);
int lower_than_string(void *, void *);
void mostrarMenu();
void cargar_Ejercicios(FILE *, HashMap *, HashMap *);
void *get_csv_field(char *tmp, int k);
HashMap *cargardatoscsv(FILE *, HashMap*);
Persona *toma_Datos_Personales(char *, Persona *);
char *quitarSalto(char *linea);
Persona *mostrar_menu_bienvenida(FILE *, HashMap *);
void buscar_ejercicios(HashMap *, HashMap *, Persona *);
void editar_perfil(Persona *);
void Mostrar_rutinas_fav (Persona *);
void Recomienda_rutinas (Persona *, FILE*);




int main() {
  char nombre[50], edad[50], altura[50], peso[50], nivel_exp[50], G_mus[50],
      espacio[50], tiempo[50], link[100], linea[50];

  FILE *file;

  HashMap *mapaTSup = createMap(100);
  HashMap *mapaTInf = createMap(100);
  HashMap *users = createMap(100);
  // List *L_ejercicios;

  // TreeMap *Rutina_fav = createTreeMap(lower_than_string);
  Persona *p = (Persona*)malloc(sizeof(Persona));
  cargar_Ejercicios(file, mapaTSup, mapaTInf);
  users = cargardatoscsv(file, users);
  
  p = mostrar_menu_bienvenida(file, users);
  
  p = toma_Datos_Personales(nombre, p);
  
  

  int opcion;
  while (opcion != 0) {
    do {
      mostrarMenu();

      fflush(stdin);
      scanf("%i", &opcion);
      getchar();
      printf("\n");
    } while (opcion > 6 && opcion < 1);

    switch (opcion) 
    {
    
      case (1):
      break;
      case (2):
      Mostrar_rutinas_fav(p);
      break;
      case (3):
      buscar_ejercicios(mapaTSup, mapaTInf, p);
      break;
      case (4):
      break;
      case (5):
      editar_perfil(p);
      break;
    }
  }
  return 0;
}

void mostrarMenu() {
  printf("Seleccione la opci�n que desea: \n");
  printf("1. Generar rutina\n");
  printf("2. Mostrar rutinas favoritas\n");
  printf("3. Buscar ejercicios\n");
  printf("4. Ver rutinas recomendadas\n");
  printf("5. Editar su perfil\n");
  printf("0. Salir de la aplicaci�n\n");
}

void cargar_Ejercicios(FILE *file, HashMap *mapaTSup, HashMap *mapaTInf) 
{
  char nombre[50],  nivel_exp[50], G_mus[50], link[100];
  
  file = fopen("ejercicios.csv", "r");
  Ejercicio *e;
  void *auxi;
  Pair *auxPair;
  List *auxList, *listEj;
  char line[1024];
  int l, contador;
  contador = 0;

  fgets(line, 1023, file);

  while (fgets(line, 1023, file) != NULL) {
    for (l = 0; l < 4; l++) {
      auxi = get_csv_field(line, l);
      
      switch (l) {
      case 0:
        fflush(stdin);
        strcpy(nombre, auxi);
        strcpy(nombre, quitarSalto(nombre));
        contador++;
        break;
      case 1:
        fflush(stdin);
        strcpy(link, auxi);
        strcpy(link, quitarSalto(link));
        contador++;
        break;
      case 2:
        fflush(stdin);
        strcpy(G_mus, auxi);
        strcpy(G_mus, quitarSalto(G_mus));
        contador++;
        break;
      case 3:
        fflush(stdin);
        strcpy(nivel_exp, auxi);
        strcpy(nivel_exp, quitarSalto(nivel_exp));
        contador++;
        break;
      }
      if (contador == 4) {
        contador = 0;
        Ejercicio *e = (Ejercicio*)malloc(sizeof(Ejercicio));

        strcpy(e->G_mus, G_mus);
        strcpy(e->nivel_exp, nivel_exp);
        strcpy(e->nombre, nombre);
        strcpy(e->link, link);
        if (strcmp(e->G_mus, "Tren inferior") == 0) {
          if (searchMap(mapaTInf, e->nivel_exp) == NULL) {
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
          if (searchMap(mapaTSup, e->nivel_exp) == NULL) {
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
        auxList = NULL;
        auxPair = NULL;
        e = NULL;
        listEj = NULL;
      }
    }
  }
}
HashMap *cargardatoscsv(FILE *file, HashMap* usuarios) {
  file = fopen("cargardatos.csv", "r");
  Persona *e = (Persona *)malloc(sizeof(Persona));
  void *auxi;
  List *auxList;
  char line[1024];
  int l, contador;
  contador = 0;
  fgets(line, 1023, file);
  while (fgets(line, 1023, file) != NULL) {
    for (l = 0; l < 5; l++) {
      auxi = get_csv_field(line, l);
      switch (l) {
      case 0:
        strcpy(e->nombre, auxi);
        contador++;
        break;
      case 1:
        strcpy(e->edad, auxi);
        contador++;
        break;
      case 2:
        strcpy(e->altura, auxi);
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
      if (contador == 5) {
        contador = 0;
        if (searchMap(usuarios, e->nombre) == NULL) {
          insertMap(usuarios, e->nombre, e);
        }
      }
    }
  }
  return usuarios;
}

char *quitarSalto(char *linea) {
  if ((strlen(linea) > 0) && (linea[strlen(linea) - 1] == '\n')) {
    linea[strlen(linea) - 1] = '\0';
  }
  return (linea);
}
Persona *toma_Datos_Personales(char *nombre, Persona *p) {
  int entero;
  int cont = 0;
  do{
    if(cont > 0)
    {
      printf("Ingrese edad en el rango solicitado :");
    }
    else
    {
      printf("Ingrese edad (entre 15 a 60 a�os): ");
    } 
    fgets(p->edad, 30, stdin);
    strcpy(p->edad, quitarSalto(p->edad));
    entero = (int) strtol(p->edad, NULL, 10);
    cont++;
  }while (entero <= 15 && entero >=60);
  cont = 0;
  do{
    if (cont > 0)
    {
      printf("Ingrese peso en el rango soliticado :");
    }
    else
    {
      printf("Ingrese peso (entre 15 a 200 kg): ");
    }
    fgets(p->peso, 30, stdin);
    strcpy(p->peso, quitarSalto(p->peso));
    entero = (int) strtol(p->peso, NULL, 10);
  }while(entero <=15 && entero>=200); 
  cont = 0;
  do{
    if (cont > 0)
    {
      printf("Ingrese altura en el rango solicitado :");
    }
    else
    {
      printf("Ingrese altura (entre 1 a 2 metros): ");
    }
    fgets(p->altura, 30, stdin);
    strcpy(p->altura, quitarSalto(p->altura));
    entero = (int) strtol(p->altura, NULL, 10);
  }while(entero <=1 && entero >=2);
  cont = 0;
  do{
    if (cont > 0)
    {
      printf("Ingrese una de las opciones correctas");
    }
    else
    {
      printf("Ingrese nivel de experiencia: \n ");
      printf("Tienes 3 opciones\nPrincipiante: 0 - Intermedio: 1 - Avanzado: 2\n");
    }
    fgets(p->nivel_exp, 30, stdin);
    strcpy(p->nivel_exp, quitarSalto(p->nivel_exp));
    entero = (int) strtol(p->nivel_exp, NULL, 10);
  }while(entero <= 0 && entero >=2 );
  
  return (p);
}

Persona *mostrar_menu_bienvenida(FILE *file, HashMap *users) {
  Persona *p = (Persona*)malloc(sizeof(Persona));
  Pair *aux;
  
  printf("BIENVENIDO A WORKOUT AT HOME\n\n");
  printf("Por favor, Ingrese nombre de usuario :");
  fgets(p->nombre, 30, stdin);
  strcpy(p->nombre, quitarSalto(p->nombre));
  
  aux = searchMap(users, p->nombre);
  if (aux == NULL) {
    file = fopen("cargardatos.csv", "w");

    if (!file)
      exit(EXIT_FAILURE);

    fprintf(file, "%s,", p->nombre);

    fclose(file);
    printf("TU USUARIO HA SIDO CREADO!!!\n\nA ENTRENAR!!!\n");
    insertMap(users, p->nombre, p);
    return (p);
  } else {
    printf("HOLA OTRA VEZ, ES UN GUSTO VERTE!!!\n\n");
    return (aux->value);
  }
}

void buscar_ejercicios(HashMap *mapaTSup, HashMap *mapaTInf, Persona *p) {
  int entero;
  char gM[50];
  Pair *auxPair;
  List *auxList;
  Ejercicio *e;
  printf("Por favor, Ingrese un grupo muscular:\n"
         "Puede ser:\n 1.Tren Superior \n 2.Tren Inferior\n");
  fflush(stdin);
  fgets(gM, 50, stdin);
  strcpy(gM, quitarSalto(gM));
  //Corroborar los datos 

  /*printf("Por favor, Ingrese un nivel de experiencia\n");
  printf("Principiante: 0\nIntermedio: 1\nAvanzado: 2\n");
  fflush(stdin);
  fgets(nivel, 50, stdin);
  strcpy(nivel, quitarSalto(nivel));*/

  /*if(strcmp(p->nivel_exp, "0") == 0)
  {
    
    
    //strcpy(p->nivel_exp,"Principiante");
    
  }
  if(strcmp(p->nivel_exp, "1") == 0)
  {
    strcpy(p->nivel_exp,"Intermedio");
  }
  if(strcmp(p->nivel_exp, "2") == 0)
  {
    strcpy(p->nivel_exp,"Avanzado");
  }*/
  
  if (strcmp(gM, "1") == 0) {
    /*printf("\na!!!!!!!\n");
    auxPair = searchMap(mapaTSup,nivel);
    if (auxPair != NULL) 
    {
      auxList = auxPair->value;
      printf("b!!!!!!!\n");
      for (e = firstList(auxList); e != NULL; e = nextList(auxList)) 
      {
        printf("nombre: %s\n", e->nombre);
        printf("link: %s\n\n", e->link);
      }
    }*/
    mostrar_Mapa(mapaTSup, p->nivel_exp);
  } 
  else 
  {
    /*
    auxPair = searchMap(mapaTInf,nivel);
    printf("\nb!!!!!!!\n");
    
    if (auxPair != NULL) 
    {
      auxList = auxPair->value;
      printf("a!!!!!!!\n");
      for (e = firstList(auxList); e != NULL; e = nextList(auxList)) 
      {
        printf("nombre: %s\n", e->nombre);
        printf("link: %s\n\n", e->link);
      }
    }*/
    mostrar_Mapa(mapaTInf, p->nivel_exp);
  }
}
int lower_than_string(void *key1, void *key2) {
  char *k1 = (char *)key1;
  char *k2 = (char *)key2;
  if (strcmp(k1, k2) < 0)
    return 1;
  return 0;
}
void *get_csv_field(char *tmp, int k) {
  int open_mark = 0;
  char *ret = (char *)malloc(100 * sizeof(char));
  int ini_i = 0, i = 0;
  int j = 0;
  while (tmp[i + 1] != '\0') {
    if (tmp[i] == '\"') {
      open_mark = 1 - open_mark;
      if (open_mark)
        ini_i = i + 1;
      i++;
      continue;
    }
    if (open_mark || tmp[i] != ',') {
      if (k == j)
        ret[i - ini_i] = tmp[i];
      i++;
      continue;
    }
    if (tmp[i] == ',') {
      if (k == j) {
        ret[i - ini_i] = 0;
        return ret;
      }
      j++;
      ini_i = i + 1;
    }
    i++;
  }
  if (k == j) {
    ret[i - ini_i] = 0;
    return ret;
  }
  return NULL;
}

void mostrar_Mapa(HashMap* mapa, char* nivel)
{
  
  Pair *aux;
  List *lista;
  Ejercicio *e;
  //printf("NIVEL: %s\n", nivel);
  for(aux = firstMap(mapa); aux != NULL; aux = nextMap(mapa))
  {
    lista = aux->value;
    for(e = firstList(lista); e != NULL; e = nextList(lista))
    {
      printf("Nombre: %s\n", e->nombre);
      printf("Nivel de Experiencia: %s\n", e->nivel_exp);
      printf("Link: %s\n\n\n", e->link);
    }
  }
  /*printf("Ingresa un nivel de experiencia: \n");
  fgets( nivel, 30, stdin);
  strcpy(nivel, quitarSalto(nivel));

  aux = searchMap(mapa, nivel);
  if(aux != NULL)
  {
    lista = aux->value;
    for(e = firstList(lista); e != NULL; e = nextList(lista))
    {
      printf("nombre: %s\n", e->nombre);
      printf("Nivel de Experiencia: %s\n", e->nivel_exp);
      printf("link: %s\n\n", e->link);
    }
  }*/
  
  
  return;
}

void editar_perfil(Persona *p)
{
  int opcion, cont, entero;
  do
  {
    printf("Editor de Datos del Perfil:\n");
    printf("Ingrese el numero de una de las opciones disponibles:\n");
    printf("1. Edad \n2. Peso \n3. Altura \n4.Nivel de Experiencia \n0. Salir\n");

    fflush(stdin);
    scanf("%i", &opcion);
    getchar();
    printf("\n");
    
    switch(opcion)
    {
      case(0):
      break;
      case(1):
        do{
          if(cont > 0)
          {
            printf("Ingrese edad en el rango solicitado :");
          }
          else
          {
            printf("Ingrese edad (entre 15 a 60 a�os): ");
          } 
          fgets(p->edad, 30, stdin);
          strcpy(p->edad, quitarSalto(p->edad));
          entero = (int) strtol(p->edad, NULL, 10);
          cont++;
        }while (entero <= 15 && entero >=60);
        cont = 0;
      break;
      case(2):
        do{
          if (cont > 0)
          {
            printf("Ingrese peso en el rango soliticado :");
          }
          else
          {
            printf("Ingrese peso (entre 15 a 200 kg): ");
          }
          fgets(p->peso, 30, stdin);
          strcpy(p->peso, quitarSalto(p->peso));
          entero = (int) strtol(p->peso, NULL, 10);
        }while(entero <=15 && entero>=200); 
        cont = 0;
      break;
      case(3):
        do{
          if (cont > 0)
          {
            printf("Ingrese altura en el rango solicitado :");
          }
          else
          {
            printf("Ingrese altura (entre 1 a 2 metros): ");
          }
          fgets(p->altura, 30, stdin);
          strcpy(p->altura, quitarSalto(p->altura));
          entero = (int) strtol(p->altura, NULL, 10);
        }while(entero <=1 && entero >=2);
        cont = 0;
      break;
      case(4):
        do{
          if (cont > 0)
          {
            printf("Ingrese una de las opciones correctas");
          }
          else
          {
            printf("Ingrese nivel de experiencia: \n ");
            printf("Tienes 3 opciones\nPrincipiante: 0 - Intermedio: 1 - Avanzado: 2\n");
          }
          fgets(p->nivel_exp, 30, stdin);
          strcpy(p->nivel_exp, quitarSalto(p->nivel_exp));
          entero = (int) strtol(p->nivel_exp, NULL, 10);
          }while(entero <=0 && entero >=2);
      break;
    }
  }while(opcion != 0);
  
}

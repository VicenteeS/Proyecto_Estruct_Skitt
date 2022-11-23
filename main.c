/// https://github.com/VicenteeS/Proyecto_Estruct_Skitt.git

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
  char espacio[50];
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

void mostrarMenu();
void cargar_Ejercicios(FILE *, HashMap *, HashMap *);
void *get_csv_field(char *tmp, int k);
HashMap *cargarcsv(FILE *);
Persona *toma_Datos_Personales(char *nombre, Persona *p);
char *quitarSalto(char *linea);
Persona *mostrar_menu_bienvenida(FILE *, HashMap *users);
void buscar_ejercicios(HashMap *mapaTSup, HashMap *mapaTInf);

int main() { return 0; }

void mostrarMenu() { return; }

void cargar_Ejercicios(FILE *file, HashMap *mapaTSup, HashMap *mapaTInf) {
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
        strcpy(e->nombre, auxi);
        contador++;
        break;
      case 1:
        strcpy(e->link, auxi);
        contador++;
        break;
      case 2:
        strcpy(e->G_mus, auxi);
        contador++;
        break;
      case 3:
        strcpy(e->nivel_exp, auxi);
        contador++;
        break;
      }
      if (contador == 4) {
        contador = 0;

        if (strcmp(e->G_mus, "Tren inferior") == 0) {
          if (searchMap(mapaTInf, e->nivel_exp) == NULL) {
            listEj = createList();
            pushFront(listEj, e);

            insertMap(mapaTInf, e->nivel_exp, listEj);
          } else {
            auxPair = searchMap(mapaTInf, e->nivel_exp);
            auxList = auxPair->value;

            pushFront(auxList, e);
            auxPair->value = auxList;
          }
        } else {
          if (searchMap(mapaTSup, e->nivel_exp) == NULL) {
            listEj = createList();
            pushFront(listEj, e);

            insertMap(mapaTSup, e->nivel_exp, listEj);
          } else {
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
HashMap *cargarcsv(FILE *file) {
  HashMap *Usuarios;
  file = fopen("cargar.csv", "r");
  Persona *e;
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
        if (searchMap(Usuarios, e->nombre) == NULL) {
          insertMap(Usuarios, e->nombre, e);
        }
      }
    }
  }
  return Usuarios;
}

char *quitarSalto(char *linea) {
  if ((strlen(linea) > 0) && (linea[strlen(linea) - 1] == '\n')) {
    linea[strlen(linea) - 1] = '\0';
  }
  return (linea);
}

Persona *toma_Datos_Personales(char *nombre, Persona *p) {
  printf("Ingrese edad: ");
  fgets(p->edad, 30, stdin);
  strcpy(p->edad, quitarSalto(p->edad));

  printf("Ingrese peso: ");
  fgets(p->peso, 30, stdin);
  strcpy(p->peso, quitarSalto(p->peso));

  printf("Ingrese altura: ");
  fgets(p->altura, 30, stdin);
  strcpy(p->altura, quitarSalto(p->altura));

  printf("Ingrese nivel de experiencia: \n Ingrese:\n");
  printf("Principiante: 0\nIntermedio: 1\nAvanzado: 2");
  fgets(p->nivel_exp, 30, stdin);
  strcpy(p->nivel_exp, quitarSalto(p->nivel_exp));

  return (p);
}

Persona *mostrar_menu_bienvenida(FILE *file, HashMap *users) {
  Persona *p;
  printf("BIENVENIDO A WORKOUT AT HOME\n\n");
  printf("Por favor, Ingrese nombre: ");
  fgets(p->nombre, 30, stdin);
  strcpy(p->nombre, quitarSalto(p->nombre));
  Pair *aux;
  aux = searchMap(users, p->nombre);
  if (aux == NULL) {
    file = fopen("cargar.csv", "w");

    if (!file)
      exit(EXIT_FAILURE);

    fprintf(file, "%s,", p->nombre);

    fclose(file);
    printf("TU USUARIO HA SIDO CREADO!!!\n\nA ENTRENAR!!!\n'");
    insertMap(users, p->nombre, p);
    return (p);
  } else {
    printf("HOLA OTRA VEZ, ES UN GUSTO VERTE!!!\n\n");
    return (aux->value);
  }
}

void buscar_ejercicios(HashMap *mapaTSup, HashMap *mapaTInf) {
  void *gM, *nivel;
  Pair *listAUX;
  Ejercicio *e;
  printf("Por favor, Ingrese un grupo muscular:\n"
         "Puede ser:\n 1.Tren Superior \n 2.Tren Inferior\n");
  fgets(gM, 30, stdin);
  strcpy(gM, quitarSalto(gM));

  printf("Por favor, Ingrese un nivel de experiencia\n");
  printf("Principiante: 0\nIntermedio: 1\nAvanzado: 2");
  fgets(nivel, 30, stdin);
  strcpy(nivel, quitarSalto(nivel));

  if (strcmp(gM, "1") == 0) {
    listAUX = searchMap(mapaTSup, "nivel");
    if (listAUX != NULL) {
      for (e = firstList(listAUX->value); e != NULL;
           e = nextList(listAUX->value)) {
        printf("nombre: %s\n", e->nombre);
        printf("link: %s\n\n", e->link);
      }
    }
  } else {
    listAUX = searchMap(mapaTInf, "nivel");

    if (listAUX != NULL) {
      for (e = firstList(listAUX->value); e != NULL;
           e = nextList(listAUX->value)) {
        printf("nombre: %s\n", e->nombre);
        printf("link: %s\n\n", e->link);
      }
    }
  }
}
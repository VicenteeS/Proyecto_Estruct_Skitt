// https://github.com/VicenteeS/Proyecto_Estruct_Skitt.git
//Librerias estandar
#include "hashmap.h"
#include "list.h"
#include "treemap.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Estructuras que se utilizaran
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

//Funciones 
Ejercicio *recomienda_ejercicios (int , char* , HashMap* );
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
void Generar_rutina (FILE *, Persona *, Rutina *, HashMap* , HashMap* );

//Funcion principal
int main() 
{
  //variables o tdas utilizadas. Ademas reserva de memoria de las struct
  char nombre[50];
  
  Rutina *r = (Rutina *) malloc (sizeof(Rutina));
  Persona *p = (Persona*)malloc(sizeof(Persona));
  
  FILE *file;
  
  HashMap *mapaTSup = createMap(100);
  HashMap *mapaTInf = createMap(100);
  HashMap *users = createMap(100);
  
  //Se cargan los datos en los mapas que guardan los ejercicios
  cargar_Ejercicios(file, mapaTSup, mapaTInf);
  
  //Se cargan los usuarios que han utilizado la app
  users = cargardatoscsv(file, users);
  
  //Se toman toman los datos del usuario
  p = mostrar_menu_bienvenida(file, users);
  p = toma_Datos_Personales(nombre, p);
  
  int opcion;
  while (opcion != 0) {
    do {
      //Menú con las opciones de la app
      mostrarMenu();
      fflush(stdin);
      scanf("%i", &opcion);
      getchar();
      printf("\n");
    } while (opcion > 6 && opcion < 1);

    //Cada caso corresponde a una opcion del menu
    switch (opcion) 
    {
      case(0):
        free(p);
        free(users);
        free(mapaTInf);
        free(mapaTSup);
        free(file);
      break;
      case (1):
        Generar_rutina(file, p, r, mapaTInf, mapaTSup);
        break;
      case (2):
        Mostrar_rutinas_fav(p);
        break;
      case (3):
        buscar_ejercicios(mapaTSup, mapaTInf, p);
        break;
      case (4):
        Recomienda_rutinas(p, file);
        break;
      case (5):
        editar_perfil(p);
        break;
    }
  }
  return 0;
}
//Función que muestra el menú de opciones
void mostrarMenu() {
  printf("Seleccione la opción que desea: \n");
  printf("1. Generar rutina\n");
  printf("2. Mostrar rutinas favoritas\n");
  printf("3. Buscar ejercicios\n");
  printf("4. Ver rutinas recomendadas\n");
  printf("5. Editar su perfil\n");
  printf("0. Salir de la aplicación\n");
}

/*Funcion que recorre el archivo CSV para ir guardando los ejercicios en los mapas recibidos, ya sean correspondientes a tren superior o inferior.*/
void cargar_Ejercicios(FILE *file, HashMap *mapaTSup, HashMap *mapaTInf) 
{
  char nombre[50],  nivel_exp[50], G_mus[50], link[100];
  Ejercicio *e;
  void *auxi;
  Pair *auxPair;
  List *auxList, *listEj;
  char line[1024];
  int l, contador;
  
  file = fopen("ejercicios.csv", "r");
  
  contador = 0;
  fgets(line, 1023, file);
  
  //Carga los ejercicios dependiendo del caso en el que estemos
  while (fgets(line, 1023, file) != NULL) {
    for (l = 0; l < 4; l++) {
      auxi = get_csv_field(line, l);

      switch (l) 
      {
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
      if (contador == 4) 
      {
        contador = 0;
        Ejercicio *e = (Ejercicio*)malloc(sizeof(Ejercicio));
        strcpy(e->G_mus, G_mus);
        strcpy(e->nivel_exp, nivel_exp);
        strcpy(e->nombre, nombre);
        strcpy(e->link, link);
        // Dependiendo si es tren inferior o superior crea un lista que se insertará en un mapa 
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

/*Funcion que recorre el archivo CSV para ir guardando los usuarios en el mapa
recibido. Cada linea corresponde a un dato tipo Persona.*/
HashMap *cargardatoscsv(FILE *file, HashMap* usuarios) {
  Persona *e = (Persona *)malloc(sizeof(Persona));
  void *auxi;
  List *auxList;
  char line[1024];
  int l, contador;
  
  file = fopen("cargardatos.csv", "r");
  
  contador = 0;
  fgets(line, 1023, file);
  
  while (fgets(line, 1023, file) != NULL) 
  {
    for (l = 0; l < 5; l++) 
    {
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
//Funcion que quita el salto de linea proporcionado por la funcion fgets
char *quitarSalto(char *linea) {
  if ((strlen(linea) > 0) && (linea[strlen(linea) - 1] == '\n')) {
    linea[strlen(linea) - 1] = '\0';
  }
  return (linea);
}

//Toma los datos de la persona que usará la app, debe ingresar los valores en los rangos solicitados
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
      printf("Ingrese edad (entre 15 a 60 años): ");
    } 
    fgets(p->edad, 30, stdin);
    strcpy(p->edad, quitarSalto(p->edad));
    entero = (int) strtol(p->edad, NULL, 10);
    cont++;
  }while (entero < 15 || entero >60);
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
    cont++;
  }while(entero <15 || entero>200); 
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
    cont++;
  }while(entero <1 || entero >2);
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
    cont++;
  }while(entero < 0 || entero >2 );
  return (p);
}

//Bienvenida.  Se muestraal usuario al iniciar la app
//Compara el nombre con los datos del mapa Users para ver si el usuario existe
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

/*Funcion que dependiendo de la opción ingresada por el usuario, mostrará
mapaTSup o mapaTInf que es donde están guardados los ejercicios*/
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
  //Llama a la funcion mostrar_Mapa para que se vean los ejercicios
  if (strcmp(gM, "1") == 0) {
    mostrar_Mapa(mapaTSup, p->nivel_exp);
  } 
  else 
  {
    mostrar_Mapa(mapaTInf, p->nivel_exp);
  }
}

//Funcion que compara las keys
int lower_than_string(void *key1, void *key2) {
  char *k1 = (char *)key1;
  char *k2 = (char *)key2;
  if (strcmp(k1, k2) < 0)
    return 1;
  return 0;
}

//Funcion que optiene cada dato entre las comas del archivo CSV
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

/*Funcion llamada en la funcion "buscar_ejercicios", recorre el mapa del
grupo muscular que escogio el usuario para mostrar sus datos*/
void mostrar_Mapa(HashMap* mapa, char* nivel)
{
  Pair *aux;
  List *lista;
  Ejercicio *e;
  //Cada posicion del mapa tiene asociada una lista como value, que se recorre igualmente
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
  return;
}

//Permite editar los datos del perfil 
void editar_perfil(Persona *p)
{
  int opcion, cont=0, entero;
  do
  {
    printf("Editor de Datos del Perfil:\n");
    printf("Ingrese el numero de una de las opciones disponibles:\n");
    printf("1. Edad \n2. Peso \n3. Altura \n4.Nivel de Experiencia \n0. Salir\n");
    //Se pregunta que le gustaría editar y luego lo hace
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
            printf("Ingrese edad (entre 15 a 60 años): ");
          } 
          fgets(p->edad, 30, stdin);
          strcpy(p->edad, quitarSalto(p->edad));
          entero = (int) strtol(p->edad, NULL, 10);
          cont++;
        }while (entero < 15 || entero >60);
        cont = 0;
      break;
      case(2):
        do
        {
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
          cont++;
        }while(entero <15 || entero>200); 
        cont = 0;
      break;
      case(3):
        do
        {
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
          cont++;
        }while(entero <1 || entero >2);
        cont = 0;
      break;
      case(4):
        do
        {
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
          cont++;
        }while(entero <0 || entero >2);
      break;
    }
  }while(opcion != 0);
}

//Muestra las rutinas favoritas guardadas en el mapa de la struct Persona
void Mostrar_rutinas_fav (Persona *p)
{
  Rutina *r;
  PairTree *aux;
  List *lista;
  Ejercicio *e;
  int i = 1;
  for(aux = firstTreeMap(p->Rutina_fav); aux != NULL; aux = nextTreeMap(p->Rutina_fav))
  {
    printf("%i. ", i);
    i++;
    r = aux->value;
    printf("%s \n", r->nombre);
    printf("Nivel de Experiencia: %s \n", r->nivel_exp);
    printf("Tiempo: %s \n", r->tiempo);
    printf("Grupo Muscular: %s \n\n", r->G_mus);
    lista = r->L_ejercicios;
    printf("Ejercicios: \n");
    for(e = firstList(lista); e != NULL; e = nextList(lista))
    {
      printf("%s\n", e->nombre);
    }
  }
}
/*Funcion que compara las rutinas que han sido agregadas a fav por otros 
usuarios en el archivo CSV con los datos del usuario*/
void Recomienda_rutinas (Persona *p, FILE*file)
{
  void *auxi;
  char line[1024], l_Ejercicios[1024];
  int l, contador;
  Rutina *r;
  
  file = fopen("rutinas_Fav.csv", "r");
  
  contador = 0;
  
  //Recorre el archivo csv y muestra las rutinas guardadas ahí si coincide el nivel de experiencia
  fgets(line, 1023, file);
  while (fgets(line, 1023, file) != NULL) 
  {
    for (l = 0; l < 5; l++) 
    {
      auxi = get_csv_field(line, l);
      switch (l) 
      {
        case(0):
          strcpy(r->nivel_exp, auxi);
          contador++;
        break;
        case(1):
          strcpy(r->nombre, auxi);
          contador++;
        break;
        case(2):
          strcpy(r->G_mus, auxi);
          contador++;
        break;
        case(3):
          strcpy(r->tiempo, auxi);
          contador++;
        break;
        case(4):
          strcpy(l_Ejercicios, auxi);
          contador++;
        break;
      }
      if(contador == 5)
      {
        contador = 0;
        if(strcmp(p->nivel_exp, r->nivel_exp) == 0)
        {
          printf("Nombre: %s\n", r->nombre);
          printf("Ejercicios:");
          printf("%s\n", l_Ejercicios);
          printf("Grupo muscular %s\n",r->G_mus);
          printf("Tiempo %s\n",r->tiempo);
        }
        r = NULL;
      }
    }    
  }
}

/*Funcion que segun las variables que ingrese el usuario, se van guardando
ejercicios para armar una rutina*/
void Generar_rutina (FILE *file, Persona *p, Rutina *r, HashMap *mapaTInf, HashMap *mapaTSup)
{
  int tiempo, grupoMus, opcion;
  Ejercicio *e;
  List *lista = createList();
  
  //Pregunta por el tiempo y que parte quiere trabajar 
  do
  {
    printf("Ingrese tiempo de la rutina\nIngrese una"
           "de las siguientes alternativas:\n");
    printf("1. 30 minutos\n2. 60 minutos\n3. 90 minutos\n");
    fflush(stdin);
    scanf("%i", &tiempo);
    getchar();
    printf("\n");
  }while(tiempo<1 || tiempo>3);
  if(tiempo == 1)
  {
    tiempo = 4;
  }
  if(tiempo == 2)
  {
    tiempo = 6;
  }
  if(tiempo == 3)
  {
    tiempo = 8;
  }
  do
  {
    printf("¿Que musculo desea trabajar?\nIngrese una"
           "de las siguientes alternativas:\n");
    printf("1. Tren Inferior\n2. Tren Superior\n3. Full Body\n");
    fflush(stdin);
    scanf("%i", &grupoMus);
    getchar();
    printf("\n");
  }while(grupoMus<1 || grupoMus>3);
/*Dependediendo de la opción ingresada llama a la funcion "recomienda_ejercicios"
que retorna un ejercicio que será guardado en una lista*/
  if(grupoMus == 1)
  {
    strcpy(r->G_mus, "Tren Inferior");
    for(int i = tiempo; i > 0; i--)
    {
      e = recomienda_ejercicios(grupoMus, p->nivel_exp, mapaTInf);
      pushFront(lista, e);
    }
  }
  if(grupoMus == 2)
  {
    strcpy(r->G_mus, "Tren Superior");
    for(int i = tiempo; i > 0; i--)
    {
      e = recomienda_ejercicios(grupoMus, p->nivel_exp, mapaTSup);
      pushFront(lista, e);
    }
  }
  if(grupoMus == 3)
  {
    strcpy(r->G_mus, "Full Body");
    for(int i = tiempo/2; i > 0; i--)
    {
      e = recomienda_ejercicios(grupoMus, p->nivel_exp, mapaTSup);
      pushFront(lista, e);
    }
    for(int i = tiempo/2; i > 0; i--)
    {
      e = recomienda_ejercicios(grupoMus, p->nivel_exp, mapaTInf);
      pushFront(lista, e);
    }
  }
  //Muestra los ejercicios generados
  for(e = firstList(lista); e != NULL; e = nextList(lista))
  {
    printf("Nombre ejercicio %s\n :",e->nombre);
  }
  r->L_ejercicios = lista;
  //Pregunta si quiere agregar la rutina generada a favoritas, si elige la opcion 1 se guardará la rutina en el csv
  printf("¿Desea agregar la rutina a favoritas? \n");
  printf("Si la quiere agregar, ingrese 1: \nSi no, ingrese cualquier valor:\n");
  fflush(stdin);
  scanf("%i", &opcion);
  getchar();
  printf("\n");
  if (opcion==1)
  {
    printf("Por favor, Ingrese nombre de la rutina :\n");
    fgets(r->nombre, 30, stdin);
    strcpy(r->nombre, quitarSalto(r->nombre));
    strcpy(r->nivel_exp, p->nivel_exp);
    file = fopen("rutinas_Fav.csv", "w");
    if (!file)
      exit(EXIT_FAILURE);
    fprintf(file, "%s,", r->nivel_exp);
    fprintf(file, "%s,", r->nombre);
    fprintf(file, "%s,", r->G_mus);
    fprintf(file, "%s,", r->tiempo);
    for(e = firstList(r->L_ejercicios); e != NULL; e = nextList(r->L_ejercicios))
    {
      fprintf(file, "%s-", e->nombre);
    }

    fclose(file);
  }
}
/*En el mapa, segun el nivel de experiencia del usuario, se recorre la lista 
asociada a la posición una cantidad aleatoria de veces, y retornando el 
ejercicio que hay en esa posicion*/
Ejercicio *recomienda_ejercicios (int grupo, char* nivel, HashMap* mapa)
{
  Pair *aux;
  List* listaAUX;
  Ejercicio *e;
  int num = rand()%10;
  
  aux = searchMap(mapa, nivel);
  if(aux != NULL)
  {
    listaAUX = aux->value;
    for(e = firstList(listaAUX); e != NULL; e = nextList(listaAUX))
    {
      num--;
      if(num == 0)
      {
        return(e);
      }
    }
  }
  return(e);
}
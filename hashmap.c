#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  if(map == NULL || key == NULL)
    return;
  long posicion = hash(key,map->capacity); // posición inicial pa insertar la clave-valor, función hash devuelve un valor que representa la posición en la que se debe insertar el par
  while(map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL) //busca la posicion vacia, y si esta lleno avanza  ala siguiente
    {
      if(is_equal(map->buckets[posicion]->key, key)) //verifica si la clave existe
      {
        map->buckets[posicion]->value = value; //actualiza el valor asociado a la clave
        return;
      }
      posicion = (posicion + 1) % map->capacity; //avanza ala siguiente posicion
    }
  if(map->size == map->capacity)  //si esta lleno:
    enlarge(map); //alarga la capacidad
  map->buckets[posicion] = createPair(key,value); // al encontrar una posición vacia, crea un nuevo par clave-valor y lo inserta en esa posición
  map->size++; //incrementa el tamaño dsp de crear la posicion
}

void enlarge(HashMap * map) {//alarga la capacidad
  enlarge_called = 1; //no borrar (testing purposes)
  

}


HashMap * createMap(long capacity) {
  
  HashMap *map = (HashMap*) malloc(sizeof(HashMap));
  if(map == NULL)
    return NULL;
  map->buckets = (Pair**)malloc(capacity * sizeof(Pair*)); //reserva memoria para el contenedor , donde se guardn las claves y valor
  map->capacity = capacity; 
  map->size = 0; //se hicia en 0 ya que notengo elementos 
  map->current = -1;
  return map;
}

void eraseMap(HashMap * map,  char * key) {    
  if(map == NULL || key == NULL)
    return;
  long posicion = hash(key, map->capacity);
  while(map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL){
    if (is_equal(map->buckets[posicion]->key, key)){
      map->buckets[posicion]->key = NULL;
      map->buckets[posicion]->value = NULL;
      map->size--;
      return;
    }   
    posicion = (posicion + 1) % map->capacity;
    }
    
}

Pair * searchMap(HashMap * map,  char * key) {   
  if(map == NULL || key == NULL)
    return NULL;
  long posicion = hash(key,map->capacity);
  while(map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL){
    if(is_equal(map->buckets[posicion]->key, key))
    {
      map->current = posicion; 
      return map->buckets[posicion];
    }
    posicion = (posicion + 1) % map->capacity;
  }
  return NULL;
}

Pair * firstMap(HashMap * map) {
  if(map == NULL || map->size == 0)
    return NULL;
  map->current = 0;
  while(map->current < map->capacity){
    if(map->buckets[map->current] != NULL && map->buckets[map->current]->key != NULL )
      return map->buckets[map->current];
    map->current++;
  }
  map->current = -1;
  return NULL;
}

Pair * nextMap(HashMap * map) {
  if(map == NULL || map->size == 0 || map->current == -1)
    return NULL;
  map->current++;
  while(map->current < map->capacity)
    {
      if (map->buckets[map->current] != NULL &&
        map->buckets[map->current]->key != NULL) {
      return map->buckets[map->current];
      }
      map->current++;

    }
  map->current = -1;
  return NULL;

}

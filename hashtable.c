/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
    for(int i = 0; i < HT_SIZE; i++) 
    { 
    (*table)[i] = NULL; //Do not cast! 
    }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  int i=get_hash(key);
  if((*table)[i]==NULL)
  {
    return NULL;
  }
  else
  {
    ht_item_t *item;
    if((*table)[i]->key==key)
    {
      return (*table)[i];
    }
    else
    {
      item=(*table)[i]->next;

      while(item!=NULL)
      {
        if(item->key==key)
        {
          return item;
        }
        item=item->next;
        
      }
      return NULL;
    }
  }
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    int hash=get_hash(key);
    ht_item_t *item;
    item=ht_search(table,key);
    if(item!=NULL){
      item->value=value;
    }

    else{
      item=malloc(sizeof(ht_item_t));
      if(item==NULL)
      {
        exit(-1);
      }
      item->key=key;
      item->value=value;
      item->next=(*table)[hash];
      (*table)[hash]=item;
    }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *item;
  item=ht_search(table,key);
  if(item==NULL)
  {
    return NULL;
  }
  else
  {
    return &(item->value); 
  }
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  ht_item_t *item;
  ht_item_t *prev;
  prev=NULL;
  int i=get_hash(key);
  item=(*table)[i];
  while(item!=NULL)
  {
    if(item->key==key)
    {
      if(prev!=NULL)
      {
        prev->next=item->next;
        free(item);
        item=NULL;
        return;
      }
      else
      {
        (*table)[i]=item->next;
        free(item);
        item=NULL;
        return;
      }
    }
    prev=item;
    item=item->next;
  }

}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
   for(int i = 0; i < HT_SIZE; i++) 
    { 
      while((*table)[i]!=NULL)
      {
        ht_delete(table,(*table)[i]->key);
      }  
    }
}

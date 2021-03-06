#pragma once
#include "linked_list.h"
#include "disastrOS_pcb.h"
#include "disastrOS_constants.h"

#define DSOS_STANDARD_RESOURCE  0 //risorsa stardand
#define DSOS_MSG_QUEUE_RESOURCE 1 //la risorsa è un messaggio di coda TIPO

typedef struct {
  ListItem list;
  const char *name;
  int rid;
  int type;
  ListHead descriptors_ptrs;
} Resource;

void Resource_init();

Resource* Resource_alloc(const char *name, int rid, int type);
int Resource_free(Resource* resource);

typedef ListHead ResourceList;

Resource* ResourceList_byId(ResourceList* l, int id);

void ResourceList_print(ListHead* l);

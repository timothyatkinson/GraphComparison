#include <time.h>
#include "common.h"
#include "debug.h"
#include "graphStacks.h"
#include "parser.h"
#include "eggp_mutate_node.h"

#include "eggp_mutate_node_pickNode.h"
Morphism *M_eggp_mutate_node_pickNode = NULL;
#include "eggp_mutate_node_mutateNode.h"
Morphism *M_eggp_mutate_node_mutateNode = NULL;

static void eggp_mutate_node_freeMorphisms(void)
{
   freeMorphism(M_eggp_mutate_node_pickNode);
   freeMorphism(M_eggp_mutate_node_mutateNode);
}

Graph* eggp_mutate_node_host = NULL;
int* eggp_mutate_node_node_map = NULL;
MorphismPot* eggp_mutate_node_pot = NULL;

static void eggp_mutate_node_garbageCollect(void)
{
   eggp_mutate_node_freeMorphisms();
   freePot(eggp_mutate_node_pot);
}

bool eggp_mutate_node_success = true;

int eggp_mutate_node_execute(Graph* host_graph)
{
   eggp_mutate_node_host = host_graph;
   eggp_mutate_node_success = true;
   eggp_mutate_node_pot = makeMorphismPot();
   emptyPot(eggp_mutate_node_pot);
   M_eggp_mutate_node_pickNode = makeMorphism(1, 0, 1);
   M_eggp_mutate_node_mutateNode = makeMorphism(2, 0, 3);

   /* Rule Call */
   emptyPot(eggp_mutate_node_pot);
   fillpoteggp_mutate_node_pickNode(eggp_mutate_node_pot, M_eggp_mutate_node_pickNode);
   if(potSize(eggp_mutate_node_pot) > 0){
      MorphismHolder *holder = drawFromPot(eggp_mutate_node_pot);
      duplicateMorphism(holder->morphism, M_eggp_mutate_node_pickNode, eggp_mutate_node_host);
      freeMorphism(holder->morphism);
      free(holder);
      applyeggp_mutate_node_pickNode(M_eggp_mutate_node_pickNode, false);
      eggp_mutate_node_success = true;
   }
   else
   {
      printf("No output graph: rule eggp_mutate_node_pickNode not applicable.\n");
      eggp_mutate_node_garbageCollect();
      return 0;
   }
   emptyPot(eggp_mutate_node_pot);
   /* Rule Call */
   emptyPot(eggp_mutate_node_pot);
   fillpoteggp_mutate_node_mutateNode(eggp_mutate_node_pot, M_eggp_mutate_node_mutateNode);
   if(potSize(eggp_mutate_node_pot) > 0){
      MorphismHolder *holder = drawFromPot(eggp_mutate_node_pot);
      duplicateMorphism(holder->morphism, M_eggp_mutate_node_mutateNode, eggp_mutate_node_host);
      freeMorphism(holder->morphism);
      free(holder);
      applyeggp_mutate_node_mutateNode(M_eggp_mutate_node_mutateNode, false);
      eggp_mutate_node_success = true;
   }
   else
   {
      printf("No output graph: rule eggp_mutate_node_mutateNode not applicable.\n");
      eggp_mutate_node_garbageCollect();
      return 0;
   }
   emptyPot(eggp_mutate_node_pot);
   eggp_mutate_node_garbageCollect();
   return 0;
}

